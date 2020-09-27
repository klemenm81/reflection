CXX := g++
LD := ld
AR := ar
CXXFLAGS := -g -std=c++17 -fvisibility=hidden -fPIC -I../../libjson/include -I../../libjobject
INCLUDES := -I../../libjson/include -I../../libjobject
LIBS := ../../libjson/lib/linux_x64/2.30/libjsoncpp.a -ldl
OBJECTS  := test.o metadata.o

my_program: $(OBJECTS)
	$(CXX) -shared -o test.so $(OBJECTS) $(LIBS)

%.o: %.cxx
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $(input) -o $(output)
