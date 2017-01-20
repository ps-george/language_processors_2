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

struct Node {
  //
};

struct Frag {
  nodePtr start;       // Points to the start state
  vector<nodePtr> out; // list of pointers not connected to anything yet
};

nodePtr construct_nfa(char c) {
  nodePtr n = new Node;
  return n;
}

string convert_to_postfix(string re);

// First create the infix to postfix function
/*
class Token {
        char get_value();
};
// We will use ~
class Character : public Token {

        char c;
};
*/


int main(int argc, char *argv[]) {
  string tmp;
  string postfix;
  while (getline(cin, tmp)) {
    postfix = convert_to_postfix(tmp);
  }

  return 0;
}

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
    default:
      // If the token is a character, then push it to the output queue.
      // If there is more than one of these, put a .
      if (num_char > 1) {
        num_char--;
        outQueue.push_back('.');
      }
      outQueue.push_back(c);
      num_char++;
      break;
    case '(': // If there have been two characters before, concatenate them
		// If the token is a left parenthesis (i.e. "("), raise the level of num_alt and num_char and reset them
							
      if (num_char > 1) {
        num_char--;
        outQueue.push_back('.');
      }
      counter.push(pair<int, int>(num_alt, num_char));
      num_alt = 0;
      num_char = 0;
      break;
    case ')':
			// Dump all binary operators that are waiting to be added to output
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
			
    case '+': // For unary operators, just place them straight to the output
      outQueue.push_back(c);
      break;
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
  while ((num_char) > 0) {
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
