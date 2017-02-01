#include "ast.hpp"

#include <iomanip>
#include <vector>

int main(int argc, char *argv[])
{
    std::vector<std::string> variables;
    std::string var;
    for (int i = 1; i < argc; ++i){
      var = std::string(argv[i]);
      variables.push_back(var);
    }
    
    const Expression *ast=parseAST();
    
    const Expression *diff1; const Expression *diff2;
    diff1 = ast->differentiate(variables[0]);
    
    for (int i = 1; i < variables.size(); ++i){
      
      diff1 = diff1->differentiate(variables[i]);
    }
    diff1->print();
    fprintf(stdout, "\n");
    return 0;
}
