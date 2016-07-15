CC       := g++
LD       := g++
CFLAGS   := -I../includes/
OPTFLAGS := -O3 -Os -mtune=corei7 -mavx -g -std=c++11
PYTHON   := python


vpath %.o ../bin/

.PHONY: all
all: bin/test
	./bin/test
	$(PYTHON) test.py

bin/test: bin/test.o $(OBJS)
	$(LD) -o bin/test $(OPTFLAGS) $^ $(LDFLAGS)

bin/%.o: %.cpp ../bin/*
	$(CC) -o $@ -c $< $(OPTFLAGS) $(CFLAGS)
