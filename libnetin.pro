TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt


INCLUDEPATH += $$PWD/External/include

win32 {
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
    parse.h \
    rtp_t.h \
    stun_t.h \
    utils.h
