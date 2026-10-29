# UDP Internet Slang Translator

## What is this experiment all about?
Unlike TCP, which establishes a formal 3-way handshake connection before transmitting data, **UDP (User Datagram Protocol)** is **connectionless**. You simply bundle your message into a packet (datagram) and shoot it across the network with `sendto()`.

This experiment demonstrates client-server communication using UDP:
- The client sends an informal chat message packed with modern abbreviations/slang (e.g., *"tbh idk if I can come asap"*).
- The UDP server intercepts the datagram, scans the sentence against an internal slang dictionary, expands each abbreviation into formal English, and fires the translated sentence back to the client via `sendto()`.

## What is the code actually doing?
1. **Server (`server.c`)**:
   - Opens a UDP datagram socket (`SOCK_DGRAM`) on port `8080`.
   - Maintains a lookup dictionary of internet terms:
     - `tbh` $\rightarrow$ "to be honest"
     - `idk` $\rightarrow$ "I do not know"
     - `asap` $\rightarrow$ "as soon as possible"
     - `irl` $\rightarrow$ "in real life"
     - `omg` $\rightarrow$ "oh my god"
     - `ttyl` $\rightarrow$ "talk to you later"
     - `nvm` $\rightarrow$ "never mind"
   - Receives sentences using `recvfrom()`, parsing words while cleanly preserving punctuation.
   - Reconstructs the translated sentence and sends it back to the client's IP and port.
2. **Client (`client.c`)**:
   - Reads an informal slang sentence from the user.
   - Sends it in a single UDP datagram using `sendto()`.
   - Waits for the server's datagram response with `recvfrom()` and prints the formal English output.

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
   *Example input*: `idk tbh, ttyl asap!`
   *Example output*: `I do not know to be honest, talk to you later as soon as possible!`
