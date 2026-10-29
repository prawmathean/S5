# Complete Cisco Packet Tracer Routing Guide
### Detailed Step-by-Step Implementation: Static Routing, RIPv2, and OSPF

---

## Table of Contents
1. [Core Networking Concepts in Plain English](#1-core-networking-concepts-in-plain-english)
2. [Master Network Topology & IP Addressing Plan](#2-master-network-topology--ip-addressing-plan)
3. [Lab Setup & Hardware Configuration](#3-lab-setup--hardware-configuration)
4. [Base Router & PC Configuration](#4-base-router--pc-configuration)
5. [Module 1: Static Routing Implementation](#5-module-1-static-routing-implementation)
6. [Module 2: RIPv2 Dynamic Routing Implementation](#6-module-2-ripv2-dynamic-routing-implementation)
7. [Module 3: OSPF Dynamic Routing Implementation](#7-module-3-ospf-dynamic-routing-implementation)
8. [Side-by-Side Comparison Matrix](#8-side-by-side-comparison-matrix)
9. [Verification, Troubleshooting & Diagnostics](#9-verification-troubleshooting--diagnostics)

---

## 1. Core Networking Concepts in Plain English

### What is a Router?
A router is a specialized computer that connects different local area networks (LANs) together. While a switch only moves traffic inside a single room or building (same IP subnet), a router directs packets across the internet or between different subnets.

### What is Routing?
When your PC (`192.168.1.10`) wants to talk to a web server (`192.168.3.10`), your PC doesn't know where that server physically lives. It sends the packet to its **Default Gateway** (its local router interface). The router looks at its **Routing Table**—a roadmap of known networks—and decides which exit port to forward the packet out of.

There are two primary ways a router learns its roadmap:

### 1. Static Routing
- **What it is**: You manually log into every router and type: *"To reach network X, send the packet out of interface Y to router Z."*
- **Analogy**: Giving someone fixed, handwritten driving directions: *"Turn right on Elm Street, then turn left on 4th Ave."*
- **Pros**: Zero router CPU/memory overhead, highly predictable, secure (no protocol chatter).
- **Cons**: Tedious to maintain. If a link goes down, the router has no backup plan unless you manually configure floating static routes.

### 2. Dynamic Routing: Distance-Vector (RIPv2)
- **What it is**: **Routing Information Protocol version 2**. Routers broadcast their entire routing table to their immediate neighbors every 30 seconds.
- **Metric**: **Hop Count** (number of routers along the path). The path with the fewest routers wins.
- **Analogy**: Asking the next person down the road: *"How many towns away is Springfield?"* They reply: *"It's 2 towns past me."*
- **Limit**: Maximum 15 hops (16 hops means unreachable). Best for small, simple networks.
- **Why Version 2?** RIPv1 was classful and did not send subnet masks. RIPv2 is classless, supports VLSM (Variable Length Subnet Masking), and sends updates via multicast (`224.0.0.9`).

### 3. Dynamic Routing: Link-State (OSPF)
- **What it is**: **Open Shortest Path First**. Every router builds a complete topological map of the entire network using the Dijkstra algorithm (Shortest Path First).
- **Metric**: **Cost**, calculated based on interface **Bandwidth** ($\text{Cost} = \frac{10^8}{\text{Bandwidth in bps}}$). Faster gigabit links have a lower cost than slow serial cables.
- **Analogy**: Google Maps with live traffic data. Instead of just counting towns, it chooses the 8-lane expressway over a single-lane dirt road even if the highway passes through an extra town.
- **Features**: Sub-second convergence, scales to massive enterprise networks, divides networks into Areas (Area 0 is the backbone).

---

## 2. Master Network Topology & IP Addressing Plan

To understand and test all three routing methods, we use a standard **3-Router, 2-PC chain topology**. This setup demonstrates multi-hop forwarding and routing through an intermediate transit router.

```text
[PC0] 
  | (FastEthernet)
[Switch0]
  |
(Gig0/0/0) [Router0] (Gig0/0/1) ------- (Gig0/0/0) [Router1] (Gig0/0/1) ------- (Gig0/0/0) [Router2] (Gig0/0/1)
                                                                                             |
                                                                                         [Switch1]
                                                                                             | (FastEthernet)
                                                                                           [PC1]
```

### IP Subnet Architecture

| Network Segment | Subnet | Subnet Mask | Devices Connected | Interfaces Assigned |
| :--- | :--- | :--- | :--- | :--- |
| **LAN 1** | `192.168.1.0/24` | `255.255.255.0` | PC0, Switch0, Router0 | PC0 (`192.168.1.10`), R0 Gig0/0/0 (`192.168.1.1`) |
| **WAN Link 1** | `10.1.1.0/30` | `255.255.255.252` | Router0, Router1 | R0 Gig0/0/1 (`10.1.1.1`), R1 Gig0/0/0 (`10.1.1.2`) |
| **WAN Link 2** | `10.2.2.0/30` | `255.255.255.252` | Router1, Router2 | R1 Gig0/0/1 (`10.2.2.1`), R2 Gig0/0/0 (`10.2.2.2`) |
| **LAN 2** | `192.168.2.0/24` | `255.255.255.0` | Router2, Switch1, PC1 | R2 Gig0/0/1 (`192.168.2.1`), PC1 (`192.168.2.10`) |

---

## 3. Lab Setup & Hardware Configuration

### Step 1: Place the Devices in Cisco Packet Tracer
1. **Routers**: Add three **Cisco 4321** or **2911** routers. Label them `Router0`, `Router1`, and `Router2`.
2. **Switches**: Add two **2960** switches. Place `Switch0` near Router0 and `Switch1` near Router2.
3. **End Devices**: Add two **Generic PCs**. Place `PC0` on LAN 1 and `PC1` on LAN 2.

### Step 2: Cable the Topology
1. Connect `PC0` (FastEthernet0) to `Switch0` (FastEthernet0/1) using a **Copper Straight-Through** cable.
2. Connect `Switch0` (FastEthernet0/2) to `Router0` (GigabitEthernet0/0/0) using a **Copper Straight-Through** cable.
3. Connect `Router0` (GigabitEthernet0/0/1) to `Router1` (GigabitEthernet0/0/0) using a **Copper Crossover** cable (or Straight-Through on auto-MDIX ports).
4. Connect `Router1` (GigabitEthernet0/0/1) to `Router2` (GigabitEthernet0/0/0) using a **Copper Crossover** cable.
5. Connect `Router2` (GigabitEthernet0/0/1) to `Switch1` (FastEthernet0/1) using a **Copper Straight-Through** cable.
6. Connect `Switch1` (FastEthernet0/2) to `PC1` (FastEthernet0) using a **Copper Straight-Through** cable.

---

## 4. Base Router & PC Configuration

Before configuring any routing protocol, interface IP addresses must be assigned and turned on.

### End Device (PC) Configuration
Click each PC, open the **Desktop** tab, and click **IP Configuration**:

- **PC0 Configuration**:
  - IP Address: `192.168.1.10`
  - Subnet Mask: `255.255.255.0`
  - Default Gateway: `192.168.1.1`

- **PC1 Configuration**:
  - IP Address: `192.168.2.10`
  - Subnet Mask: `255.255.255.0`
  - Default Gateway: `192.168.2.1`

---

### Router Base IP Configuration (CLI Commands)

Click each router, go to the **CLI** tab, press Enter, and apply the following commands:

#### Router0 (R0)
```cisco
Router> enable
Router# configure terminal
Router(config)# hostname R0

! LAN Interface
R0(config)# interface GigabitEthernet0/0/0
R0(config-if)# ip address 192.168.1.1 255.255.255.0
R0(config-if)# no shutdown
R0(config-if)# exit

! WAN Link 1 to R1
R0(config)# interface GigabitEthernet0/0/1
R0(config-if)# ip address 10.1.1.1 255.255.255.252
R0(config-if)# no shutdown
R0(config-if)# exit
```

#### Router1 (R1 - Middle Transit Router)
```cisco
Router> enable
Router# configure terminal
Router(config)# hostname R1

! WAN Link 1 from R0
R1(config)# interface GigabitEthernet0/0/0
R1(config-if)# ip address 10.1.1.2 255.255.255.252
R1(config-if)# no shutdown
R1(config-if)# exit

! WAN Link 2 to R2
R1(config)# interface GigabitEthernet0/0/1
R1(config-if)# ip address 10.2.2.1 255.255.255.252
R1(config-if)# no shutdown
R1(config-if)# exit
```

#### Router2 (R2)
```cisco
Router> enable
Router# configure terminal
Router(config)# hostname R2

! WAN Link 2 from R1
R2(config)# interface GigabitEthernet0/0/0
R2(config-if)# ip address 10.2.2.2 255.255.255.252
R2(config-if)# no shutdown
R2(config-if)# exit

! LAN Interface
R2(config)# interface GigabitEthernet0/0/1
R2(config-if)# ip address 192.168.2.1 255.255.255.0
R2(config-if)# no shutdown
R2(config-if)# exit
```

> **Check**: At this stage, all interface link lights in Packet Tracer should turn from RED to **GREEN**.

---

## 5. Module 1: Static Routing Implementation

### Concept
A static route tells the router:
`ip route <destination-network> <subnet-mask> <next-hop-ip>`
- `R0` only knows `192.168.1.0/24` and `10.1.1.0/30`. It needs routes for `10.2.2.0/30` and `192.168.2.0/24`.
- `R1` only knows `10.1.1.0/30` and `10.2.2.0/30`. It needs routes for `192.168.1.0/24` and `192.168.2.0/24`.
- `R2` only knows `10.2.2.0/30` and `192.168.2.0/24`. It needs routes for `10.1.1.0/30` and `192.168.1.0/24`.

---

### Step-by-Step Commands

#### On Router0 (R0)
```cisco
R0# configure terminal
! Route to reach R1-R2 link
R0(config)# ip route 10.2.2.0 255.255.255.252 10.1.1.2
! Route to reach remote LAN 2
R0(config)# ip route 192.168.2.0 255.255.255.0 10.1.1.2
R0(config)# end
R0# copy running-config startup-config
```

#### On Router1 (R1)
```cisco
R1# configure terminal
! Route to reach LAN 1
R1(config)# ip route 192.168.1.0 255.255.255.0 10.1.1.1
! Route to reach LAN 2
R1(config)# ip route 192.168.2.0 255.255.255.0 10.2.2.2
R1(config)# end
R1# copy running-config startup-config
```

#### On Router2 (R2)
```cisco
R2# configure terminal
! Route to reach R0-R1 link
R2(config)# ip route 10.1.1.0 255.255.255.252 10.2.2.1
! Route to reach remote LAN 1
R2(config)# ip route 192.168.1.0 255.255.255.0 10.2.2.1
R2(config)# end
R2# copy running-config startup-config
```

### Verification & Testing
1. On `PC0`, open Command Prompt and ping `PC1`:
   ```cmd
   ping 192.168.2.10
   ```
   *(The first ping packet may time out due to ARP resolution; the remaining 3 will succeed with `Reply from 192.168.2.10: bytes=32 time<1ms TTL=125`).*

2. On `R0`, check the routing table:
   ```cisco
   R0# show ip route
   ```
   *Look for codes marked with **`S`** (Static Route).*

---

## 6. Module 2: RIPv2 Dynamic Routing Implementation

### Concept
With RIPv2, we stop manually typing destinations. Instead, we tell each router: *"Advertise all subnets that are directly connected to you."* The routers communicate automatically and share routes.

> **Note**: If transitioning from Static Routing to RIPv2, remove static routes using `no ip route ...` on each router first.

---

### Step-by-Step Commands

#### On Router0 (R0)
```cisco
R0# configure terminal
R0(config)# router rip
! Enable Version 2 (classless routing)
R0(config-router)# version 2
! Disable automatic summarization at classful boundaries
R0(config-router)# no auto-summary
! Tell RIP which directly connected networks to advertise
R0(config-router)# network 192.168.1.0
R0(config-router)# network 10.0.0.0
! Prevent RIP updates from flooding into user LAN
R0(config-router)# passive-interface GigabitEthernet0/0/0
R0(config-router)# end
R0# copy running-config startup-config
```

#### On Router1 (R1)
```cisco
R1# configure terminal
R1(config)# router rip
R1(config-router)# version 2
R1(config-router)# no auto-summary
! R1 is connected to both 10.1.1.0 and 10.2.2.0 (class A 10.0.0.0)
R1(config-router)# network 10.0.0.0
R1(config-router)# end
R1# copy running-config startup-config
```

#### On Router2 (R2)
```cisco
R2# configure terminal
R2(config)# router rip
R2(config-router)# version 2
R2(config-router)# no auto-summary
R2(config-router)# network 10.0.0.0
R2(config-router)# network 192.168.2.0
R2(config-router)# passive-interface GigabitEthernet0/0/1
R2(config-router)# end
R2# copy running-config startup-config
```

### Verification & Testing
1. On `R0`, check the routing table:
   ```cisco
   R0# show ip route
   ```
   *Routes marked with **`R`** are dynamically learned via RIP:*
   `R  192.168.2.0/24 [120/2] via 10.1.1.2`
   - `[120/2]` means Administrative Distance = 120, Metric (Hop Count) = 2.
2. Run `debug ip rip` to watch live 30-second multicast routing updates sent to `224.0.0.9`.
3. Ping from `PC0` to `PC1` to confirm end-to-end communication.

---

## 7. Module 3: OSPF Dynamic Routing Implementation

### Concept
OSPF uses:
`network <network-ip> <wildcard-mask> area <area-id>`

#### What is a Wildcard Mask?
A wildcard mask is the inverse of a subnet mask (calculated as `255.255.255.255 - Subnet Mask`):
- For `/24` (`255.255.255.0`) $\rightarrow$ Wildcard: `0.0.0.255`
- For `/30` (`255.255.255.252`) $\rightarrow$ Wildcard: `0.0.0.3`

In single-area OSPF, all routers belong to the backbone area: **`area 0`**.

---

### Step-by-Step Commands

#### On Router0 (R0)
```cisco
R0# configure terminal
! Start OSPF process with ID 1
R0(config)# router ospf 1
! Assign unique Router ID
R0(config-router)# router-id 1.1.1.1
! Advertise LAN 1 in Area 0
R0(config-router)# network 192.168.1.0 0.0.0.255 area 0
! Advertise WAN Link 1 in Area 0
R0(config-router)# network 10.1.1.0 0.0.0.3 area 0
! Don't send OSPF Hello packets to user computers
R0(config-router)# passive-interface GigabitEthernet0/0/0
R0(config-router)# end
R0# copy running-config startup-config
```

#### On Router1 (R1)
```cisco
R1# configure terminal
R1(config)# router ospf 1
R1(config-router)# router-id 2.2.2.2
! Advertise WAN Link 1
R1(config-router)# network 10.1.1.0 0.0.0.3 area 0
! Advertise WAN Link 2
R1(config-router)# network 10.2.2.0 0.0.0.3 area 0
R1(config-router)# end
R1# copy running-config startup-config
```
*(Notice console message: `%OSPF-5-ADJCHANGE: Process 1, Nbr 1.1.1.1 on GigabitEthernet0/0/0 from LOADING to FULL, Done` — OSPF adjacency is formed!)*

#### On Router2 (R2)
```cisco
R2# configure terminal
R2(config)# router ospf 1
R2(config-router)# router-id 3.3.3.3
! Advertise WAN Link 2
R2(config-router)# network 10.2.2.0 0.0.0.3 area 0
! Advertise LAN 2
R2(config-router)# network 192.168.2.0 0.0.0.255 area 0
R2(config-router)# passive-interface GigabitEthernet0/0/1
R2(config-router)# end
R2# copy running-config startup-config
```

### Verification & Testing
1. On `R1`, inspect established OSPF neighbors:
   ```cisco
   R1# show ip ospf neighbor
   ```
   *State should read `FULL/BDR` or `FULL/DR`.*
2. Check routing table on `R0`:
   ```cisco
   R0# show ip route
   ```
   *Routes marked with **`O`** are OSPF routes:*
   `O  192.168.2.0/24 [110/3] via 10.1.1.2`
   - `[110/3]` means Administrative Distance = 110, Cost Metric = 3.
3. Test connectivity: Ping from `PC0` to `PC1` (`192.168.2.10`) with `ping` or `tracert`.

---

## 8. Side-by-Side Comparison Matrix

| Property | Static Routing | RIPv2 | OSPFv2 |
| :--- | :--- | :--- | :--- |
| **Protocol Type** | Manual | Distance-Vector (Interior) | Link-State (Interior) |
| **Algorithm** | None | Bellman-Ford | Dijkstra (Shortest Path First) |
| **Metric** | None (manual) | Hop Count (Max 15) | Cost ($\propto \frac{1}{\text{Bandwidth}}$) |
| **Administrative Distance** | **1** | **120** | **110** |
| **Convergence Speed** | None (instant if static) | Slow (30-second cycles) | Very Fast (event-triggered LSAs) |
| **Subnet Support** | Classless (CIDR/VLSM) | Classless (VLSM) | Classless (VLSM & CIDR) |
| **Resource Usage** | Zero CPU/RAM | Very Low | Moderate to High (topological map) |
| **Network Scalability** | Small networks | Small networks (< 15 routers) | Large enterprise / ISP networks |

---

## 9. Verification, Troubleshooting & Diagnostics

### Key Cisco IOS Diagnostic Commands

| Command | Purpose |
| :--- | :--- |
| `show ip interface brief` | View status (`up/up` or `administratively down`) and IP of all ports |
| `show ip route` | View the active routing table |
| `show ip protocols` | View active dynamic routing processes (RIP/OSPF timers, networks, neighbors) |
| `show ip ospf neighbor` | Verify bidirectional OSPF neighbor adjacencies |
| `show ip ospf interface` | View OSPF cost, priority, area, and Hello/Dead timer intervals |
| `traceroute <ip>` | Trace the exact path/routers a packet traverses to reach the destination |
| `debug ip rip` | View live RIP advertisement broadcasts in real-time |
| `undebug all` | Turn off all active debugging |

### Common Troubleshooting Scenarios
1. **Link light is RED**:
   - Check if the interface was turned on with `no shutdown`.
   - Check cable type (use Straight-Through for PC-to-Switch, Crossover for Router-to-Router).
2. **Ping times out between PCs**:
   - Verify Default Gateway IP on PC matches the connected router's IP.
   - Run `show ip route` on both end routers to ensure routes exist in **both directions** (packets need a return path).
3. **OSPF neighbor not forming**:
   - Verify both routers are in the same OSPF Area (`area 0`).
   - Check that Hello and Dead timers match on connected interfaces.
   - Confirm subnet masks match on point-to-point links.
