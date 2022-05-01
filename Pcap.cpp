#include "Pcap.h"
#include "stun_t.h"
#include "rtp_t.h"
#include "rtcp_t.h"
#include "turn_t.h"
#include "nill_t.h" // test variardity
#include "t38_t.h"
#include "mqtt_t.h"
#include <string.h>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>

using namespace stun;
using namespace rtp;
using namespace rtcp;
using namespace turn;
using namespace t38;
using namespace mqtt;



template<>
struct is_validator<T38Rfc>
{
    static constexpr bool value = true;
};
/**
 * @brief if you need your class to be parsed by VParse,
 * please specialize it below
 */
template<>
struct is_validator<RtcpRFC>
{
    static constexpr bool value = true;
};

template<>
struct is_validator<TurnRFC>
{
    static constexpr bool value = true;
};

template<>
struct is_validator<StunRFC>
{
    static constexpr bool value = true;
};

template<>
struct is_validator<RtpRFC>
{
    static constexpr bool value = true;
};

template<>
struct is_validator<MqttRFC>
{
    static constexpr bool value = true;
};


namespace libnetin {
void Pcap::showAll()
{
    static char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t* ifaces;
    int n MAYBEUNUSED = pcap_findalldevs(&ifaces, errbuf);
    for(pcap_if_t* curr = ifaces; curr != nullptr; curr = curr->next) {
        std::cout << curr->name << "\r\n";
        curr = curr->next;
    }
    for(pcap_if_t* curr = ifaces; curr != nullptr; curr = curr->next) {
        pcap_if_t* t = curr;
        curr = curr->next;
        free(t);
    }
    // loop to all devs or capture on a specific one
}


Pcap::Pcap() : m_stop{1}, p_Cap{nullptr},errbuf{0}/* default mode is offline */
{
    memset(&m_options, 0, sizeof(m_options));
    memset(&m_nextRes, 0, sizeof(m_nextRes));
}

Pcap::~Pcap()
{
    if (p_Cap) {
        pcap_close(p_Cap);
    }
}

bool Pcap::offline(const char *fname)
{
    p_Cap = pcap_open_offline(fname, errbuf);
    if (!p_Cap)
        return false;

    return true;
}

bool Pcap::live(const char *dev)
{
    p_Cap = pcap_open_live(dev,PCAP_BUF_SIZE,0,-1,errbuf);
    if (p_Cap) {
        m_options.live = true;
        std::cout << "Opened " << dev << " in live mode" << std::endl;
        return true;
    } else {
        std::cerr << "Error: " << errbuf << std::endl;
        return false;
    }
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
 * @todo: Add a file appender to the threaded loop
 */
void Pcap::loop()
{

    if (m_options.live) {
        std::thread t{[&]() {

                for(Result_t& res =  next(); m_stop.load(); operator++())
                {
                    MAYBEUNUSED auto resultNwork =
                                    VParse(
                                          MqttRFC{res},
                                          T38Rfc{res},
                                          RtcpRFC{res},
                                          TurnRFC{res},
                                          StunRFC{res},
                                          RtpRFC{res});
                }
            }
        };
        t.detach();
        std::cout << "Press Q or q to stop..." << std::endl;
        char q='a';
        std::cin >> q;
        switch (q) {
        case 'q':
        case 'Q':
            m_stop.store(0);
        default:
            break;
        }
    } else {
        for(Result_t& res =  next(); hasNext(); operator++())
        {
            MAYBEUNUSED auto resultNwork =
                VParse(MqttRFC{res},
                      T38Rfc{res},
                      RtcpRFC{res},
                      TurnRFC{res},
                      StunRFC{res},
                      RtpRFC{res});
        }
    }
    std::cout << "Finished capturing... writing out...\r\n";
    std::ofstream jsonfile;
    jsonfile.open ("out.json");
    jsonfile<< serializer.serialize();
    jsonfile.close();
    T38Rfc::dbg();
}

}//libnetin


