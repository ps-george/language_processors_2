#include "ast.hpp"

int Expression::tabcount = 0;
int Expression::changed = 0;

int main()
{
    const Expression *ast=parseAST();
    
    ast->print();
    std::cout<<std::endl;
    return 0;
}
