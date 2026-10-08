#Here is how to run this server!

## 1. Compile the Code
Use the standard GCC compiler to build the server (`bserve`) and client (`bcurl`) executables:
```bash
gcc bserve.c -o bserve
gcc bcurl.c -o bcurl
```

2.You must open two separate terminal windows to test the connection.
Terminal 1 (Start the Server):
Run the server executable, providing the root directory and the port number.   
```bash
./bserve ./www 9000
```
Terminal 2 (Run the Client):
Run the client executable with the verbose flag (-v) to request the file and trigger the hexdump output.   
```bash
./bcurl -v localhost:9000/index.html
```

After running this we'll see the hex for request and response
