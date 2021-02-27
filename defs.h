#ifndef DEFS_H
#define DEFS_H
#define PCAP_BUF_SIZE	1024
#define PCAP_SRC_FILE	2
#define SWAP4(x) (((x) << 24) & 0xFF000000) | (((x) >> 24) & 0x000000FF) | (((x) << 8) & 0x00FF0000) | (((x) >> 8) & 0x0000FF00)
#define SWAP2(x) ((x) << 8) | ((x) >> 8)

#endif // DEFS_H
