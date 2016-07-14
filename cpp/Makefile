TARGET   := libecpy_native.so
CC       := g++
LD       := g++
LDFLAGS  := -lgmp -lgmpxx
CFLAGS   := -Iincludes/
OPTFLAGS := -O3 -Os -mtune=corei7 -mavx -g -fPIC -shared -std=c++11
OBJS     := ecpy_native.o
CP       := cp
DESTDIR  := /usr/local/lib/


vpath %.cpp src/
vpath %.h includes/
vpath %.hpp includes/
vpath %.o bin/

all: bin/$(TARGET)

bin/$(TARGET): $(OBJS)
	$(LD) -o $@ $(addprefix bin/,$(OBJS)) $(LDFLAGS) $(OPTFLAGS)

%.o: %.cpp includes/*
	$(CC) $(CFLAGS) -c -o bin/$@ $< $(OPTFLAGS)

install: bin/$(TARGET)
	cp -rf bin/$(TARGET) $(DESTDIR)
