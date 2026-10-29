# Raw Socket Network Packet Sniffer

## What is this experiment all about?
When you use a standard internet browser, the operating system kernel handles all the low-level network details (Ethernet MAC addresses, IP headers, TCP sequence numbers) and only gives your application the final data.

A **Packet Sniffer** (like Wireshark or `tcpdump`) operates at the bottom of the network stack using **Raw Sockets** (`AF_PACKET` / `SOCK_RAW`). Instead of letting the OS discard packet headers, a raw socket captures the raw, unfiltered binary frames right off the network card as they fly across the wire.

This experiment builds a packet sniffer from scratch in C that decodes:
- **Layer 2 (Data Link)**: Ethernet frame header & MAC EtherType.
- **Layer 3 (Network)**: IPv4 packet header (Source IP, Destination IP, header length).
- **Layer 4 (Transport)**: TCP and UDP segments (Source and Destination port numbers).

## What is the code actually doing?
1. **Creates a Raw Socket**:
   - Calls `socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))`.
   - Requires `sudo` / root permissions because raw sockets can inspect all incoming and outgoing network traffic.
2. **Captures frames**:
   - Calls `recvfrom()` to pull entire Ethernet frames into a 64 KB buffer.
3. **Decodes Layer-by-Layer**:
   - Reads the 14-byte Ethernet header (`struct ethhdr`). Filters for IPv4 traffic (`ETH_P_IP`).
   - Jumps to the IP header (`struct iphdr`). Extracts and formats the sender and receiver IP addresses.
   - Calculates dynamic IP header length ($\text{ihl} \times 4$ bytes) to find where the transport header starts.
   - If TCP, parses `struct tcphdr` to print Source and Destination port numbers (e.g., Port 80 for HTTP, 443 for HTTPS).
   - If UDP, parses `struct udphdr` to print UDP ports (e.g., Port 53 for DNS).

## How to compile and run
1. Compile with `gcc`:
   ```bash
   gcc -Wall -Wextra sniffer.c -o sniffer
   ```
2. Run with root privileges (raw sockets require superuser access):
   ```bash
   sudo ./sniffer
   ```
3. Generate traffic in another terminal (e.g. `ping 8.8.8.8` or `curl https://google.com`), and watch the sniffer dissect live packets on your screen in real time!
