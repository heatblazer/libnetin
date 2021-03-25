#ifndef DEFS_H
#define DEFS_H

    #define PCAP_BUF_SIZE	1024
    #define PCAP_SRC_FILE	2
    #define SWAP4(x) (((x) << 24) & 0xFF000000) | (((x) >> 24) & 0x000000FF) | (((x) << 8) & 0x00FF0000) | (((x) >> 8) & 0x0000FF00)
    #define SWAP2(x) ((x) << 8) | ((x) >> 8)

    #ifdef __unix__
        #include <pcap/pcap.h>
        #include <netinet/in.h>
        #include <netinet/ip.h>
        #include <net/if.h>
        #include <netinet/if_ether.h>
        #include <net/ethernet.h>
        #include <netinet/tcp.h>
        #include <netinet/udp.h>
        #include <arpa/inet.h>
    #else
        #include "winpcap_t.h"
        #include <pcap/pcap.h>
    #endif

#endif
