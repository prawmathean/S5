# Algorithm: Matrix Classification Server using TCP Sockets

## Server Algorithm
1. **Initialize & Listen**:
   - Create a TCP stream socket (`socket(AF_INET, SOCK_STREAM, 0)`).
   - Enable address reuse (`SO_REUSEADDR`) and bind to Port 8080.
   - Seed the pseudo-random number generator (`srand(time(NULL))`).
   - Listen for incoming connections (`listen()`).

2. **Handle Client Request**:
   - Accept connection from client (`accept()`).
   - Receive integer matrix dimension $N$ from client (`recv()`).
   - Randomly generate an $N \times N$ matrix by choosing between three patterns:
     - *Diagonal Matrix*: Non-zero values only along main diagonal ($i = j$).
     - *Upper Triangular*: Zeroes strictly below the diagonal ($i > j$).
     - *Lower Triangular*: Zeroes strictly above the diagonal ($i < j$).
   - Print the generated matrix to server console.

3. **Classification & Response**:
   - Check matrix properties:
     - Set flags `is_upper = 1` and `is_lower = 1`.
     - If any element below diagonal ($i > j$) is non-zero, clear `is_upper = 0`.
     - If any element above diagonal ($i < j$) is non-zero, clear `is_lower = 0`.
     - If both flags remain set $\rightarrow$ "Diagonal Matrix".
     - If only `is_upper` $\rightarrow$ "Upper Triangular Matrix".
     - If only `is_lower` $\rightarrow$ "Lower Triangular Matrix".
     - Otherwise $\rightarrow$ "Normal/Ordinary Matrix".
   - Send classification result string to client (`send()`).
   - Close sockets and exit.

---

## Client Algorithm
1. **Input & Connect**:
   - Prompt user to enter matrix dimension $N$.
   - Create a TCP stream socket (`socket(AF_INET, SOCK_STREAM, 0)`).
   - Configure server address (`127.0.0.1`, Port 8080).
   - Connect to the server (`connect()`).

2. **Communicate & Display**:
   - Send integer $N$ to the server (`send()`).
   - Read server's classification string (`read()`).
   - Print classification result to standard output.
   - Close socket and terminate.
