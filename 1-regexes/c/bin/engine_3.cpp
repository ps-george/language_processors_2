

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "node.hpp"

using namespace std;

int main(int argc, char* argv[]){
  string test_str = "aab";
  nodePtr head = new Node; // Points to the head of the list
  
  std::vector<char> rgx_v;
  for(char& c : test_str) {
    rgx_v.push_back(c);
  }
  
  // Construct tree
  nodePtr parent = head;
  for (int i = 0;i<rgx_v.size();++i){
    switch(rgx_v[i]){
      default:
      // TODO: If it is a-b, A-Z or 0-9
      // Create a child node.
        nodePtr child = new Node;
      // Point parent to child
        parent->set_arrow(rgx_v[i],child);
      // Set parent to child.
        parent = child;
      break;
    }
  }
  
  // Traverse tree
  parent = head;
  
  return 0;
}
