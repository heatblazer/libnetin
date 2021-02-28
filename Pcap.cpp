#include "Pcap.h"
#include "stun_t.h"
#include "rtp_t.h"
#include <string.h>
#include <map>
#include <string>
#include <iostream>
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

Pcap::Result_t& Pcap::next()
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
#if 1
    std::map<AppName, std::string> appnames;
    appnames[AppName::Facebook] = "Facebook";
    appnames[AppName::WhatsApp] = "Whatsapp";
    appnames[AppName::Telegram] = "Telegram";
    appnames[AppName::Zoom] = "Zoom";
    appnames[AppName::GoogleMeet] = "GoogleMeet";
    appnames[AppName::Skype] = "Skype";
    appnames[AppName::Unknown] = "Unknown";
#endif
    size_t totalStun=0;
    std::vector<StunRFC> stunCaps;
    std::vector<RtpRFC> rtpCaps;
    ;
    for(Pcap::Result_t& res =  next(); hasNext(); operator++())
    {
        auto resstun = packetHandlerT<StunRFC, StunRFC>(NULL, res, StunRFC{});
        auto resrtp = packetHandlerT<RtpRFC, RtpRFC>(NULL, res, RtpRFC{});
        if (resrtp.valid()) {
            //TODO:
        }
       // if (resstun.valid()) {
        //    totalStun += resstun.count();

            //AppName n = resstun.app();
            //if (n != AppName::Unknown)
            //    std::cout << appnames[n] << "\r\n";
        //    stunCaps.push_back(resstun);
        //} else if (resrtp.valid()) {

        //}
//        memset(&m_nextRes, 0, sizeof(m_nextRes));
    }
    printf("[%zu total\r\n", totalStun);
}

template<typename T, typename Return_T>
Return_T Pcap::packetHandlerT(u_char *userData, const Result_t &res, T protocol)
{
    (void)userData; //reserved userdata...
    return protocol(res);
}
