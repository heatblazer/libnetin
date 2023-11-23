#ifndef PCAP_H
#define PCAP_H
#include "defs.h"
#include "types.h"
#include "tjson.hpp"
#include <memory>
#include <pcap/pcap.h>
#include <atomic>
#ifdef __unix__
#include <dlfcn.h>
#endif

/**
 * @brief The is_validator specialize that template for all supported types
 * in VParse(...)
 */
template<typename T>
struct is_validator
{
    static constexpr bool value = false;
};

namespace libnetin {

struct LinuxLoadPcap
{
    void* m_Handle;

    struct {
        int	(*pcap_findalldevs)(pcap_if_t**, char *);
        pcap_t	*(*pcap_open_offline)(const char *, char *);
        pcap_t	*(*pcap_open_live)(const char *, int, int, int, char *);
        const u_char *(*pcap_next)(pcap_t *, struct pcap_pkthdr *);
        void	(*pcap_close)(pcap_t *);

    } fn;


    bool LoadAPcap(const char* fname) {
#ifdef WIN32
#else
        m_Handle = dlopen(fname, RTLD_LAZY);
        if (m_Handle) {

            *(void**)fn.pcap_findalldevs = dlsym(m_Handle, "pcap_findalldevs");
            *(void**)fn.pcap_next = dlsym(m_Handle, "pcap_next");
            *(void**) fn.pcap_open_live = dlsym(m_Handle, "pcap_open_live");
            *(void**) fn.pcap_open_offline = dlsym(m_Handle, "pcap_open_offline");
            *(void**) fn.pcap_close = dlsym(m_Handle, "pcap_close");
            if (!fn.pcap_open_live || !fn.pcap_open_offline || !fn.pcap_findalldevs || !fn.pcap_next)
                return false;
        } else {
            return false;
        }
#endif
        return true;
    }

};


struct Pcap
{

    Pcap();

    ~Pcap();

    bool offline(const char* fname);

    bool live(const char* dev);

    Result_t& next();

    bool hasNext() const;

    void loop();

    static void showAll();


private:
    struct {
        bool live;
        bool running;
    } m_options;

    std::atomic<int> m_stop;

    pcap_t* p_Cap;

    char errbuf[PCAP_ERRBUF_SIZE];

    Result_t m_nextRes;

    tjson::JsonSerializer serializer; //make static to access in private scenarios

    /**
     * @brief VParse - variardic terminator
     * @return {}
     */
    Result_t VParse()
    {
        return {};
    }

    template <typename T, class...Args>
    auto VParse(T&& value, Args&&...FArgs)
    {
        if constexpr (is_validator<T>::value){
            T& resultready = value();
            if (resultready.Valid) {
                value.value.type = resultready.type(); //add live option writer
                serializer.Add(resultready.jsonb);
                return value.value;
            }
            else {
                return VParse(std::forward<Args>(FArgs)...);
            }
        } else {
            return VParse(std::forward<Args>(FArgs)...);
        }
    }
};
}//libnetin
#endif // PCAP_H
