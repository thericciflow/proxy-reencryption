TARGET   := libecpy_native.so
CC       := g++
LD       := g++
LDFLAGS  := -lgmp -lgmpxx
CFLAGS   := -Iincludes/
OPTFLAGS := -O2 -Os -mtune=corei7 -mavx -g 
GFLAGS   := -fPIC -shared -std=c++11 -DPIC -g
OBJS     := ecpy_native.o FF.o
CP       := cp
DESTDIR  := /usr/local/lib/
SUDO     := sudo
PIP      := pip


vpath %.cpp src/
vpath %.h includes/
vpath %.hpp includes/
vpath %.o bin/
export OBJS LDFLAGS

.PHONY: all
all: bin/$(TARGET)

bin/$(TARGET): $(OBJS)
	$(LD) -o $@ $(addprefix bin/,$(OBJS)) $(LDFLAGS) $(OPTFLAGS) $(GFLAGS)

%.o: %.cpp includes/*
	$(CC) $(CFLAGS) -c -o bin/$@ $< $(OPTFLAGS) $(GFLAGS)

.PHONY: install
install: bin/$(TARGET)
	$(SUDO) $(CP) -rf bin/$(TARGET) $(DESTDIR)
	$(PIP) install --upgrade py/

.PHONY: test
test: install
	$(MAKE) -C test all
