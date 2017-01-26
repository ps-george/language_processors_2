CXXFLAGS += -std=c++11 -W -Wall -g

penny_machine.o: bin/pennymachine.cpp
	g++ $(CXXFLAGS) -c -o $@ $<

bin/regex_engine: bin/regex_engine.cpp bin/penny_machine.o
	g++ $(CXXFLAGS) -o $@ $^

clean:
	@rm -f bin/regex_engine
	@rm -f bin/penny_machine.o
	@rm -f regex_engine_ref
