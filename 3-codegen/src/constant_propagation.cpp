#include "ast.hpp"

#include <cstdlib>

int main(int argc, char *argv[]) {
  TreePtr src = Parse(std::cin);

  // Constant propagation
  int changed = 0;
  std::map<std::string, std::string> bindings;
  ConstantPropagation(bindings, src, changed);

  PrettyPrint(std::cout, src);

  std::cerr << "Constants propogated: " << changed << std::endl;
  if (changed > 0) {
    return 0;
  }
  return 1;
}
