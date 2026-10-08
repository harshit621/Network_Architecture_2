// bcurl.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "protocol.h"

// -v hexdumps every frame[cite: 1]
void hexdump(const uint8_t* data, size_t size, const char* label) {
    printf("--- %s (%zu bytes) ---\n", label, size);
    for (size_t i = 0; i < size; ++i) {
        printf("%02x ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n-----------------------\n");
}

int main(int argc, char* argv[]) {
    int verbose = 0;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        }
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }
    
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9000);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // And never open a second connection[cite: 1]
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return 1;
    }

    // Build the binary request frame[cite: 1]
    const char* path = "/index.html"; 
    uint16_t path_len = (uint16_t)strlen(path);
    uint8_t header_id = 0x01; // E.g., 0x01 maps to the ":path" header
    
    size_t payload_size = sizeof(header_id) + sizeof(path_len) + path_len;
    size_t total_frame_size = sizeof(FrameHeader) + payload_size;
    
    FrameHeader req_header;
    memset(&req_header, 0, sizeof(req_header));
    req_header.type = 0; // 0 = Request frame
    req_header.length = (uint32_t)payload_size;
    
    // Serialize into one buffer for sending
    uint8_t* frame_buffer = (uint8_t*)malloc(total_frame_size);
    memcpy(frame_buffer, &req_header, sizeof(FrameHeader));
    
    size_t offset = sizeof(FrameHeader);
    memcpy(frame_buffer + offset, &header_id, sizeof(header_id));
    offset += sizeof(header_id);
    memcpy(frame_buffer + offset, &path_len, sizeof(path_len));
    offset += sizeof(path_len);
    memcpy(frame_buffer + offset, path, path_len);

    if (verbose) {
        hexdump(frame_buffer, total_frame_size, "OUTGOING REQUEST FRAME");
    }

    send(sock, frame_buffer, total_frame_size, 0);
    free(frame_buffer);

    // Read the response header
    FrameHeader res_header;
    if (recv(sock, &res_header, sizeof(res_header), 0) == sizeof(res_header)) {
        
        // Read the response payload
        uint8_t* res_payload = (uint8_t*)malloc(res_header.length);
        recv(sock, res_payload, res_header.length, 0);
        
        if (verbose) {
            size_t total_res_size = sizeof(FrameHeader) + res_header.length;
            uint8_t* full_res_frame = (uint8_t*)malloc(total_res_size);
            memcpy(full_res_frame, &res_header, sizeof(FrameHeader));
            memcpy(full_res_frame + sizeof(FrameHeader), res_payload, res_header.length);
            
            hexdump(full_res_frame, total_res_size, "INCOMING RESPONSE FRAME");
            free(full_res_frame);
        }

        // Read the response, body to stdout[cite: 1]
        fwrite(res_payload, 1, res_header.length, stdout);
        printf("\n");
        free(res_payload);
    }
    
    close(sock);
    
    // Exit non-zero on 4xx / 5xx[cite: 1] (Logic goes here based on server reply)
    return 0;
}
