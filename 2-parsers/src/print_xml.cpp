#include "ast.hpp"
#include "sstream"


int Expression::tabcount = 0;

int main()
{
    const Expression *ast=parseAST();
    
    std::stringstream ss;
    ss << "<?xml version=\"1.0\"?>\n";
    ss << "<Expression>\n";
    ast->print_xml();
    ss << "</Expression>\n";
    
    
    return 0;
}
