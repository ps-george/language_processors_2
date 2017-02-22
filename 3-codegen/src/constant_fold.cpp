#include "ast.hpp"

#include <cstdlib>

int main(int argc, char *argv[])
{
    TreePtr src=Parse(std::cin);
    
    // Constant fold
    int changed = 0;
    ConstantFold(src, changed);
    
    PrettyPrint(std::cout, src);
    std::cerr << "Constants folded: " << changed << std::endl;
    
    if (changed>0){
      return 0;
    }
    return 1;
}
