#include "App.h"


namespace libnetin
{

App::App(int& argc, char **argv)
{
    for(int i=1; i < argc; i++) {
        m_args.push_back(argv[i]);
    }
}

int App::exec()
{

    bool res=false;
    if (m_args.size() == 0) {
        std::cout << "(libnetin --help) to see usage\r\n";
        return -1;
    } else {
        for(size_t i=0; i < m_args.size(); i++) {

            if (m_args[i] == "-l") {
                Pcap::showAll();
                return 0;
            } else if (m_args[i] == "-p"){
                for(size_t j=i+1; j < m_args.size(); j++) {
                    res = m_pcap.offline(m_args[j].c_str());
                }
            } else if (m_args[i] == "-i") {
                res = m_pcap.live(m_args[i+1].c_str());
            } else {

            }
        }
    }
    if (res) {
        m_pcap.loop();
    } else {
        std::cerr << "Failed to open pcap\r\n";
    }
    return res ? 0 : -1;
}

}//ns libnetin
