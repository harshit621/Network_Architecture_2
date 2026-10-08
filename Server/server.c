
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "protocol.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

void handle_connection(int client_socket) {
    while (1) { // Keep the connection open
        FrameHeader header;
        
        // Read one binary request frame[cite: 1]
        ssize_t bytes_read = recv(client_socket, &header, sizeof(header), 0);
        
        if (bytes_read <= 0) break; // Client disconnected gracefully or error
        if (bytes_read != sizeof(header)) {
            // Rule: 400 if the frame is malformed[cite: 1]
            // (Implementation of sending the 400 reply goes here)
            break;
        }

        // Rule: A receiver meeting a frame type it does not know MUST skip it cleanly[cite: 1]
        if (header.type > 1 /* assuming 0 and 1 are known types */) {
            char discard_buf[1024];
            size_t remaining = header.length;
            while (remaining > 0) {
                size_t to_read = MIN(remaining, sizeof(discard_buf));
                recv(client_socket, discard_buf, to_read, 0);
                remaining -= to_read;
            }
            continue; // Go back to waiting for the next frame
        }
	
        FrameHeader res_header;
        memset(&res_header, 0, sizeof(res_header));
        res_header.type = 1; // 1 = Response frame
        const char* reply_body = "Hello from Track 1!";
        res_header.length = (uint32_t)strlen(reply_body);

        // Reply: status, headers, the bytes
        send(client_socket, &res_header, sizeof(res_header), 0);
        send(client_socket, reply_body, res_header.length, 0);


        // To do for final project: Read the actual payload, map the path to a file under a root, and reply[cite: 1]
    }
    close(client_socket);
}

int main(int argc, char* argv[]) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }
    
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9000); // Listening on port 9000
    
    if (bind(server_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return 1;
    }
    
    // Accept a TCP connection[cite: 1]
    listen(server_socket, 3);
    printf("Server listening on port 9000...\n");
    
    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket >= 0) {
            handle_connection(client_socket);
        }
    }
    
    close(server_socket);
    return 0;
}
