TARGET   := ecpy_native.so
CC       := g++
LD       := g++
LDFLAGS  := -lboost_python -lgmp -lgmpxx
CFLAGS    = -Iincludes/ -I$(shell python -c 'from distutils.sysconfig import *; print get_python_inc()') -DPIC
OPTFLAGS := -O0 -g -fPIC -shared -std=c++11
OBJS     := ecpy_native.o ZZPoint.o ZZ.o

vpath %.cpp src/
vpath %.h includes/
vpath %.hpp includes/
vpath %.o bin/

all: bin/$(TARGET)

bin/$(TARGET): $(OBJS)
	$(LD) -o $@ $(addprefix bin/,$(OBJS)) $(LDFLAGS) $(OPTFLAGS)

%.o: %.cpp includes/*
	$(CC) $(CFLAGS) -c -o bin/$@ $< $(OPTFLAGS)
