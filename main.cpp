#include "App.h"
#include <iostream>
#include <pcap/pcap.h>

int main(int argc, char** argv)
{
#if 1
    libnetin::App app{argc, argv};
    return app.exec();
#else
    libnetin::App app{argc, argv};
    app.test_offline();
    return 0;
#endif
}


