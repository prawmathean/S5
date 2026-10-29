# Algorithm: Internet Slang Translator using Connectionless UDP Sockets

## Server Algorithm
1. **Initialize Dictionary & Socket**:
   - Define a static lookup table pairing internet abbreviations with their formal English expansions:
     `tbh` $\rightarrow$ "to be honest", `asap` $\rightarrow$ "as soon as possible", `idk` $\rightarrow$ "I do not know", etc.
   - Create a connectionless datagram socket (`socket(AF_INET, SOCK_DGRAM, 0)`).
   - Configure server address (`AF_INET`, `INADDR_ANY`, Port 8080) and bind using `bind()`.

2. **Receive & Tokenize**:
   - Loop continuously awaiting datagrams via `recvfrom()`, capturing sender's `sockaddr_in`.
   - Tokenize the received sentence word-by-word using delimiter characters (spaces, punctuation).

3. **Translate**:
   - For each extracted word:
     - Perform case-insensitive comparison against dictionary abbreviations.
     - If matched, append the formal expansion to an output buffer.
     - If not matched, append the original word as-is.
     - Preserve trailing punctuation (commas, periods, exclamation marks).

4. **Respond**:
   - Send the constructed formal sentence back to the client using `sendto()`.

---

## Client Algorithm
1. **Initialize Socket & Address**:
   - Create a UDP datagram socket (`socket(AF_INET, SOCK_DGRAM, 0)`).
   - Configure target server IP (`127.0.0.1`) and Port 8080.

2. **Input & Transmit**:
   - Prompt user to input an informal sentence containing slang (e.g., *"idk tbh I will call you asap"*).
   - Send datagram directly to server using `sendto()` without connection handshake.

3. **Receive & Display**:
   - Await response datagram from server using `recvfrom()`.
   - Null-terminate the buffer and display the translated formal sentence.
   - Close socket and exit.
