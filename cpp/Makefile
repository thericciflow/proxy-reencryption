TARGET   := ecpy_native.so
CC       := g++
LD       := g++
LDFLAGS  := -lboost_python -lgmp -lgmpxx
CFLAGS   := -Iincludes/ -I$(shell python -c 'from distutils.sysconfig import *; print get_python_inc()') -DPIC
OPTFLAGS := -O2 -Os -mtune=corei7 -mavx -g -fPIC -shared -std=c++11
OBJS     := ecpy_native.o ZZTheory.o ZZPoint.o ZZ.o ec.o
SUDO     := sudo
CP       := cp
DESTDIR  := $(shell python -c 'import sys; sys.stdout.write(__import__("sysconfig").get_path("purelib"))')


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
	$(SUDO) cp -rf bin/$(TARGET) $(DESTDIR)
