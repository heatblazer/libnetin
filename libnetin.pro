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

DESTDIR = $$PWD/Dest
OBJECTS_DIR = $$PWD/obj
MOC_DIR = $$PWD/obj


SOURCES += \
        Pcap.cpp \
        main.cpp \
        nill_t.cpp \
        rtcp_t.cpp \
        rtp_t.cpp \
        stun_t.cpp \
        turn_t.cpp \
        utils.cpp

HEADERS += \
    Pcap.h \
    defs.h \
    nill_t.h \
    rtcp_t.h \
    rtp_t.h \
    stun_t.h \
    tjson.hpp \
    turn_t.h \
    types.h \
    utils.h
