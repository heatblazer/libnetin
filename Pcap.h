#ifndef PCAP_H
#define PCAP_H
#include "defs.h"

#include <pcap/pcap.h>

class Pcap
{
public:
    struct Result_t
    {
        const u_char* data;
        struct pcap_pkthdr out;
    };

    Pcap();

    ~Pcap();

    bool init(const char* fname);

    void operator++();

    Result_t& next();

    bool hasNext() const;

    void loop();

protected:

    template<typename T, typename Return_T>
    Return_T packetHandlerT(u_char *userData, const Result_t& res, T protocol);

    pcap_t* p_Cap;

    char errbuf[PCAP_ERRBUF_SIZE];

    Result_t m_nextRes;
};

#endif // PCAP_H
