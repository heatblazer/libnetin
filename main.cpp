#include "Pcap.h"
#include "utils.h"
#include "App.h"
#include <iostream>
#include <pcap/pcap.h>

int main(int argc, char** argv)
{

    libnetin::App app{argc, argv};
    return app.exec();
}


