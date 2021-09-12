PNAME := apollo

PCXXSRC := $(wildcard src/*.cpp)
PCXXSRC += $(wildcard src/display/*.cpp)
PCXXSRC += $(wildcard src/display/gl/*.cpp)
PCXXSRC += $(wildcard src/geometry/*.cpp)
PCXXSRC += $(wildcard src/camera/*.cpp)
PCXXSRC += $(wildcard src/light/*.cpp)
PCXXSRC += $(wildcard src/backends/*.cpp)
PCXXSRC += $(wildcard src/backends/gpu/*.cpp)

POBJS := $(PCXXSRC:.cpp=.o)

CXXFLAGS += -std=c++17 -O3

LDLIBS += -lSDL2 -lGL -lGLEW -lassimp -pthread

.SILENT: all $(POBJS)
.PHONY: all

all: $(PNAME)

$(PNAME): $(POBJS)
	@-$(LINK.cc) $(POBJS) -o $(PNAME) $(LDLIBS)
	@-$(RM) $(POBJS)

clean:
	@- $(RM) $(PNAME)
	@- $(RM) $(POBJS)