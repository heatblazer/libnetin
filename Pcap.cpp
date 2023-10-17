#include "Pcap.h"
#include "stun_t.h"
#include "rtp_t.h"
#include "rtcp_t.h"
#include "turn_t.h"
#include "t38_t.h"
#include "mqtt_t.h"
#include "rtsp_t.h"
#include "websocket_t.h"
#include "custom_t.h"

#include <string.h>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
#ifdef WIN32
#else
    #include <dlfcn.h>
#endif
using namespace stun;
using namespace rtp;
using namespace rtcp;
using namespace turn;
using namespace t38;
using namespace mqtt;
using namespace websocket;
using namespace rtsp;
using namespace custom;



/**
 * @brief if you need your class to be parsed by VParse,
 * please specialize it below
 */
template<>
struct is_validator<T38Rfc>
{
    static constexpr bool value = true;
};

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

template<>
struct is_validator<WebSocketRFC>
{
    static constexpr bool value = true;
};

template<>
struct is_validator<RtspRFC>
{
    static constexpr bool value = true;
};

template<>
struct is_validator<CustomRFC>
{
    static constexpr bool value = true;
};



namespace libnetin {

void Pcap::showAll()
{
    static char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t* ifaces=NULL;
    int n MAYBEUNUSED = pcap_findalldevs(&ifaces, errbuf);
    for(pcap_if_t* curr = ifaces; curr ; curr = curr->next) {
        std::cout << curr->name << "\r\n";
    }


//TODO: check how to free the devices 
#if 0 //leak for now
//    pcap_freealldevs(ifaces);


    for(pcap_if_t* curr = ifaces; curr ; ) {
        pcap_if_t* t = curr;
        curr = curr->next;
        if (t) {
            free(t);
        }

    }
    // loop to all devs or capture on a specific one
#endif
}


Pcap::Pcap() : m_stop{1}, p_Cap{nullptr},errbuf{0}/* default mode is offline */
{

    memset(&m_options, 0, sizeof(m_options));
    memset(&m_nextRes, 0, sizeof(m_nextRes));

//    if (!Pcap::m_pcapLoader.LoadAPcap("/home/ilian/storage/Builds/build-libnetin-Desktop_Qt_6_3_0_GCC_64bit-Debug/libpcap.a"))
//        throw std::exception{};
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
    if (!p_Cap) {
        fprintf(stderr, "[ERROR]%s", errbuf);
        return false;
    }

    return true;
}
// \Device\NPF_{8C83AA62-C8C8-42C4-9302-1EDA68EA3F5B}
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
    std::ofstream jsonfile;
    jsonfile.open ("out.json");

    if (m_options.live) {
        std::thread t{[&]() {
            jsonfile << serializer.beginSerialize();
            for(Result_t& res =  next(); m_stop.load(); operator++())
            {                
                MAYBEUNUSED auto resultNwork =                    
                                VParse(RtspRFC{res},
                                      MqttRFC{res},
                                      T38Rfc{res},
                                      RtcpRFC{res},
                                      TurnRFC{res},
                                      StunRFC{res},
                                      RtpRFC{res},
                                      WebSocketRFC{res},
                                     CustomRFC{res});
                jsonfile << serializer.serialzeNow();
            }
        }};
        t.detach();
        
        std::cout << "Press Q or q to stop..." << std::endl;
        char q='a';
        std::cin >> q;
        switch (q) {
        case 'q':
        case 'Q': {
            //win test - \Device\NPF_{8C83AA62-C8C8-42C4-9302-1EDA68EA3F5B}
            m_stop.store(0);
            jsonfile << serializer.endSerialize();
            jsonfile.close();
            break;
        }
        default:
            break;

        }
    } else {
        for(Result_t& res =  next(); hasNext(); operator++())
        {
            MAYBEUNUSED auto resultNwork =
                VParse(WebSocketRFC{res},
                      MqttRFC{res},
                      T38Rfc{res},
                      RtcpRFC{res},
                      TurnRFC{res},
                      StunRFC{res},
                      RtpRFC{res},
                    RtspRFC{res},
                    CustomRFC{res});
        }
        jsonfile << serializer.serialize();
        jsonfile.close();
    }
    std::cout << "Finished capturing... writing out...\r\n";

}

}//libnetin


