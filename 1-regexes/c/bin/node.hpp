#ifndef NODE_HPP_ 
#define NODE_HPP_

#include <vector>

// Node is a NFA state with a number of arrows exiting.
typedef class Node* nodePtr;
class Node {
public:
  Node();
  Node(char c, nodePtr n);
  void set_arrow(char c, nodePtr n);
  bool is_empty();
  bool test_match(char c);
  bool matched() const{ return end;}
  
private:
  std::vector<std::pair<char, nodePtr>> arrows;
  unsigned int i;
  bool end;
};

#endif
  
