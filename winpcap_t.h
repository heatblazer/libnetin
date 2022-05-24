//this is a helper WIN32 header only
#ifndef WINPCAP_T_H
#define WINPCAP_T_H
#ifndef __unix__
#include <winsock2.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#include <errno.h>
#include <cstdio>
#include <cstring>
#include <cstdint>

#ifndef ETHERTYPE_IP
    #define ETHERTYPE_IP 0x0800
#endif

//typedef uint32_t in_addr_t;
//struct in_addr
//{
 //   in_addr_t s_addr;
//};

#pragma pack(1)
struct ether_header
{
    uint8_t ether_dhost[6];
    uint8_t ether_shost[6];
    uint16_t ether_type;
};// __attribute__((__packed__)); //if gcc
//Ip header (v4)

#pragma pack(1)
struct ip
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ip_hl:4;		/* header length */
    unsigned int ip_v:4;		/* version */
#else

    unsigned int ip_v:4;		/* version */
    unsigned int ip_hl:4;		/* header length */
#endif
//    uint8_t ip_tos;			/* commented out for Win32*/
    unsigned short ip_len;		/* total length */
    unsigned short ip_id;		/* identification */
//    unsigned short ip_off;		/* Commented out for Win32 */
#define IP_RF 0x8000
#define IP_DF 0x4000
#define IP_MF 0x2000
#define IP_OFFMASK 0x1fff
    uint8_t ip_ttl;
    uint8_t ip_p;
    unsigned short ip_sum;
    struct in_addr ip_src, ip_dst;
};

//UDP header
#pragma pack(1)
struct udphdr
{
    union {
        struct {
            uint16_t uh_sport;
            uint16_t ud_dport;
            uint16_t uh_ulen;
            uint16_t uh_sum;
        };
        struct {
            uint16_t source;
            uint16_t dest;
            uint16_t len;
            uint16_t check;
        };
    };
};

// TCP header
#pragma pack(1)
struct tcphdr
{
    unsigned short source_port; // source port
    unsigned short dest_port; // destination port
    unsigned int sequence; // sequence number - 32 bits
    unsigned int acknowledge; // acknowledgement number - 32 bits

    unsigned char ns :1; //Nonce Sum Flag Added in RFC 3540.
    unsigned char reserved_part1:3; //according to rfc
    unsigned char data_offset:4; /*The number of 32-bit words in the TCP header.
    This indicates where the data begins.
    The length of the TCP header is always a multiple
    of 32 bits.*/

    unsigned char fin :1; //Finish Flag
    unsigned char syn :1; //Synchronise Flag
    unsigned char rst :1; //Reset Flag
    unsigned char psh :1; //Push Flag
    unsigned char ack :1; //Acknowledgement Flag
    unsigned char urg :1; //Urgent Flag

    unsigned char ecn :1; //ECN-Echo Flag
    unsigned char cwr :1; //Congestion Window Reduced Flag

    ////////////////////////////////

    unsigned short window; // window
    unsigned short checksum; // checksum
    unsigned short urgent_pointer; // urgent pointer
};

#pragma pack(1)
struct icmp_hdr
{
    uint8_t type; // ICMP Error type
    uint8_t code; // Type sub code
    uint16_t checksum;
    uint16_t id;
    uint16_t seq;
};
// Restore the byte boundary back to the previous value

#ifdef __cplusplus
extern "C" {
#endif

const char *inet_ntop(int af, const void *a0, char *s, socklen_t l);

#ifdef __cplusplus
}
#endif

#endif

#endif // WINPCAP_T_H
