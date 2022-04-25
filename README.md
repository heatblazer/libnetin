#### RFC STUN and RTP VoIP analyser

#### Build:
~~1.Using make:~~

~~`make Makefile`~~

1. For windows - open `prj` solution file

2. Linux - Using  `qtcreator` file or invoke `qmake` in the root folder of the project.

This program is made with the help of `libpcap` https://github.com/the-tcpdump-group/libpcap
please read the license agreement of `PCAP`.

3.For `Win32` install follow the guide to install `NCAP` from here: https://nmap.org/npcap/#download

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

#### App recognition (not 100% accurate):
1. `Whatsapp`
2. `Telegram`
3. `Facebook`
4. `Skype`

#### Live support
1. In order to use live mode you need administrator rules for windows or `sudo` for Linux

#### Bugs:

~~1.Win32 build can't resolve StunRFC, possible `pragma pack(x)` needed.~~ 

#### TODO:
1. Resolve `TCP` parsing

-------------

