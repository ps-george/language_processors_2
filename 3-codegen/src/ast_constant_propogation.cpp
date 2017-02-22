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
      std::cerr << "    Apply binding:  " << node->type << " = " << bindings[node->type] << std::endl;
      node->type = bindings[node->type];
      changed++;
    }
  }
  
  if (node->branches.size() > 0) {
    if (node->type == "If"){
      TreePtr stat1 = node->branches.at(1);
      TreePtr stat2 = node->branches.at(2);
      // If either of the if statements are an assign
      // remove the bindings for the things being assign
      if ((stat1->type == "Assign")||(stat2->type == "Assign")){
        if (stat1->type == "Assign") {
          if (bindings.count(stat1->value)){
            std::cerr << "    Remove binding: " << stat1->value << " = " << bindings[stat1->value] << std::endl;
            bindings.erase(stat1->value);
            ConstantPropogation(bindings, stat2, changed);
          }
        }
        else if (stat2->type == "Assign") {
          if (bindings.count(stat2->value)){
            std::cerr << "    Remove binding: " << stat2->value << " = " << bindings[stat2->value] << std::endl;
            bindings.erase(stat2->value);
            ConstantPropogation(bindings, stat1, changed);
          }
        }
        else {
          ConstantPropogation(bindings, node->branches.at(0), changed);
          return;
        }
        ConstantPropogation(bindings, node->branches.at(0), changed);
        return;
      }
    }
    
    // if node is a While, similar behaviour to if it is an if
    if (node->type == "While") {
      TreePtr stat = node->branches.at(1);
      if ((stat->type == "Assign")){
        if (bindings.count(stat->value)){
          std::cerr << "    Remove binding: " << stat->value << " = " << bindings[stat->value] << std::endl;
          bindings.erase(stat->value);
        }
        ConstantPropogation(bindings, node->branches.at(0), changed);
        return;
      }
    }
    
    // if the node is an assign
    if (node->type == "Assign"){
      // std::cerr << "Type: " << node->type << std::endl;
      // std::cerr << "Value: " << node->value << std::endl;
      // if the thing being assigned is a constant
      // if binding already exists, delete it.
      if (bindings.count(node->value)){
        std::cerr << "    Remove binding: " << node->value << " = " << bindings[node->value] << std::endl;
        bindings.erase(node->value);
      }
      if (regex_match(node->branches.at(0)->type, reNum)){
        // Until the next assignment of the same variable, replace all future instances of that variable to the constant
        std::cerr << "    Adding binding: " << node->value << " = " << node->branches.at(0)->type << std::endl;
        bindings[node->value] =  node->branches.at(0)->type;
      }
    }
    for (unsigned i = 0; i < node->branches.size(); i++) {
      ConstantPropogation(bindings, node->branches[i], changed);
    }
  }
  return;
}
