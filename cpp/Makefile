CC      := g++
LD      := g++
RM      := rm
CFLAGS  := -Iincludes/ -std=c++11
LDFLAGS := -lm -lgmp -lgmpxx
OPTFLAGS:= -g -O0
#OPTFLAGS:= -g -O2 -mtune=native
#OPTFLAGS:= -O2 -Ofast -mtune=corei7 -fomit-frame-pointer
TARGET  := ecpy
OBJS    := complexfield.o finitefield.o util.o main.o pairing.o

vpath %.h includes/
vpath %.cpp src/
vpath %.o bin/

all: bin/$(TARGET)

bin/$(TARGET): $(OBJS)
	$(LD) -o $@ $(addprefix bin/,$(OBJS)) $(LDFLAGS) $(OPTFLAGS)

%.o: %.cpp includes/*.h
	$(CC) $(CFLAGS) -c -o bin/$@ $< $(OPTFLAGS)

clean:
	$(RM) bin/*.o
	$(RM) bin/$(TARGET)
