#include "ast.hpp"

#include <iomanip>
#include <vector>

int main(int argc, char *argv[])
{
    std::vector<std::string> variables;
    std::string var;
    fprintf(stderr, "W.r.t:\n");
    for (int i = 1; i < argc; ++i){
      var = std::string(argv[i]);
      variables.push_back(var);
      fprintf(stderr, "%s\n", var.c_str());
    }
    
    const Expression *ast=parseAST();
    
    const Expression *diff1; const Expression *diff2;
    diff1 = ast->differentiate(variables[0]);
    
    for (int i = 0; i < variables.size(); ++i){
      fprintf(stderr, "%d\n", i);
      if (!i) continue;
      diff1 = ast->differentiate(variables[i]);
    }
    diff1->print();
    return 0;
}
