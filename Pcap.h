#ifndef PCAP_H
#define PCAP_H
#include "defs.h"
#include "types.h"
#include "tjson.hpp"
#include <memory>
#include <pcap/pcap.h>
#include <atomic>

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
struct Pcap
{

    Pcap();

    ~Pcap();

    bool offline(const char* fname);

    bool live(const char* dev);

    void operator++();

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
