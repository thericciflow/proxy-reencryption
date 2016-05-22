CC      := g++
LD      := g++
CFLAGS  := -g -O0 -Iincludes/ -std=c++11
LDFLAGS := -lm -lgmp -lgmpxx
OPTFLAGS:= -g -O0
TARGET  := ecpy
OBJS    := finitefield.o util.o main.o pairing.o

vpath %.h includes/
vpath %.cpp src/
vpath %.o bin/

all: bin/$(TARGET)

bin/$(TARGET): $(OBJS)
	$(LD) -o $@ $(addprefix bin/,$(OBJS)) $(LDFLAGS) $(OPTFLAGS)

%.o: %.cpp includes/*.h
	$(CC) $(CFLAGS) -c -o bin/$@ $< $(OPTFLAGS)
