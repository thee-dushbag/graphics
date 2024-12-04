CXX=g++
ROOT_DIR=$(shell realpath ../..)
INC_DIR=$(ROOT_DIR)/include
LIB_DIR=$(ROOT_DIR)/lib
CXXFLAGS=-Wall -I"$(INC_DIR)" -Wl,-rpath="$(LIB_DIR)" -L"$(LIB_DIR)"
CXXFLAGS+=-lX11 -lm -lpng16 -lolcPGE -lGL
OLC_LIB=$(LIB_DIR)/libolcPGE.so
MAKE+=--no-print-directory

SOURCES=$(shell find . -name '*.cpp' -type f)
OBJECTS=$(subst .cpp,.o,$(SOURCES))
PROGRAMS=$(subst .o,,$(OBJECTS))
TO_CLEAN=$(PROGRAMS)

ifeq ($(build),release)
	CXXFLAGS+=-03
else
	CXXFLAGS+=-g
endif

all: $(OLC_LIB) $(PROGRAMS)

$(OLC_LIB):
	@$(MAKE) -C "$(LIB_DIR)" CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)"

clr:
	rm -rfv $(PROGRAMS)

clean: clr
	@$(MAKE) -C "$(LIB_DIR)" clean

