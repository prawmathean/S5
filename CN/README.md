# Computer Networks Laboratory (Semester 5)

This repository contains the C socket programming experiments and Cisco Packet Tracer routing configurations for the Computer Networks Laboratory course.

---

## Repository Structure

```text
CN/
├── conc_time_server/
│   ├── server.c                         # Multi-client concurrent TCP file server using fork()
│   ├── client.c                         # Client requesting file from server
│   ├── algorithm.md                     # Step-by-step socket & concurrency algorithm
│   └── README.md                        # Plain-English explanation, compilation, and usage
│
├── tcp_matrix/
│   ├── server.c                         # TCP server generating & classifying NxN matrices
│   ├── client.c                         # Client sending matrix dimension N
│   ├── algorithm.md                     # Matrix classification & socket algorithm
│   └── README.md                        # High-level overview, matrix types, and commands
│
├── slang_explain/
│   ├── server.c                         # Connectionless UDP internet slang translation server
│   ├── client.c                         # Client sending informal slang sentences
│   ├── algorithm.md                     # UDP datagram algorithm with tokenizer & dictionary
│   └── README.md                        # High-level overview & translation examples
│
├── packet_cap/
│   ├── sniffer.c                        # Layer 2/3/4 Raw Socket network packet sniffer
│   ├── algorithm.md                     # Frame decoding & header parsing algorithm
│   └── README.md                        # Explanation of raw sockets & packet dissection
│
└── packet_tracer_routing_guide/
    ├── README.md                        # In-depth Packet Tracer guide: Static, RIPv2, OSPF
    └── algorithm.md                     # Step-by-step algorithms for all three routing methods
```

---

## Socket Programming Experiments Summary

| Folder | Protocol | Architecture | Concept & Key Mechanism |
| :--- | :--- | :--- | :--- |
| [`conc_time_server`](file:///home/prawmathean/playground/S5/CN/conc_time_server) | TCP (`SOCK_STREAM`) | Multi-process (`fork()`) | Handles multiple simultaneous clients via child processes; streams requested files in binary chunks; avoids zombies with `SIGCHLD`. |
| [`tcp_matrix`](file:///home/prawmathean/playground/S5/CN/tcp_matrix) | TCP (`SOCK_STREAM`) | Iterative Client-Server | Client requests an $N \times N$ matrix; server generates and mathematically classifies it as Diagonal, Upper, Lower Triangular, or Ordinary. |
| [`slang_explain`](file:///home/prawmathean/playground/S5/CN/slang_explain) | UDP (`SOCK_DGRAM`) | Connectionless Datagram | Transmits chat messages via `sendto()`/`recvfrom()`; server parses slang tokens (`tbh`, `idk`, `asap`) and returns formal English. |
| [`packet_cap`](file:///home/prawmathean/playground/S5/CN/packet_cap) | Raw Socket (`SOCK_RAW`) | Packet Inspection | Captures raw Ethernet frames off network interface; decapsulates Ethernet, IPv4, TCP, and UDP headers. |

---

## Cisco Packet Tracer Routing Guide Summary

The [`packet_tracer_routing_guide`](file:///home/prawmathean/playground/S5/CN/packet_tracer_routing_guide) directory contains an exhaustive, step-by-step lab guide covering:
1. **Network Topology & IP Plan**: Complete 3-Router, 2-PC topology with subnet allocations.
2. **Static Routing**: Configuring manual next-hop paths (`ip route <dest> <mask> <next-hop>`).
3. **RIPv2 Routing**: Distance-vector routing with hop count metrics, `version 2`, `no auto-summary`, and `passive-interface`.
4. **OSPF Routing**: Link-state routing using Dijkstra SPF, wildcard masks, `router-id`, and `area 0`.
5. **Comparison & Diagnostics**: Administrative distance tables, convergence comparisons, and Cisco IOS verification commands (`show ip route`, `show ip ospf neighbor`, `traceroute`).

See [`CN/packet_tracer_routing_guide/README.md`](file:///home/prawmathean/playground/S5/CN/packet_tracer_routing_guide/README.md) for full details.
