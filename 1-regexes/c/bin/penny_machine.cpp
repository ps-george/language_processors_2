
/*
1. Put a penny into the machine
2. If there are any blank arrows, create a penny at the destination of the arrow
3. Whenever the machine reads an input character c, move all the pennies. If a
penny
is on a node with an outgoing arrow labeled c, move the penny along the arrow.
If
there is more than one such arrow, the penny clones itself and follows both
arrows. If a
penny is on a node with no arrow labeled c, remove the penny.
4.  When the input is all read, machine return 'Yes' if there are any pennys on
final nodes
 */

#include "penny_machine.hpp"

Penny::Penny(nodePtr &ptr) : current(ptr) {}

void Penny::update(nodePtr &ptr){
  current = ptr;
}

void Penny::move(Arrow & a){
  current =a.d;
}

void Machine::start() {
  // Create a penny at the start node
  pennys.push_back(Penny(head));
}

int Machine::input_char(char c) {
  // For each penny in the machine, move all the pennys
  // If the node has no arrows, delete the penny
  int matches = 0;
  for (auto it = pennys.begin(); it != pennys.end();) {
    // For each penny, get the current node of the penny
    nodePtr tmp = (*it).current;
    // If node is blank, it's probably an end node.
    if (tmp->is_blank()) {
      matches++;
    }
    
    int matched = 0;
    // for each arrow coming out of the node, check if it is a match
    for (auto it1 = tmp->arrows.begin(); it1 != tmp->arrows.end(); ++it1) {
      if ((*it1).match(c)) {
        // If it is a match, clone the penny the other end of the arrow
        if (!matched) {
          // If there have been no matches yet, delete the penny
          it = pennys.erase(it);
        }
        pennys.insert(it, Penny((*it1).d));
        matched++;
      }
    }
    // if nothing matched in the whole for loop, delete the penny
    if (!matched) {
      it = pennys.erase(it);
    }
  }
  return matches;
}
