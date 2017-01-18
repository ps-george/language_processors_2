#include "node.hpp"

Node::Node() {
  end = false;
  i = 0;
}

Node::Node(char c, nodePtr n) {
  end = false;
  i = 0;
  set_arrow(c, n);
}

// Set one of the arrows from this node to another node
void Node::set_arrow(char c, nodePtr n) {
  arrows[i] = std::pair<char, nodePtr>(c, n);
  i++;
}

// Test whether the given character matches any of the arrow chars
// returns the number of arrows it has matched
bool Node::test_match(char c) {
  unsigned int count = 0;
  for (const auto &itPair : arrows) {
    if (itPair.first == c) {
      count++;
    }
  }
  return 0;
}

bool Node::is_empty(){
  return i;
}
