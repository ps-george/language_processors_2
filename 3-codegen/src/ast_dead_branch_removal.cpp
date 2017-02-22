#include "ast.hpp"
#include <iostream>
#include <regex>

void DeadBranchRemoval(TreePtr node, int &changed) 
{  
  std::regex reNum("^-?[0-9]+$");
  /*
  std::cerr << "Type: " << node->type << std::endl;
  std::cerr << "Value: " << node->value << std::endl;
  std::cerr << "Num. branches: " << node->branches.size() << std::endl;
  */
  // If the node type is an if
  if (node->branches.size() > 0) {
    if (node->type == "If") {
      std::string cond;
      // If the condition is a constant
      cond = node->branches.at(0)->type;
      if (regex_match(cond, reNum)){
        int32_t value = stoi(cond);
        
        TreePtr stat1 = node->branches.at(1);
        TreePtr stat2 = node->branches.at(2);
        if (value){
          // replace this with stat1
          node->type = stat1->type;
          node->value = stat1->value;
          node->branches = stat1->branches;
        }
        else {
          // replace with stat2
          node->type = stat2->type;
          node->value = stat2->value;
          node->branches = stat2->branches;
        }
        changed++;
      }
    }
    for (unsigned i = 0; i < node->branches.size(); i++) {
      DeadBranchRemoval(node->branches[i], changed);
    }
  }
  
  return;
}
