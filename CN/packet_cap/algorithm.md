# Algorithm: Layer-2/3 Raw Socket Packet Sniffer and Header Parser

1. **Initialize Raw Socket**:
   - Create a raw packet socket bound to all Layer-2 Ethernet protocols:
     `socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))`
   - Check for superuser / root privileges (raw sockets require `CAP_NET_RAW` or `sudo`).
   - Allocate a packet reception buffer of 65,536 bytes.

2. **Packet Capture Loop**:
   - Loop indefinitely:
     - Read incoming frame from network interface using `recvfrom()`.
     - Capture frame length and pass packet buffer to `process_packet()`.

3. **Layer-2 (Ethernet) Decapsulation**:
   - Cast start of buffer to Ethernet header struct: `struct ethhdr *eth = (struct ethhdr *)buffer`.
   - Check EtherType protocol (`ntohs(eth->h_proto)`):
     - If `ETH_P_IP` ($0x0800$), proceed with IPv4 parsing.
     - Otherwise, ignore non-IP frames (e.g., ARP, IPv6).

4. **Layer-3 (IPv4) Header Parsing**:
   - Locate IPv4 header immediately following the 14-byte Ethernet header:
     `struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr))`.
   - Extract Source IP (`iph->saddr`) and Destination IP (`iph->daddr`) and format with `inet_ntoa()`.
   - Compute IP Header Length: $\text{IHL} = \text{iph->ihl} \times 4$ bytes.
   - Read transport protocol field: `iph->protocol`.

5. **Layer-4 (Transport) Header Parsing**:
   - Locate transport header at offset: $\text{sizeof(ethhdr)} + \text{IHL}$.
   - If `IPPROTO_TCP` ($6$):
     - Cast to `struct tcphdr *tcph`.
     - Extract Source Port (`ntohs(tcph->source)`) and Destination Port (`ntohs(tcph->dest)`).
   - If `IPPROTO_UDP` ($17$):
     - Cast to `struct udphdr *udph`.
     - Extract Source Port (`ntohs(udph->source)`) and Destination Port (`ntohs(udph->dest)`).
   - Print packet metadata (Total bytes, IPs, Transport Protocol, Ports).
