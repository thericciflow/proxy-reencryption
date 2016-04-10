CC      := g++
LD      := g++
CFLAGS  := -g -O0 -Iincludes/ -std=c++11 -g -O0
LDFLAGS := -lm -lgmp -lgmpxx -g -O0 # -shared -fPIC
TARGET  := ecpy
OBJS    := main.o finitefield.o util.o

vpath %.h includes/
vpath %.cpp src/
vpath %.o bin/

all: bin/$(TARGET)

bin/$(TARGET): $(OBJS)
	$(LD) -o $@ $(addprefix bin/,$(OBJS)) $(LDFLAGS)

%.o: %.cpp includes/*.h
	$(CC) $(CFLAGS) -c -o bin/$@ $<
