// protocol.h
#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <stdint.h>

// Fixed-size frame header
#pragma pack(push, 1) // Ensure no padding bytes are added by the compiler
typedef struct {
    uint8_t type;       // Frame type (e.g., 0=Request, 1=Response)
    uint32_t length;    // Length of the payload following this header
    uint8_t flags;      // Flags (e.g., end of stream)
} FrameHeader;
#pragma pack(pop)

#endif
