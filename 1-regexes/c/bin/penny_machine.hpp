#ifndef PENNY_MACHINE_HPP_
#define PENNY_MACHINE_HPP_
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

#include <list>
#include <vector>

using namespace std;

typedef class Node *nodePtr;

struct Arrow {
  bool match(const char &c1) const;
  char c;
  nodePtr d;
};

class Node {
public:
  bool is_blank() const;
  std::vector<Arrow> arrows;
  unsigned int i;
  bool end;
};

class Penny {
public:
  Penny(nodePtr &ptr);
  int match(Node n, char c);
  void update(nodePtr &ptr);
  void move(Arrow & a);
  nodePtr current;
};

// Machine contains all the nodes and pennys
class Machine {
public:
  //! Start up the machine (place penny on the starting node)
  void start();
  //! Input a character to the machine.
  int input_char(char c);
  //! Create a node inside the machine
  void create_node();
  void move_penny();

private:
  // Points to the start of the machine
  nodePtr head;
  std::list<Penny> pennys;
};

#endif
