#include "penny_machine.hpp"
#include <iostream>
#include <stack>
#include <string>

using namespace std;

nodePtr construct_penny_machine(string psfx) {
  stack<Frag> fragStack;
  Frag f1, f2;
  Node tmp;
  for (int i = 0; i < psfx.length(); i++) {
    cout << i << "th char ";
    char c = psfx[i];
    cout << c << endl;
    switch (c) {
    // Default, token is a character. Create a Char node and push onto the stack
    default:
      // Create a start node, pointing to nothing
      tmp = Node({Arrow({c, NULL})});
      // Create a fragment with start = &tmp and added ptr to arrow to the list
      fragStack.push(Frag({&tmp, list1(&tmp.a1)}));
      
      break;
    // Token is a concatentation token
    // Pop the top two items from the stack and join them together
    case '.':
      f1 = fragStack.top();
      fragStack.pop();
      f2 = fragStack.top();
      fragStack.pop();
      // Connect all the arrows in f1.alist to f2.start
      route(f1.alist, f2.start);
      fragStack.push(Frag({f1.start,f2.alist}));
      break;
    // Token is a Kleene Cross
    // Pop the first item off the stack, create a Node pointing to the start of
    // the fragment and point all the arrows of the fragment to that node
    case '+':
      f1 = fragStack.top();
      fragStack.pop();
      // For now, we will use space as epsilon, and deal with it later.
      tmp = Node({Arrow({' ', f1.start}), Arrow({' ', NULL})});
      route(f1.alist, &tmp);
      fragStack.push(Frag({f1.start, list1(&tmp.a2)}));
      break;
    // Token is an Alternation
    // pop the top two items from the stack
    // create a new node pointing to each of the popped off fragments
    // push a new fragment with all the outwards arrows of the two previoius
    // fragments combined
    case '|':
      f1 = fragStack.top();
      fragStack.pop();
      f2 = fragStack.top();
      fragStack.pop();
      tmp = Node({Arrow({' ', f1.start}), Arrow({' ', f2.start})});
      fragStack.push(Frag({&tmp, join(f1.alist, f2.alist)}));
    }
  }
  f1 = fragStack.top();
  // Connect all unconnected arrows to a matching state
  Node match = Node({.end=true});
  route(f1.alist,&match);
  // Node match = Node({Match});
  // f.route(&match);
  return f1.start;
}
