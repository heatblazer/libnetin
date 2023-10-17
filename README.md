#### RFC STUN and RTP VoIP analyser
An interactive or offline packet analyzer with `libpcap` for `Windows` and `Linux`.
Analysys if rtp, stun, rtsp, mqtt, websocket, stun, rtp, rtcp and more to come.
Writes out to a json as a format. 

#### Build:
~~1.Using make:~~

~~`make Makefile`~~

1. For windows - open `prj` solution file

2. Linux - Using  `qtcreator` file or invoke `qmake` in the root folder of the project.

This program is made with the help of `libpcap` https://github.com/the-tcpdump-group/libpcap
please read the license agreement of `PCAP`.

3. If you get linker errors, install `libDbus` for your distro and add `LIBS+= -lpthread -ldbus-1` to `.pro` file. Also you may recompile `libpcap` with `./configure --enable-dbus=no'

4. For `Win32` install follow the guide to install `NCAP` from here: https://nmap.org/npcap/#download

5. There is a custom script `imake.py` that can build the project without qtcreator or cmake.
 Call `python3 imake.py` in a console then copy / paste the output and call `g++` or `clang` with the pasted output. It will output the `libnetin` exe.
 Also call `python3 imake.py -d` if you want a debug version
#### Usage:
1. Show devices: `libnetin -l`
2. Analyze a capture `libnetin -p mycapture.pcap`
3. Live mode `sudo libnetin -l <Mydevice>`

* Note: when in live mode, press `q` or `Q` to stop capturing.

#### Protocols:
1. `STUN` 
2. `RTP` 
3. `RTCP`
4. `UDPTL` - `t38` (not fully implemented, artifacts)
5. `MQTT` - support for facebook [pending].
6. `WebSocket` - support and demasking (needed for `MQTT` )
7. `RTSP` - partial

#### App recognition (not 100% accurate):
1. `Whatsapp`
2. `Telegram`
3. `Facebook` (chat & VoIP)
4. `Skype`

#### Live support
1. In order to use live mode you need administrator rules for windows or `sudo` for Linux

#### Bugs:

~~1.Win32 build can't resolve StunRFC, possible `pragma pack(x)` needed.~~ 

#### TODO:
1. ~~Resolve `TCP` parsing~~
2. ~~Create an appendable `json` builder~~
3. Resolve `TCP` options 
4. `MQTT` is buggy, some offset is not good or the crash comes from `WebSocket`

#### PVS-STUDIO report:
```
pvs-studio.com/en/docs/warnings/	1	err	Help: The documentation for all analyzer warnings is available here: https://pvs-studio.com/en/docs/warnings/.
/home/ilian/gitprojects/libnetin/types.h	33	warn	V730 Not all members of a class are initialized inside the constructor. Consider inspecting: value.
```

-------------

