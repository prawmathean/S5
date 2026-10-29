# Concurrent File Server (Fork-based Multi-client Architecture)

## What is this experiment all about?
When multiple people visit a website or download a file at the same time, the server cannot make Client B wait until Client A finishes their 2-hour download. A server must be **concurrent**—handling many clients simultaneously.

This experiment implements a **multi-process concurrent TCP server** in C using the Unix `fork()` system call:
- The parent server sits in an endless loop waiting at the door (`accept()`).
- The moment a client connects, the parent clones itself using `fork()`, producing a child process dedicated specifically to that client.
- The parent immediately goes back to listening for more connections.
- The child process serves the client's file request and then cleanly exits, preventing zombie processes via a `SIGCHLD` signal handler.

## What is the code actually doing?
1. **Server (`server.c`)**:
   - Binds to port `8080` and listens for TCP connections.
   - When a client connects, it forks a child process.
   - The child transmits its own Process ID (PID) to the client so the client can verify which worker process is serving it.
   - Reads the requested filename sent by the client.
   - Opens the file on disk and streams its contents across the network in 2048-byte chunks until EOF.
   - If the file does not exist, sends an error message: `"ERROR: File not found on server."`
   - Automatically cleans up child exit statuses using `waitpid()` inside a `SIGCHLD` handler to avoid dead/zombie processes.
2. **Client (`client.c`)**:
   - Asks the user which file they want to download.
   - Connects to the server over TCP port `8080`.
   - Prints the server child's PID.
   - Sends the filename and prints the streamed contents directly to the terminal.

## How to compile and run
1. Open a terminal and compile both programs:
   ```bash
   gcc -Wall -Wextra server.c -o server
   gcc -Wall -Wextra client.c -o client
   ```
2. Start the server:
   ```bash
   ./server
   ```
3. In another terminal (or multiple terminals simultaneously), run the client:
   ```bash
   ./client
   ```
   *Example input*: Enter `server.c` to have the server stream its own source code back to you!
