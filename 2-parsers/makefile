CPPFLAGS += -std=c++11 -W -Wall -g -Wno-unused-parameter
CPPFLAGS += -I include

src/maths_parser.tab.cpp src/maths_parser.tab.hpp : src/maths_parser.y
	bison -v -d src/maths_parser.y -o src/maths_parser.tab.cpp

src/maths_lexer.yy.cpp : src/maths_lexer.flex src/maths_parser.tab.hpp
	flex -o src/maths_lexer.yy.cpp  src/maths_lexer.flex

bin/print_canonical : src/print_canonical.o src/maths_parser.tab.o src/maths_lexer.yy.o src/maths_parser.tab.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/print_canonical $^
	
bin/print_shrunk : src/print_shrunk.o src/maths_parser.tab.o src/maths_lexer.yy.o src/maths_parser.tab.o
		mkdir -p bin
		g++ $(CPPFLAGS) -o bin/print_shrunk $^
	
bin/eval_expr : src/eval_expr.o src/maths_parser.tab.o src/maths_lexer.yy.o src/maths_parser.tab.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/eval_expr $^

bin/diff_expr: src/diff_expr.o src/maths_parser.tab.o src/maths_lexer.yy.o src/maths_parser.tab.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/diff_expr $^

clean :
	rm -f src/*.o
	rm -f src/*.tab.*
	rm -f bin/*
	rm -f src/*.tab.cpp
	rm -f src/*.yy.cpp
