ROOTLIBS = $(shell root-config --libs) $(shell pkg-config --libs liblz4)
CXXFLAGS = $(shell root-config --cflags) $(shell pkg-config --cflags liblz4) 
LIBFLAG = -c $(shell root-config --auxcflags) -D__LZ4__ -D__LIBZ__
DEBUG = -D__DEBUG__
LIB = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
CXX = g++

.PHONY: clean
all: $(LIB)

$(LIB): %.o: %.cpp
	$(CXX) $(LIBFLAG) -o $@ $<

clean:
	-rm -f $(LIB)
