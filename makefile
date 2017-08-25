IDIR=/Users/Boelroy/Documents/Project/github/ChakraCore/lib/Jsrt
BUILDDIR=./build
CC=g++

LIBRARY_PATH=/Users/Boelroy/Documents/Project/github/ChakraCore/out/Test
LDIR=$(LIBRARY_PATH)/lib/libChakraCoreStatic.a

ICU4C_LIBRARY_PATH ?= /usr/local/opt/icu4c
CFLAGS=-lstdc++ -std=c++11 -I$(IDIR) -I./src -I$(ICU4C_LIBRARY_PATH)/include
FORCE_STARTS=-Wl,-force_load,
FORCE_ENDS=
LIBS=-framework CoreFoundation -framework Security -lm -ldl -Wno-c++11-compat-deprecated-writable-strings \
	-Wno-deprecated-declarations -Wno-unknown-warning-option -o $(BUILDDIR)/main
LDIR+=$(ICU4C_LIBRARY_PATH)/lib/libicudata.a \
	$(ICU4C_LIBRARY_PATH)/lib/libicuuc.a \
	$(ICU4C_LIBRARY_PATH)/lib/libicui18n.a


testmake:
	$(CC) src/main.cpp $(CFLAGS) $(FORCE_STARTS) $(LDIR) $(FORCE_ENDS) $(LIBS)

.PHONY: clean

clean:
	rm $(BUILDDIR)/main