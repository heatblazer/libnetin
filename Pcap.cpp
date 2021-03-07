#include "Pcap.h"
#include "stun_t.h"
#include "rtp_t.h"
#include <string.h>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

using namespace stun;
using namespace rtp;

Pcap::Pcap()
{
    memset(&m_nextRes, 0, sizeof(m_nextRes));
}

Pcap::~Pcap()
{
}

bool Pcap::init(const char *fname)
{
    p_Cap = pcap_open_offline(fname, errbuf);
    if (!p_Cap) return false;
    return true;
}

void Pcap::operator ++()
{
    m_nextRes.data = pcap_next(p_Cap, &m_nextRes.out);
}

Result_t& Pcap::next()
{
    operator++();
    return m_nextRes;
}

bool Pcap::hasNext() const
{
    return  m_nextRes.data != nullptr;
}

/**
 * @brief Pcap::loop - example loop of packets...
 */
void Pcap::loop()
{
    for(Result_t& res =  next(); hasNext(); operator++())
    {
        auto resultNwork = VParse(StunRFC{res},
                                  RtpRFC{res});
        (void)resultNwork;
    }
    std::ofstream jsonfile;
    jsonfile.open ("out.json");
    jsonfile<< serializer.serialize();
    jsonfile.close();
}






