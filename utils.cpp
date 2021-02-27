#include "utils.h"
#include <string.h>

namespace utils {

    void split(const char *str, const char *delim, std::vector<std::string>& out)
    {
        const char* begin = str;
        const char* it = strstr(str, delim);
        if (it != NULL){
            std::string data{begin, it};
            out.push_back(data);
            it++;
            split(it, delim, out);
        } else {
            std::string data{str};
            out.push_back(data);
        }
    }


}
