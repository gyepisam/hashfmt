H = hashfmt

$(H): CXXFLAGS += -std=c++0x -g3 -Wall
$(H): $(H).cc

.PHONY: clean

clean:
	$(RM) $(H)

