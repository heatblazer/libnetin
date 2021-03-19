#include "Pcap.h"
#include "stun_t.h"
#include "rtp_t.h"
#include "turn_t.h"
#include "nill_t.h" // test variardity
#include <string.h>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

using namespace stun;
using namespace rtp;
using namespace turn;

Pcap::Pcap() : m_offline{true} /* default mode is offline */
{
    memset(&m_nextRes, 0, sizeof(m_nextRes));
}

Pcap::~Pcap()
{
}

bool Pcap::init(const char *fname)
{
    if (m_offline) {
        p_Cap = pcap_open_offline(fname, errbuf);
    } else {
        pcap_if_t* ifaces;
        int n = pcap_findalldevs(&ifaces, errbuf);
        // loop to all devs or capture on a specific one
        p_Cap = pcap_open_live(ifaces[0].name,PCAP_BUF_SIZE,0,-1,errbuf);
    }
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
        auto resultNwork = VParse(TurnRFC{res},
                                  StunRFC{res},
                                  RtpRFC{res});
        (void)resultNwork;//do something if needed
    }
    std::ofstream jsonfile;
    jsonfile.open ("out.json");
    jsonfile<< serializer.serialize();
    jsonfile.close();
}






