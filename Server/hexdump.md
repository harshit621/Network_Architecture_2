=== ANNOTATED HEXDUMP BREAKDOWN ===
Here is the annotated hexdump of one complete request and response.

--- OUTGOING REQUEST FRAME (20 bytes) ---
Raw: 00 0e 00 00 00 00 01 0b 00 2f 69 6e 64 65 78 2e 68 74 6d 6c 

[Fixed-Size Header: 6 bytes]
00          -> Frame Type (0 = Request Frame)
0e 00 00 00 -> Payload Length (14 bytes in little-endian format)
00          -> Flags (0 = None)

[Payload: 14 bytes]
01          -> Header ID Number (1 = assigned to ":path")
0b 00       -> Length-prefix for the following string (11 bytes in little-endian format)
2f 69 6e 64 65 78 2e 68 74 6d 6c -> The ASCII string "/index.html"


--- INCOMING RESPONSE FRAME (27 bytes) ---
Raw: 01 15 00 00 00 00 c8 00 48 65 6c 6c 6f 20 66 72 6f 6d 20 54 72 61 63 6b 20 31 21 

[Fixed-Size Header: 6 bytes]
01          -> Frame Type (1 = Response Frame)
15 00 00 00 -> Payload Length (21 bytes in little-endian format)
00          -> Flags (0 = None)

[Payload: 21 bytes]
c8 00       -> Status Code (200 OK in little-endian format)
48 65 6c 6c 6f 20 66 72 6f 6d 20 54 72 61 63 6b 20 31 21 -> The ASCII string "Hello from Track 1!"

