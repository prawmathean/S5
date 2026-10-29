# TCP Matrix Classification Server

## What is this experiment all about?
In computer networking, client-server applications often exchange structured binary data (numbers, objects, arrays) rather than plain text. 

In this experiment:
- A client connects to a server over a reliable **TCP connection** and asks for an $N \times N$ matrix.
- The server dynamically creates a matrix of size $N$, randomly shaping it into a **Diagonal**, **Upper Triangular**, or **Lower Triangular** matrix.
- The server runs a mathematical classification algorithm to inspect the matrix structure and replies with its geometric classification.

This demonstrates sending raw primitive data types (integers) across network byte streams and receiving parsed responses.

## What is the code actually doing?
1. **Client (`client.c`)**:
   - Takes integer dimension $N$ from user input (e.g., $3$ or $4$).
   - Establishes a TCP connection to `127.0.0.1:8080`.
   - Sends the integer $N$ using binary `send()`.
   - Reads the server's analysis text and prints it to the screen.
2. **Server (`server.c`)**:
   - Binds and listens on port 8080.
   - Accepts the client connection and receives the integer $N$.
   - Dynamically fills an $N \times N$ matrix with random values.
   - Evaluates:
     - Is it **Diagonal**? (All non-zero elements lie strictly on the main diagonal $i = j$).
     - Is it **Upper Triangular**? (All values below the diagonal $i > j$ are zero).
     - Is it **Lower Triangular**? (All values above the diagonal $i < j$ are zero).
   - Transmits the result string back to the client.

## How to compile and run
1. Compile both files:
   ```bash
   gcc -Wall -Wextra server.c -o server
   gcc -Wall -Wextra client.c -o client
   ```
2. Start the server:
   ```bash
   ./server
   ```
3. In another terminal, run the client:
   ```bash
   ./client
   ```
   *Example input*: Enter `3` to request a $3 \times 3$ matrix analysis.
