#include "ast.hpp"

#include <iomanip>
#include <vector>

int Expression::tabcount = 0;
int Expression::changed = 0;

int main(int argc, char *argv[])
{
    std::vector<std::string> variables;
    std::string var;
    for (int i = 1; i < argc; ++i){
      var = std::string(argv[i]);
      variables.push_back(var);
    }
    
    const Expression *diff=parseAST();
    
    for (unsigned i = 0; i < variables.size(); ++i){
      diff = diff->differentiate(variables[i]);
      
    }
    //diff->print();
    //fprintf(stdout, "\n"); // or fflush()
    
    int c = 1;
    while (c){
      c = 0;
      Expression::reset_changed();
      diff = diff->shrink();
      c = diff->has_changed();
      fprintf(stderr, "Changes: %d\n", c);
    }
    
    diff->print();
    
    fprintf(stdout, "\n");
    return 0;
}
