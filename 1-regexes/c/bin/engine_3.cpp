#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

using namespace std;

/*
The program takes as an input argument an ASCII regular expression, which can
contain the following constructs:
    Character literals (but not character ranges)
    One-or-more (but not zero-or-more)
    Grouping
    Alternation
 */

typedef struct Node *nodePtr;

enum nodeType { Match = 0, Char = 1, Cross = 2, Concat = 3, Altern = 4 };

struct Node {
  nodeType type;
  char c;
  nodePtr arrow;
  nodePtr arrow1;
};

struct Frag {
  nodePtr start; // Points to the start state
  vector<nodePtr *>
      arrow_list; // list of out pointers not connected to anything yet
  Frag join(Frag &f2);
  void route(Node *n);
};

vector<nodePtr *> arrow_list(nodePtr *in) {
  vector<nodePtr *> v;
  v.push_back(in);
  return v;
}

// Take in two arrow lists and join them together
void join_arrow_lists(vector<nodePtr *> &in1, const vector<nodePtr *> &in2) {
  for (int i = 0; i < in2.size(); i++) {
    in1.push_back(in2[i]);
  }
}

// For each dangling arrow in frag 1, connect it to the start of frag 2
// Each pointer in this list points to a pointer
// Now, point each of the arrows in this list to the start of the second
// fragment
Frag Frag::join(Frag &f2) {
  for (int i = 0; i < arrow_list.size(); i++) {
    *arrow_list[i] = f2.start;
  }
  // New fragment inherits arrow list from second fragment
  arrow_list = f2.arrow_list;
  return *this;
}

void Frag::route(Node *n) {
  // Point all of the arrows in the fragment to the specified state
  for (int i = 0; i < arrow_list.size(); i++) {
    *arrow_list[i] = n;
  }
}

string convert_to_postfix(string re);

nodePtr construct_nfa(string psfx) {
  stack<Frag> fragStack;
  Frag f, f1;
  Node tmp;
  for (int i = 0; i < psfx.length(); i++) {
    cout << i << "th char ";
    char c = psfx[i];
    cout << c << endl;
    switch (c) {
    // Default, token is a character. Create a Char node and push onto the stack
    default:
      tmp = Node({Char, c, nullptr, nullptr});
      fragStack.push(Frag({&tmp, arrow_list(&tmp.arrow)}));
      break;
    // Token is a concatentation token
    // Pop the top two items from the stack and join them together
    case '.':
      f1 = fragStack.top();
      fragStack.pop();
      f = fragStack.top();
      fragStack.pop();
      fragStack.push(f.join(f1));
      break;
    // Token is a Kleene Cross
    // Pop the first item off the stack, create a Node pointing to the start of
    // the fragment and point all the arrows of the fragment to that node
    case '+':
      f = fragStack.top();
      fragStack.pop();
      tmp = Node({Cross, c, f.start, nullptr});
      // connect f out list to tmp
      f.route(&tmp);
      fragStack.push(Frag({f.start, arrow_list(&tmp.arrow1)}));
      break;
    // Token is an Alternation
    // pop the top two items from the stack
    // create a new node pointing to each of the popped off fragments
    // push a new fragment with all the outwards arrows of the two previoius
    // fragments combined
    case '|':
      f1 = fragStack.top();
      fragStack.pop();
      f = fragStack.top();
      fragStack.pop();
      tmp = Node({Altern, c, f.start, f1.start});
      join_arrow_lists(f.arrow_list, f1.arrow_list);
      fragStack.push(Frag({&tmp, f.arrow_list}));
    }
  }
  f = fragStack.top();
  // Connect all unconnected arrows to a matching state
  Node match = Node({Match});
  f.route(&match);
  return f.start;
}

int main(int argc, char *argv[]) {
  string re;
  string postfix;
  nodePtr start;
  while (getline(cin, re)) {
    postfix = convert_to_postfix(re);
    start = construct_nfa(postfix);
  }

  return 0;
}

// First create the infix to postfix function
string convert_to_postfix(string re) {
  // Create the stacks
  stack<char> opStack;
  vector<char> outQueue;
  stack<pair<int, int>> counter;

  int num_char = 0;
  int num_alt = 0;
  // For each character in the string
  for (int i = 0; i < re.length(); i++) {
    char c = re[i];
    cout << c << endl;
    switch (c) {
    // If the token is a character, then push it to the output queue.
    // If there is more than one of these, put a concat
    default:
      if (num_char > 1) {
        num_char--;
        outQueue.push_back('.');
      }
      outQueue.push_back(c);
      num_char++;
      break;

    // If there have been two characters before, concatenate them
    // If the token is a left parenthesis (i.e. "("), raise the level of
    // num_alt and num_char and reset them
    case '(':

      if (num_char > 1) {
        num_char--;
        outQueue.push_back('.');
      }
      counter.push(pair<int, int>(num_alt, num_char));
      num_alt = 0;
      num_char = 0;
      break;

    // Dump all binary operators that are waiting to be added to output
    case ')':
      while (num_char > 1) {
        num_char--;
        outQueue.push_back('.');
      }
      while (num_alt > 0) {
        num_alt--;
        outQueue.push_back('|');
      }

      // Retreive previous values of num_alt and num_char before open brackets
      num_alt = counter.top().first;
      num_char = counter.top().second;
      counter.pop();
      num_char++;
      break;
    // For unary operators, just place them straight to the output
    case '+':
      outQueue.push_back(c);
      break;
    // For binary operators, push concats while there are concatentations to be
    // made and increment the number of alts to add at the end
    case '|':
      while (num_char > 1) {
        num_char--;
        outQueue.push_back('.');
      }
      num_alt++;
      break;
    }
  }

  // exhaust all remaining binary operators from the virtual stack
  while ((num_char) > 1) {
    outQueue.push_back('.');
    num_char--;
  }
  while (num_alt > 0) {
    outQueue.push_back('|');
    num_alt--;
  }

  // tranform outQueue vector into string
  std::string postfix(outQueue.begin(), outQueue.end());
  cout << postfix << endl;
  return postfix;
}
