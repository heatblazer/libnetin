#ifndef APP_H
#define APP_H
#include "Pcap.h"
#include <thread>
#include <vector>

namespace libnetin
{

class App
{

    Pcap m_pcap;

    std::vector<std::string> m_args;

public:

    explicit App(int &argc,  char** argv);

    ~App() noexcept
    {

    }

    int exec();
};



}//namespace libnetin



#endif // APP_H
