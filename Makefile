# Tools
CXX:=g++
RES:=windres
LD:=g++
ifeq ($(OS),Windows_NT)
	RM:=del /F
else
	RM:=rm -f
endif
# Configuration / flags
CXXFLAGS:=-Wall -Wmissing-include-dirs -Wunreachable-code -Wfloat-equal -Winline -std=c++11 -msse2 -O3 -DWIN32_LEAN_AND_MEAN
RESFLAGS:=-O coff
LDFLAGS:=-s
ifeq ($(OS),Windows_NT)
	LDFLAGS+= -static
endif
# Files
BIN:=Space_Shooter
SRC:=$(wildcard src/*.c src/*.cpp)
ifeq ($(OS),Windows_NT)
	SRC+= $(wildcard src/*.rc)
endif
OBJ:=$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.rc,%.res,$(SRC))))
LIBS:=-lncurses

# Full build batch (first target = default)
all: $(OBJ) $(BIN)

# Build binary / link objects
$(BIN): $(OBJ)
	$(LD) -o $@ $^ $(LIBS) $(LDFLAGS)

# Build objects / compile sources
src/%.o: src/%.c src/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)
ifeq ($(OS),Windows_NT)
src/%.res: src/%.rc
	$(RES) -o $@ $< $(RESFLAGS)
endif

# Remove all already built files
clean:
ifeq ($(OS),Windows_NT)
	$(RM) src\*.o src\*.res $(BIN).exe
else
	$(RM) src/*.o $(BIN)
endif
# Make 'clean' target phony, so ignoring an existing file
.PHONY: clean