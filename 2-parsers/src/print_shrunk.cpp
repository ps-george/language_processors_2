#include "ast.hpp"

int main()
{
    const Expression *ast=parseAST();
    const Expression *diff = ast->shrink();
    
    ast->print();
    std::cout<<std::endl;
    
    diff = diff->shrink();
    diff->print();
    std::cout<<std::endl;
    return 0;
}
