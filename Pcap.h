#ifndef PCAP_H
#define PCAP_H
#include "defs.h"
#include "types.h"
#include "tjson.hpp"
#include <memory>
#include <pcap/pcap.h>

/**
 * @brief The is_validator specialize that template for all supported types
 * in VParse(...)
 */
template<typename T>
struct is_validator
{
    static const bool value = false;
};

namespace libnetin {
struct Pcap
{

    Pcap();

    ~Pcap();

    bool init(const char* fname);

    void operator++();

    Result_t& next();

    bool hasNext() const;

    void loop();

    static void showAll();

private:
    /**
     *@deprecated
     */
    template<typename T, typename Return_T>
    Return_T packetHandlerT(u_char *userData, const Result_t& res, T protocol)
    {
        (void)userData; //reserved userdata...
        return protocol(res);
    }

    template<typename T>
    T packetHandlerT(T protocol)
    {
        return protocol();
    }

    pcap_t* p_Cap;

    char errbuf[PCAP_ERRBUF_SIZE];

    bool m_offline;

    Result_t m_nextRes;


    tjson::JsonSerializer serializer;

    /**
     * @brief VParse - variardic terminator
     * @return {}
     */
    Result_t VParse()
    {
        return {};
    }

    template <typename T, class...Args>
    auto VParse(T value, Args&&...FArgs)
    {
        if constexpr (is_validator<T>::value){
            T resultready = packetHandlerT(value);
            if (resultready.Valid) {
                value.value.type = resultready.type();
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
