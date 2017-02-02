#include "ast.hpp"

int Expression::tabcount = 0;

int main()
{
    const Expression *ast=parseAST();
    ast->print_xml();
    const Expression *diff = ast->shrink();
    
    
    std::cout<<std::endl;
    
    diff->print_xml();
    std::cout<<std::endl;
    return 0;
}
