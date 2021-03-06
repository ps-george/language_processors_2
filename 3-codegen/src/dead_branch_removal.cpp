#include "ast.hpp"

#include <cstdlib>

int main(int argc, char *argv[])
{
    TreePtr src=Parse(std::cin);
    
    // Constant fold
    int changed = 0;
    DeadBranchRemoval(src, changed);
    
    PrettyPrint(std::cout, src);
    std::cerr << "Dead branches removed: " << changed << std::endl;
    if (changed>0){
      return 0;
    }
    return 1;
}
