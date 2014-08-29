CC=g++
BASE_CXXFLAGS=-g -std=c++11 -Igecode -Iinclude 
CXXFLAGS=$(BASE_CXXFLAGS) -O3 -g

LDPATH=-Lgecode
LDFLAGS=-g -lgecodedriver -lgecodesupport -lgecodekernel -lgecodesearch -lgecodeint -lgecodeminimodel -lgecodeset -lgecodefloat -lgecodeflatzinc

SRC_DIR=src
GECODE_DIR=gecode
BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/o
BIN_DIR=$(BUILD_DIR)/bin

CPP := $(shell find $(SRC_DIR) -type f -name '*.cpp' -printf '%P\n')
OBJS=$(addprefix $(OBJ_DIR)/,$(CPP:.cpp=.o))
#LD_OBJS:=$(filter-out $(wildcard $(OBJ_DIR)/bin/*.o), $(OBJS))
#LD_OBJS=$(filter-out $(OBJ_DIR)/$($(filter bin/%.cpp, $(CPP)):.cpp=.o), $(OBJS))
LD_OBJS=$(filter-out $(addprefix $(OBJ_DIR)/, $(filter bin/%.o, $(CPP:.cpp=.o))), $(OBJS))

BIN_SRC_DIR=$(SRC_DIR)/bin
BIN_SRC_CPP=$(wildcard $(BIN_SRC_DIR)/*.cpp)
BIN_FILES=$(addprefix $(BIN_DIR)/, $(notdir $(BIN_SRC_CPP:.cpp=)))

objs: $(OBJS)

run: CXXFLAGS += -DUSE_BIT_INVERSION=true
run: build/bin/main

benchmark: CXXFLAGS += -DUSE_BIT_INVERSION=true
benchmark: build/bin/incremental

# special non-optimized variants for testing
test: CXXFLAGS = $(BASE_CXXFLAGS) -O0 -g
test: build/bin/test

debug: CXXFLAGS = $(BASE_CXXFLAGS) -O0 -g
debug: build/bin/main

$(BIN_DIR)/%: $(OBJS) | $(BIN_DIR)
	mkdir -p $(dir $@) && $(CC) -o $@ $(LDPATH) $(OBJ_DIR)/bin/$(notdir $@.o) $(LD_OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@) && $(CC) $(CXXFLAGS) $< -c -o $@

$(GECODE_DIR):
	mkdir -p $(GECODE_DIR) 
	wget --output-document=$(GECODE_DIR)/gecode.tar.gz http://www.gecode.org/download/gecode-4.2.1.tar.gz 
	tar --strip-components=1 -C $(GECODE_DIR) -xvvzf $(GECODE_DIR)/gecode.tar.gz

gecode-compile:
	cd gecode
	./configure --with-vis=../src/bitvec/bitvec.vis
	make

diff-list:
	bash -c "colordiff -y <(ls -1 src/bitvec/ | grep -v 'bitvec.vis' | grep -o '^[^.]*' | sort) <(ls -1 build/o/bitvec/ | grep -o '^[^.]*' | sort)"

bitwise:
	ack -r --type=notext '^(?!.*//)(?!\s*\*).*\b(and|or)\b' src/* include/* test/* benchmark/*

fixme:
	ack -r FIXME src/ include/

todo:
	ack -r TODO src/ include/

# this doesn't work for the user running make; it's just a note
path:
	cd gecode/
	export LD_LIBRARY_PATH=`pwd`

gdb:
	gdb -tui --args ./build/bin/sbox -model bitvec 6 4 12

runit:
	./build/bin/main -model bitvec -branching none -s2 hc -s7 global 6 4 12

clean:
	rm -f $(BIN_FILES)
	find $(OBJ_DIR) -type f -name '*.o' -delete

