#### RFC STUN and RTP VoIP analyser

#### Build:
1.Using make:

`make Makefile`

2.Using  `Qt` either `qtcreator.pro` file or invoke `qmake` in the root folder of the project.

This program is made with the help of `libpcap` https://github.com/the-tcpdump-group/libpcap
please read the license agreement of `PCAP`.

3.For `Win32` install follow the guide to install `NCAP` from here: https://nmap.org/npcap/#download


#### Protocols:
1. `STUN` 
2. `RTP` 
3. `RTCP`
4. `UDPTL`

#### App recognition (not 100%):
1. `Whatsapp`
2. `Telegram`
3. `Facebook`
4. `Skype`

#### Bugs:
~~1.Win32 build can't resolve StunRFC, possible `pragma pack(x)` needed.~~ 


-------------

