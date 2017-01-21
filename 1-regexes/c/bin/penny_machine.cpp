
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
#include "re2postfix.hpp"
#include <iostream>
#include <stack>

using namespace std;

Penny::Penny(nodePtr &ptr) : current(ptr) {}

bool Arrow::traversable(const char &c1) const{
  if (c==c1 | c==' ') return true;
  return false;
}

Machine::Machine(string psfx) {
  loopcount = 0;
  stack<Frag> fragStack;
  Frag f1, f2, tmpf;
  Node* tmp;
  for (int i = 0; i < psfx.length(); i++) {
    char c = psfx[i];
    switch (c) {
    // Default, token is a character. Create a Char node and push onto the stack
    default:
      // Create a start node, pointing to nothing
      tmp = new Node;
      tmp->a1 = Arrow({c, NULL});
      tmp->num = 1;
      // Create a fragment with start = &tmp and added ptr to arrow to the list
      // tmpf
      fragStack.push(Frag({tmp,list1(&(tmp->a1))}));
      break;
    // Token is a concatentation token
    // Pop the top two items from the stack and join them together
    case '.':
      f2 = fragStack.top();
      fragStack.pop();
      f1 = fragStack.top();
      fragStack.pop();
      // Connect all the arrows in f1.alist to f2.start
      route(f1.alist, f2.start);
      fragStack.push(Frag({f1.start, f2.alist}));
      break;
    // Token is a Kleene Cross
    // Pop the first item off the stack, create a Node pointing to the start of
    // the fragment and point all the arrows of the fragment to that node
    case '+':
      f1 = fragStack.top();
      fragStack.pop();
      // For now, we will use space as epsilon, and deal with it later.
      // Very difficult to set the char of the second arrow!
      tmp = new Node({Arrow({' ', NULL}),Arrow({' ', f1.start}), 2});
      route(f1.alist, tmp);
      fragStack.push(Frag({f1.start, list1(&(tmp->a1))}));
      break;
    // Token is an Alternation
    // pop the top two items from the stack
    // create a new node pointing to each of the popped off fragments
    // push a new fragment with all the outwards arrows of the two previoius
    // fragments combine 
    case '|':
      f2 = fragStack.top();
      fragStack.pop();
      f1 = fragStack.top();
      fragStack.pop();
      tmp = new Node({Arrow({' ', f1.start}),Arrow({' ', f2.start}),2});
      fragStack.push(Frag({tmp, join(f1.alist, f2.alist)}));
    }
  }
  f1 = fragStack.top();
  fragStack.pop();
  // Connect all unconnected arrows to a matching state
  // Why do we only have 1 final state?
  Node *match = new Node({});
  match->end = true;
  route(f1.alist, match);
  slot = f1.start;
}

void Machine::move_epsilon(){
  // For each penny in the list
  loopcount++;
  int erased = 0;
  for (auto it1 = pennys.begin(); it1 != pennys.end();) {
    // Get its current node
    erased = 0;
    nodePtr tmp = (*it1).current;
    if (tmp->num>0) {
      // If the arrows are epsilon arrows and don't point to themselves, move penny
      if (tmp->a1.epsilon()){
        if (!( (tmp->a1).target == (*it1).current )){
          pennys.push_back(Penny((tmp->a1).target));
          // Delete the penny?
          it1 = pennys.erase(it1);
          erased++;
        }
      }
      if (tmp->num==2){
        if (!( (tmp->a2).target == (*it1).current )){
          pennys.push_back(Penny((tmp->a2).target));
          // Delete the penny?
          if (!erased) {
            it1 = pennys.erase(it1);
          }
        }
      }
    }
    ++it1;
  }
  // Only go once, but add cloned pennys to the end of the list
}
void Machine::start() {
  // destroy all pennys
  pennys.clear();
  loopcount = 0;
  // Create a penny at the start node
  // pennys.push_back(Penny(slot));
  // Move all pennys until none of the pennys are on epsilons. (keep going until moved = 0)
  // this->move_epsilon();
}

int Machine::input_char(char c) {
  // For each penny in the machine, move all the pennys
  int matches = 0;
  int traversed = 0;
  int size = pennys.size();
  // Place a penny at the start
  cout << "There are " << size << " pennys in the machine.";
  pennys.push_back(Penny(slot));
  cout << " Putting one in and navigating epsilons..." << endl;
  move_epsilon();
  cout << "There are " << pennys.size() << " pennys after navigating epsilons." << endl;
  
  for (auto it = pennys.begin(); it != pennys.end();) {
    loopcount++;
    // For each penny, get the current node of the penny
    nodePtr tmp = (*it).current;
    
    // If the node has 0 arrows, delete the penny and go to next penny
    if (((*tmp).num) == 0){
        it = pennys.erase(it);
        continue;
    }
    
    cout << "Penny at node: ";
    cout << "Node has " << tmp->num << " arrows.";
    if (tmp->num==1) {
      cout << " Arrow wants: " << tmp->a1.c << endl;
    }
    else if (tmp->num==2) {
      cout << "First arrow wants: " << tmp->a1.c << endl;
      cout << "Second arrow wants: " << tmp->a2.c << endl;
    }
    
    // if there are no arrows on the node, erase the penny
    traversed = 0;
    // for each arrow coming out of the node, check if it is traversable
    if (tmp->a1.traversable(c)){
      // If node is match, note it down
      if ((*(tmp->a1).target).is_match()){
        matches++;
      }
      // Erase penny
      it = pennys.erase(it);
      // Clone penny at target
      pennys.insert(it, Penny((tmp->a1).target));
      traversed++;
    }
    if (tmp->num==2){
      if (tmp->a2.traversable(c)){
        if ((*(tmp->a2).target).is_match()){
          matches++;
        }
        // If the previous arrow wasn't traversed, erase this penny
        if (!traversed)
          it = pennys.erase(it);
        // Clone penny at target, it points to penny after
        pennys.insert(it, Penny((tmp->a2).target));
        traversed++;
      }
    }
    // if nothing matched, delete the penny
    if (!traversed) {
      it = pennys.erase(it);
    }
    cout << "Pennys moved = " << traversed << endl;
  }
  if (traversed){
    move_epsilon();
  }
  return matches;
}

void Machine::reset(){
  // Clear all pennys
  pennys.clear();
  //
  
  
}

int main(){
  string re;
  string postfix;
  string in;
  int total = 0;
  // Get the regular expression
  getline(cin,re);
  // Convert it to postfix
  postfix = re2postfix(re);
  cout << postfix << endl;
  // Construct the machine
  
  Machine m(postfix);
  // Each time there's a new line
  int matches = 0;
  while (getline(cin, in)) {
    total = 0;
    m.start();
    for (int i = 0; i<in.length(); i++){
      matches = 0;
      cout << i+1 << "th character: " << in[i] << endl;
      matches = m.input_char(in[i]);
      total += matches;
      cout << "Matches: " << matches << " Total Matches thus far: " << total << endl;
    }
    if (total>0) cout << "Match" << endl;
    else cout << "NoMatch" << endl;
    cout << "Total loops: " << m.get_loopcount() << endl;
    
  }
  
}
