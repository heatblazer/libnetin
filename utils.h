#ifndef UTILS_H
#define UTILS_H
#include "defs.h"
#include "types.h"

#include <vector>
#include <string>

namespace utils {

    void split(const char *str, const char *delim, std::vector<std::string>& out);

    struct EthL4 GetEthL4(const u_char* data);

    template <size_t CVAL>
    size_t  padcalc(size_t val)
    {
        size_t mod = val % CVAL;
        return mod == 0 ? mod : CVAL - mod;
    }

    template <typename T, bool BE=true>
    T tobin(const char* data)
    {
        T val=0;
        for(size_t i=0; i < sizeof(T); i++) {
            val |= (data[i]) << (i * 8);
        }
        if (BE) {
            val = SWAP4(val);
        }
        return val;
    }


}

#endif // UTILS_H
