CXX := g++
LD := ld
LDFLAGS := -Wl,--export-dynamic
AR := ar
CXXFLAGS := -g -std=c++17 -fvisibility=hidden -fPIC -I../libjson/include -I../libjobject
INCLUDES := -I../libjson/include -I../libjobject
LIBS := ../libjson/lib/linux_x64/2.30/libjsoncpp.a ../libjobject/libjobject.a -ldl -lbfd
OBJECTS  := main.o metadata.o example.o

all: $(OBJECTS)
	$(CXX) $(LDFLAGS) -o client $(OBJECTS) $(LIBS)
	cp client ..

%.o: %.cxx
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $(input) -o $(output)

clean:
	rm client *.o
