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
    
    // If the node type is a while and condition is 0, remove the while completely
    if (node->type == "While") {
      std::string cond;
      // If the condition is a constant
      cond = node->branches.at(0)->type;
      if (regex_match(cond, reNum)){
        int32_t value = stoi(cond);
        TreePtr stat = node->branches.at(1);
        if (value==0){
          // replace this with nothing
          node->type = "";
          node->value = "";
          node->branches.clear();
          changed++;
        }
        else {
          std::cerr << "Infinite loop detected during optimisation." << std::endl;
        }
      }
    }
    for (unsigned i = 0; i < node->branches.size(); i++) {
      DeadBranchRemoval(node->branches[i], changed);
    }
  }
  
  return;
}
