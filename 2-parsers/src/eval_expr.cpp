#include "ast.hpp"

#include <iomanip>

int Expression::tabcount = 0;
int Expression::changed = 0;

int main(int argc, char *argv[])
{
    std::map<std::string,double> bindings;
    
    // TODO : for each pair of arguments:
    //  - read the variable name
    //  - parse the value
    //  - insert into the bindings map
    std::string varname;
    double val;
    fprintf(stderr, "Bindings:\n");
    for (int i = 1; i < argc - 1; i+=2){
      varname = std::string(argv[i]);
      val = atof(argv[i+1]);
      bindings[varname] = val;
      fprintf(stderr, "%s=%f\n", varname.c_str(), val);
    }
    
    const Expression *ast=parseAST();
    
    double res=ast->evaluate(bindings);
    // TODO : print out with 6 decimal digits of precision
    fprintf(stdout, "%f\n", res);
    return 0;
}
