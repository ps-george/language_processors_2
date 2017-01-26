CXXFLAGS += -std=c++11 -W -Wall -g

# Set constant directorys
SDIR=src
INC=-Iinclude
# Preprocessor flags, MMD creates dependency files
CPPFLAGS += -MMD $(INC)

# List of source files
_SRCS=regex_engine.cpp penny_machine.cpp
SRCS=$(patsubst %,$(SDIR)/%,$(_SRCS))

all: bin/regex_engine
%/regex_engine: $(SRCS:%.cpp=%.o)
	g++ $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(INC)

clean:
	rm -f bin/*
	rm -f regex_engine_ref

.PHONY: clean

-include $(_SRCS:%.cpp=%.d)
