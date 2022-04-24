#include "Pcap.h"
#include "utils.h"
#include "App.h"
#include <iostream>
#include <pcap/pcap.h>

int main(int argc, char** argv)
{

    libnetin::App app{argc, argv};
#if 1
    Pcap::showAll();
    Pcap p;
    for(int i=1; i < argc; i++) {
        if (p.init(argv[i])) {
            p.loop();
        }
    }
#endif
    return 0;
}


