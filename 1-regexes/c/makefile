CXX=g++
CXXFLAGS += -std=c++11 -W -Wall -g
RM=rm -f

# Set constant directorys
ODIR = obj
SDIR = src
INC = -Iinclude

# List of source files
_SRCS = regex_engine.cpp penny_machine.cpp
SRCS = $(patsubst %,$(SDIR)/%,$(_SRCS))

# List of object files
_OBJS=$(subst .cpp,.o,$(_SRCS))
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

all: bin/regex_engine

%/regex_engine: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INC)

$(ODIR)/%.o: $(SDIR)/%.cpp 
	$(CXX) $(CXXFLAGS) -c $(INC) -o $@ $<

depend: .depend

.depend: $(OBJS)
	$(RM) ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(ODIR)/*
	$(RM) bin/*
	@rm -f regex_engine_ref

.PHONY: clean

include .depend
