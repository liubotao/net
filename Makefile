CXX      = g++
INCPATH  = -I.
CXXFLAGS = -std=c++0x -pipe -Wall -W -O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 --param=ssp-buffer-size=4 -mtune=generic 
LIBS     = $(SUBLIBS)  /usr/local/lib/libevent.a /usr/local/lib/libevent_pthreads.a -lrt -pthread /usr/local/lib/libjemalloc.a

HEADERS = src/tcpsocket.h 
        
SOURCES = src/tcpsocket.cc \
		  src/tcpserver.cc \
		  src/eventloop.cc \
		  src/thread.cc \
		  src/buffer.cc \
		  src/echoserver.cc \
		  src/file_util.cc \
          src/config.cc \
		  src/main.cc

OBJECTS = tmp/tcpsocket.o \
		  tmp/tcpserver.o \
		  tmp/eventloop.o \
		  tmp/thread.o \
		  tmp/buffer.o \
		  tmp/echoserver.o \
		  tmp/file_util.o \
		  tmp/config.o \
		  tmp/main.o

main:src/main.cc $(OBJECTS)
	 $(CXX) -o main  $(OBJECTS) $(LIBS)

clean:
	rm -f $(OBJECTS)
	rm -f *.core

tmp/main.o:src/main.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/main.o src/main.cc


tmp/config.o:src/config.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/config.o src/config.cc

tmp/file_util.o:src/file_util.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/file_util.o src/file_util.cc

tmp/tcpsocket.o:src/tcpsocket.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/tcpsocket.o src/tcpsocket.cc

tmp/eventloop.o:src/eventloop.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/eventloop.o src/eventloop.cc

tmp/thread.o:src/thread.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/thread.o src/thread.cc

tmp/buffer.o:src/buffer.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/buffer.o src/buffer.cc

tmp/tcpserver.o:src/tcpserver.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/tcpserver.o src/tcpserver.cc

tmp/echoserver.o:src/echoserver.cc   
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tmp/echoserver.o src/echoserver.cc
