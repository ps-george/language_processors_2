CXXFLAGS += -std=c++11 -w -Wall -g
CXXFLAGS += -I include

bin/% : src/%.cpp 
	mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	
bin/interpreter : src/ast_parse.o src/ast_pretty_print.o src/ast_interpret.o

bin/compiler : src/ast_parse.o src/ast_pretty_print.o src/ast_compile.o

bin/pretty_print : src/ast_parse.o src/ast_pretty_print.o

bin/constant_fold : src/ast_parse.o src/ast_pretty_print.o src/ast_constant_fold.o

bin/constant_propogation : src/ast_parse.o src/ast_pretty_print.o src/ast_constant_propogation.o

bin/dead_branch_removal : src/ast_parse.o src/ast_pretty_print.o src/ast_dead_branch_removal.o

bin/generate_c : src/ast_parse.o src/ast_pretty_print.o src/ast_generate_c.o

bin/generate_mips : bin/generate_c
	cp src/generate_mips.sh bin/generate_mips
	chmod +x bin/generate_mips

clean :
	-rm -f bin/*
	-rm -f src/*.o
	-rm -f working/*/*
