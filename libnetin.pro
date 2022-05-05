TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -save-temps
#QMAKE_CXXFLAGS +=  --std=c++17 -stdlib=libstdc++  -static -lstdc++

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
#    LIBS += -lpthread -ldbus-1
}

#DESTDIR = $$PWD/Dest
#OBJECTS_DIR = $$PWD/obj
#MOC_DIR = $$PWD/obj


SOURCES += \
        App.cpp \
        Pcap.cpp \
        defs.cpp \
        main.cpp \
        mqtt_t.cpp \
        nill_t.cpp \
        rtcp_t.cpp \
        rtp_t.cpp \
        stun_t.cpp \
        t38_t.cpp \
        turn_t.cpp \
        utils.cpp \
        websocket_t.cpp

HEADERS += \
    App.h \
    Pcap.h \
    defs.h \
    mqtt_t.h \
    nill_t.h \
    rtcp_t.h \
    rtp_t.h \
    stun_t.h \
    t38_t.h \
    tjson.hpp \
    turn_t.h \
    types.h \
    utils.h \
    websocket_t.h

DISTFILES += \
    Data/out.txt \
    README.md \
    prj/libnetin.sln \
    prj/libnetin/libnetin.vcxproj \
    prj/libnetin/libnetin.vcxproj.filters
