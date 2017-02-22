#include "ast.hpp"
#include <iostream>
#include <regex>

void ConstantPropogation(std::map<std::string,std::string>& bindings, TreePtr node, int &changed) 
{  
  std::regex reNum("^-?[0-9]+$");
  std::regex reId("^[a-z][a-z0-9]*$");
  
  /*
  std::cerr << "  Bindings: " << std::endl;
  for (const auto &p : bindings) {
    std::cerr << "    m[" << p.first << "] = " << p.second << '\n';
  }
  */
  // If a constant is assigned to a variable, replace all uses of that variable with the constant
  if ( regex_match(node->type, reId) ) {
    // if there exists a binding
    if (bindings.count(node->type)){
      // Change the node from a variable to a constant
      std::cerr << "Applying binding: " << node->type << " = " << bindings[node->type] << std::endl;
      node->type = bindings[node->type];
      changed++;
    }
  }
  
  if (node->branches.size() > 0) {
    // if the node is an assign
    if (node->type == "Assign"){
      // std::cerr << "Type: " << node->type << std::endl;
      // std::cerr << "Value: " << node->value << std::endl;
      // if the thing being assigned is a constant
      // if binding already exists, delete it.
      if (bindings.count(node->value)){
        std::cerr << "Removing binding: " << node->value << " = " << bindings[node->value] << std::endl;
        bindings.erase(node->value);
      }
      if (regex_match(node->branches.at(0)->type, reNum)){
        // Until the next assignment of the same variable, replace all future instances of that variable to the constant
        std::cerr << "Adding binding: " << node->value << " = " << node->branches.at(0)->type << std::endl;
        bindings[node->value] =  node->branches.at(0)->type;
      }
    }
    for (unsigned i = 0; i < node->branches.size(); i++) {
      ConstantPropogation(bindings, node->branches[i], changed);
    }
  }
  return;
}
