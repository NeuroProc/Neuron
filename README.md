--------------MAN IMAGE---------------
http://i.imgur.com/e2v5jyc.png


--------------DLL CREATE-------------
EXPORT DEFINES:
#ifdef BUILDING_XML_DLL
#define XML_DLL __attribute__((visibility ("default")))
#else
#define XML_DLL
#endif

DLL BUILD:
c++ -fPIC -c -DBUILDING_XML_DLL xml.cpp
c++ -shared -Wl,-soname,libxml.so.1 -o libxml.so.1.0 *.o

LN:
ln -s libxml.so.1.0 libxml.so.1
ln -s libxml.s0.1.0 libxml.so

TEST BUILD:
c++ test.cpp -o testbin -L. -lxml

TEST LAUNCH:
FreeBSD: setenv LD_LIBRARY_PATH .
Linux:   LD_LIBRARY_PATH=.
./testbin


