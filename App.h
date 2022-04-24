#ifndef APP_H
#define APP_H
#include "Pcap.h"
#include <thread>


namespace libnetin
{

class App
{
public:

    explicit App(int &argc,  char** argv);

    ~App() noexcept
    {

    }

    int exec();
};



}//namespace libnetin



#endif // APP_H
