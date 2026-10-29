# Algorithm: Concurrent File Transfer Server using TCP Sockets and Fork

## Server Algorithm
1. **Initialize & Setup**:
   - Create a TCP stream socket (`socket(AF_INET, SOCK_STREAM, 0)`).
   - Set socket options (`SO_REUSEADDR`) to allow immediate port reuse.
   - Configure server address structure (`AF_INET`, `INADDR_ANY`, Port 8080).
   - Bind socket to IP and port (`bind()`).
   - Listen for incoming client connections (`listen()`, backlog 5).
   - Register a signal handler for `SIGCHLD` (`sigaction` with `waitpid(-1, &status, WNOHANG)`) to automatically reap zombie child processes.

2. **Connection Loop**:
   - Loop indefinitely:
     - Block and accept an incoming client connection (`accept()`), receiving `client_socket`.
     - Create a child process using `fork()`.
     - **In Parent Process**: Close `client_socket` immediately (delegated to child) and continue listening.
     - **In Child Process**: Close listening `server_fd` and call `handle_client(client_socket)`:
       a. Get child's PID using `getpid()`.
       b. Send `PID` to client via `send()`.
       c. Receive requested filename from client via `recv()`.
       d. Attempt to open requested file in binary read mode (`fopen(filename, "rb")`).
       e. If file not found, send an error string to the client.
       f. If file found, read file in blocks of 2048 bytes and stream via `send()` until EOF (`fclose()`).
       g. Close `client_socket` and terminate child process (`exit(0)`).

---

## Client Algorithm
1. **Input & Setup**:
   - Prompt user for target filename to request.
   - Create a TCP stream socket (`socket(AF_INET, SOCK_STREAM, 0)`).
   - Configure server address (`127.0.0.1`, Port 8080) using `inet_pton()`.

2. **Connect & Request**:
   - Establish TCP three-way handshake with server (`connect()`).
   - Receive server child process PID via `recv()` and display it.
   - Send requested filename to server via `send()`.

3. **Receive & Display**:
   - Enter a loop calling `recv()` until the server closes the connection (returns 0).
   - Null-terminate and print received byte chunks to standard output.
   - Close socket and exit.
