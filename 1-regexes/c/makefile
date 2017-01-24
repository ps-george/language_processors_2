CXXFLAGS += -std=c++11 -W -Wall -g

penny_machine.o: bin/pennymachine.cpp
	g++ $(CXXFLAGS) -c -o $@ $<

bin/regex_engine: bin/regex_engine.cpp bin/penny_machine.o
	g++ $(CXXFLAGS) -o $@ $^

regex_engine_ref: regex_engine_ref.cpp
		g++ $(CXXFLAGS) -o $@ $^

clean:
	-rm bin/regex_engine
	-rm regex_engine_ref
