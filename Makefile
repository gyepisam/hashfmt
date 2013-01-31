
program = hashfmt
CC = g++
CXXFLAGS += -std=c++0x -Wall

$(program): $(program).o

.PHONY: clean tests

tests test:
	@sh tests

clean:
	$(RM) $(program) .o *~

