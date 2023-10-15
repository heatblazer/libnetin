

compile_files = ['main.cpp', 'mqtt_t.cpp', 'nill_t.cpp', 'Pcap.cpp', 't38_t.cpp', 'rtsp_t.cpp','rtcp_t.cpp', 'rtp_t.cpp',
                 'stun_t.cpp', 'turn_t.cpp','utils.cpp', 'websocket_t.cpp', 'App.cpp']


class CompileBld:

    def __init__(self) -> None:
        self._compileropt = []
        self._this = self

    def addExeName(self, name):
        s = str("-o " + name)
        self._compileropt.append(s)
        return self

    def addDefines(self, d):
        s = str("-D"+d)
        self._compileropt.append(s)
        return self

    def addCXXFlags(self, f):
        self._compileropt.append(f)
        return self

    def addLinkerOpt(self,l):
        s = str("-l"+l)
        self._compileropt.append(s)
        return self._this
    
    def addLinkerPath(self, p):
        s = str("-L"+p)
        self._compileropt.append(s)
        return self

    def addIncludePath(self, i):
        s = str("-I"+i)
        self._compileropt.append(s)
        return self
    
    def addCFiles(self, files):
        for f in files:
            self._compileropt.append(f)
        return self


    def build(self):
        s = ' '.join(self._compileropt)
        return s


def main():

    cb = CompileBld()
    prn  = cb.addExeName("libnetin").addCFiles(compile_files).addCXXFlags('-O2').addCXXFlags('-s').addDefines('NDEBUG').addCXXFlags('-std=c++17')\
    .addIncludePath("/home/ilian/dev/libnetin/External/include").addLinkerPath("/home/ilian/dev/libnetin/External/lib") \
    .addLinkerOpt('pthread') \
    .addLinkerOpt('pcap').addLinkerOpt('dbus-1').build() 
    print(prn)
    pass


if __name__ == "__main__":
    main()
