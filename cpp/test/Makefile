CC       := g++
LD       := g++
CFLAGS   := -I../includes/
OPTFLAGS := -O3 -Os -mtune=corei7 -mavx 
GFLAGS   := -g -std=c++11
PYTHON   := python


vpath %.o ../bin/

.PHONY: all
all: bin/test
	valgrind -v ./bin/test
	sudo ldconfig
	$(PYTHON) test.py

bin/test: bin/test.o $(OBJS)
	$(LD) -o bin/test $(GFLAGS) $(OPTFLAGS) $^ $(LDFLAGS)

bin/%.o: %.cpp ../bin/*
	$(CC) -o $@ -c $< $(OPTFLAGS) $(CFLAGS) $(GFLAGS)
