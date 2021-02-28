#ifndef PARSE_H
#define PARSE_H

#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

template <typename T>
struct Parse_T
{
    void parse(T data)
    {
        const struct ether_header* ethernetHeader;
        const struct ip* ipHeader;
        const struct udphdr* udpHeader;
        char sourceIP[INET_ADDRSTRLEN];
        char destIP[INET_ADDRSTRLEN];
        if (!res.data)
            return *this;
        ethernetHeader = (struct ether_header*)res.data;
        if (ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP) {
            ipHeader = (struct ip*)(res.data + sizeof(struct ether_header));
            inet_ntop(AF_INET, &(ipHeader->ip_src), sourceIP, INET_ADDRSTRLEN);
            inet_ntop(AF_INET, &(ipHeader->ip_dst), destIP, INET_ADDRSTRLEN);
        }
    }
};

#endif // PARSE_H
