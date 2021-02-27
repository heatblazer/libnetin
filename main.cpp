#include <iostream>
#include <pcap/pcap.h>
#include "Pcap.h"
#include "utils.h"

int main(int argc, char** argv)
{
    Pcap p;
    for(int i=1; i < argc; i++) {
        if (p.init(argv[i])) {
            p.loop();
        }
    }

    return 0;
}


