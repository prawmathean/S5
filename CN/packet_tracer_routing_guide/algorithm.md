# Algorithms: Static, RIPv2, and OSPF Routing Configurations in Cisco Packet Tracer

This document outlines the concise step-by-step algorithms for configuring and testing Static Routing, RIPv2, and OSPF on Cisco routers.

---

## Algorithm 1: Static Route Configuration

1. **Identify Unreachable Networks**:
   - For router $R_i$, determine all remote destination network IDs $N_{dest}$, their subnet masks $M_{dest}$, and the immediate next-hop router interface IP $IP_{next}$.

2. **Access Privileged Configuration Mode**:
   ```cisco
   enable
   configure terminal
   ```

3. **Inject Static Route**:
   - Execute command:
     `ip route <N_dest> <M_dest> <IP_next>`
   - Repeat for all remote subnets.

4. **Ensure Bidirectional Routing**:
   - Configure corresponding return static routes on remote routers so response packets have a valid path back.

5. **Verify**:
   - Verify route injection with `show ip route` (routes flagged with code `S`).
   - Validate end-to-end reachability with ICMP `ping` between hosts.

---

## Algorithm 2: RIPv2 Dynamic Routing Configuration

1. **Activate RIP Process**:
   ```cisco
   enable
   configure terminal
   router rip
   ```

2. **Enable Version 2 & Classless Operation**:
   - Force RIP version 2: `version 2`
   - Disable automatic classful summarization: `no auto-summary`

3. **Advertise Directly Connected Networks**:
   - For every subnet directly connected to router $R_i$, execute:
     `network <major-network-ip>`

4. **Suppress Unnecessary Broadcasts (Passive Interfaces)**:
   - Mark LAN interfaces (ports connecting to user PCs/switches) as passive:
     `passive-interface <interface-id>`
   - This prevents security exposure and LAN bandwidth consumption.

5. **Verify Convergence**:
   - Wait 30 seconds for RIP advertisement cycles.
   - Run `show ip route` (dynamically learned routes appear with code `R`).
   - Check protocol metrics with `show ip protocols`.

---

## Algorithm 3: Single-Area OSPF Dynamic Routing Configuration

1. **Activate OSPF Process**:
   - Enable OSPF with a local Process ID:
     `router ospf <process-id>` (e.g., `router ospf 1`).

2. **Set Router ID**:
   - Assign unique 32-bit identifier:
     `router-id <A.B.C.D>` (e.g., `1.1.1.1`).

3. **Compute Wildcard Masks**:
   - For each directly connected subnet, calculate:
     $$\text{Wildcard Mask} = 255.255.255.255 - \text{Subnet Mask}$$

4. **Advertise Interfaces into Area 0**:
   - For every active interface subnet, bind to Backbone Area:
     `network <subnet-ip> <wildcard-mask> area 0`

5. **Configure Passive Interfaces**:
   - Block Hello packets on user-facing LAN ports:
     `passive-interface <interface-id>`

6. **Verify Adjacencies & Shortest Path Tree**:
   - Check neighbor adjacency states: `show ip ospf neighbor` (states must reach `FULL`).
   - Check OSPF routing table entries: `show ip route` (routes flagged with code `O`).
   - Confirm end-to-end low-latency ICMP `ping` across hosts.
