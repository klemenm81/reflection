CXX := g++
LD := ld
LDFLAGS := -Wl,--export-dynamic -shared
AR := ar
CXXFLAGS := -g -std=c++17 -fvisibility=hidden -fPIC -I../../libjson/include -I../../libjobject
INCLUDES := -I../../libjson/include -I../../libjobject
LIBS := ../../libjson/lib/linux_x64/2.30/libjsoncpp.a -ldl
OBJECTS  := test.o metadata.o

all: $(OBJECTS)
	$(CXX) $(LDFLAGS) -o Test.so $(OBJECTS) $(LIBS)
	cp Test.so ../..

%.o: %.cxx
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $(input) -o $(output)

clean:
	rm *.o *.so
