#include "ast.hpp"

void ConstantPropogation(
  std::ostream &dst, 
  TreePtr node) 
{
  // If the node has a value, print it out as a comment
  if (node->value.size() > 0) {
    dst << " : " << node->value;
  }
  
  // If a node has branches, 
  if (node->branches.size() > 0) {
    dst << " [" << std::endl;
    for (unsigned i = 0; i < node->branches.size(); i++) {
      ConstantPropogation(dst, node->branches[i]);
    }
    dst << indent << "]";
  }

  return node;
}
