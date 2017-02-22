#include "ast.hpp"
#include <iostream>
#include <stdexcept>
#include <regex>

void ConstantFold(TreePtr node, int &changed) 
{  
  std::regex reNum("^-?[0-9]+$");
  /*
  std::cerr << "Type: " << node->type << std::endl;
  std::cerr << "Value: " << node->type << std::endl;
  std::cerr << "Num. branches: " << node->branches.size() << std::endl;
  */
 
  if (node->branches.size() > 0) {
    if (node->type == "Add" || node->type == "Sub") {
      std::string left,right;
      left = node->branches.at(0)->type; 
      right = node->branches.at(1)->type;
      if (regex_match(left, reNum) && regex_match(right, reNum)){
        int32_t value = 0;
        if (node->type == "Add"){
          value = stoi(left) + stoi(right);
        }
        else {
          value = stoi(left) - stoi(right);
        }
        changed++;
        node->type = std::to_string(value);
        node->value = "";
        node->branches.clear();
      }
    }
    for (unsigned i = 0; i < node->branches.size(); i++) {
      ConstantFold(node->branches[i], changed);
    }
  }
  
  return;
}
