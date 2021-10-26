TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

#DEFINES += MINJSON

win32 {
    #TODO: implement
    message("Win32")
    HEADERS +=  winpcap_t.h
    SOURCES +=  winpcap_t.cpp

    INCLUDEPATH += $$PWD/External/Include
    LIBS += -L$$PWD/External/win32/Lib/ -lwpcap -lws2_32
} else {
    message("Unix")
    INCLUDEPATH += $$PWD/External/include
    LIBS += -L$$PWD/External/lib/libpcap.a -lpcap
}

#DESTDIR = $$PWD/Dest
#OBJECTS_DIR = $$PWD/obj
#MOC_DIR = $$PWD/obj


SOURCES += \
        Pcap.cpp \
        defs.cpp \
        main.cpp \
        nill_t.cpp \
        rtcp_t.cpp \
        rtp_t.cpp \
        stun_t.cpp \
        t38_t.cpp \
        turn_t.cpp \
        utils.cpp

HEADERS += \
    Pcap.h \
    defs.h \
    nill_t.h \
    rtcp_t.h \
    rtp_t.h \
    stun_t.h \
    t38_t.h \
    tjson.hpp \
    turn_t.h \
    types.h \
    utils.h

DISTFILES += \
    Data/out.txt \
    README.md
