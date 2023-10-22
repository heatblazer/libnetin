#include "custom_t.h"
#include "utils.h"

namespace custom {

CustomRFC::CustomRFC(const IParseable::type& res MAYBEUNUSED)
    : IParseable<Result_t, CustomRFC>{res}
{
}

CustomRFC &CustomRFC::operator()(const IParseable::type& res)
{
    struct EthL4 eth = utils::GetEthL4(res.data);
    MAYBEUNUSED  size_t offset = 0;
    MAYBEUNUSED  size_t total = res.out.len;
    MAYBEUNUSED unsigned int payload_len = 0;
//TODO: custom type is not supported
    switch (eth.type) {
    FALLTROUGH;
    case EthL4::TCP:
    case EthL4::UDP:
    case EthL4::UNKNOWN:
    default:
        break;
    }
    return *this;
}

CustomRFC &CustomRFC::operator()()
{
    return this->operator()(this->value);
}


} // custom

//hint
#if 0
        offset = eth.options_len + (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(tcphdr));
        MAYBEUNUSED const char *pdata = (const char*)res.data+offset;
        break;
#endif
