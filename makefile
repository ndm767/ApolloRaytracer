PNAME := apollo

PCXXSRC := $(wildcard src/*.cpp)
PCXXSRC += $(wildcard src/display/*.cpp)
PCXXSRC += $(wildcard src/geometry/*.cpp)
PCXXSRC += $(wildcard src/camera/*.cpp)

POBJS := $(PCXXSRC:.cpp=.o)

CXXFLAGS += -std=c++17
CXXFLAGS += -Wall -Wpedantic -Wextra

LDLIBS += -lSDL2

.SILENT: all $(POBJS)
.PHONY: all

all: $(PNAME)

$(PNAME): $(POBJS)
	@-$(LINK.cc) $(POBJS) -o $(PNAME) $(LDLIBS)
	@-$(RM) $(POBJS)

clean:
	@- $(RM) $(PNAME)
	@- $(RM) $(POBJS)