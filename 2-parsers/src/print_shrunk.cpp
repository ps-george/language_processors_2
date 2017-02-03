#include "ast.hpp"

int Expression::tabcount = 0;
int Expression::changed = 0;
int main()
{
    const Expression *ast=parseAST();
    ast->print_xml();
    
    int c = 1;
    while (c){
      c = 0;
      Expression::reset_changed();
      ast = ast->shrink();
      c = ast->has_changed();
      fprintf(stderr, "Changes: %d\n", c);
    }
    
    ast->print_xml();
    std::cout<<std::endl;
    return 0;
}
