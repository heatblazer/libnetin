TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

#DEFINES += MINJSON
INCLUDEPATH += $$PWD/External/include

win32 {
#TODO: implement
    message("Win32")
} else {
    message("Unix")
    LIBS += -L$$PWD/External/lib/libpcap.a -lpcap
}

SOURCES += \
        Pcap.cpp \
        main.cpp \
        rtp_t.cpp \
        stun_t.cpp \
        utils.cpp

HEADERS += \
    Pcap.h \
    defs.h \
    rtp_t.h \
    stun_t.h \
    tjson.hpp \
    types.h \
    utils.h
