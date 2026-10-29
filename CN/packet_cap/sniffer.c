#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

#define BUFFER_SIZE 65536

void process_packet(unsigned char* buffer, int size) {
    // 1. Parse Ethernet Header
    struct ethhdr *eth = (struct ethhdr *)buffer;
    
    // We only care about IP packets (Protocol 0x0800)
    if (ntohs(eth->h_proto) != ETH_P_IP) {
        return;
    }

    // 2. Parse IP Header (Located right after the 14-byte Ethernet header)
    struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    
    struct sockaddr_in source, dest;
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;

    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

    printf("\n==================================================");
    printf("\n[IP Packet Captured | Size: %d bytes]", size);
    printf("\nFrom Source IP : %s", inet_ntoa(source.sin_addr));
    printf("\nTo Dest IP     : %s", inet_ntoa(dest.sin_addr));
    
    // 3. Parse Transport Layer Protocol
    int ip_header_len = iph->ihl * 4; // Length field is in 32-bit words

    if (iph->protocol == IPPROTO_TCP) {
        // Parse TCP Header
        struct tcphdr *tcph = (struct tcphdr *)(buffer + sizeof(struct ethhdr) + ip_header_len);
        printf("\nProtocol       : TCP");
        printf("\nSource Port    : %d", ntohs(tcph->source));
        printf("\nDest Port      : %d", ntohs(tcph->dest));
    } 
    else if (iph->protocol == IPPROTO_UDP) {
        // Parse UDP Header
        struct udphdr *udph = (struct udphdr *)(buffer + sizeof(struct ethhdr) + ip_header_len);
        printf("\nProtocol       : UDP");
        printf("\nSource Port    : %d", ntohs(udph->source));
        printf("\nDest Port      : %d", ntohs(udph->dest));
    } 
    else if (iph->protocol == IPPROTO_ICMP) {
        printf("\nProtocol       : ICMP");
    } 
    else {
        printf("\nProtocol       : Other (Protocol ID: %d)", iph->protocol);
    }
    printf("\n==================================================\n");
}

int main() {
    int raw_socket;
    int packet_size;
    socklen_t saddr_size;
    struct sockaddr saddr;
    unsigned char *buffer = (unsigned char *)malloc(BUFFER_SIZE);

    if (buffer == NULL) {
        perror("Buffer allocation failed");
        return 1;
    }

    // Create a raw socket looking at all incoming IP packets over Ethernet
    raw_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    if (raw_socket < 0) {
        perror("Socket creation failed. Are you running as root/sudo?");
        free(buffer);
        return 1;
    }

    printf("Raw socket sniffer initialized. Listening for IP packets...\n");

    while (1) {
        saddr_size = sizeof(saddr);
        // Receive raw packets from the interface
        packet_size = recvfrom(raw_socket, buffer, BUFFER_SIZE, 0, &saddr, &saddr_size);
        if (packet_size < 0) {
            perror("Failed to read packet data");
            close(raw_socket);
            free(buffer);
            return 1;
        }
        
        // Pass the raw byte array down to extract network details
        process_packet(buffer, packet_size);
    }

    close(raw_socket);
    free(buffer);
    return 0;
}
