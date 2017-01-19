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


map<char, int> operators {
	{'\\',0},
	{'(',1},
	{')',2},
	{'+',3},
	{'.',4},
	{'|',5}
};

// If o1 has higher or equal precedance
bool lower_precedence(char o1, char o2){
	if (operators[o1]>operators[o2]) return true;	 
	return false;
}

string convert_to_postfix(string re) {
  // Create the stacks
  stack<char> opStack;
	vector<char> outQueue;
	int count = 0;
  // For each character in the string
  for (int i = 0; i < re.length(); i++) {
		char c = re[i];
		cout << c << endl;
		switch(c){
			default:
				// If the token is a character, then push it to the output queue.
				// If there is more than one of these, put a .
				if (count>1){
					count--;
					outQueue.push_back('.');
				}
				outQueue.push_back(c);
				count++;
				break;
			case '(': // If the token is a left parenthesis (i.e. "("), then push it onto the stack.
				opStack.push('(');
				break;
			case ')':
			/* Until the token at the top of the stack is a left parenthesis, pop operators off the stack onto the output queue.
					Pop the left parenthesis from the stack, but not onto the output queue.
					If the stack runs out without finding a left parenthesis, then there are mismatched parentheses. */
					while (opStack.top()!='(') {
						outQueue.push_back(opStack.top());
						opStack.pop();
					}
					opStack.pop();
				break;
			case '|':
			case '+':
				if (opStack.empty()){
					outQueue.push_back(c);
				}
				else if (!lower_precedence(c,opStack.top())){
					opStack.push(c);  // If the token is an operator, if it has higher or equal precedence than the operator at the top of the opStack, push it onto the opStack
				}
				else {
					while(lower_precedence(c,opStack.top())){ 
					// If it has lower precedence than the operator at the top of the opStack pop all higher precedence operators from the opStack onto the output and push the operator onto the opStack
						outQueue.push_back(opStack.top());
						opStack.pop();
					}
				}
				break;
		}
		cout << "outQueue:";
		stack<char> clone = opStack;
		for (int j = 0; j<outQueue.size();j++){
			cout << outQueue[j];
		}
		
  }
	while (!opStack.empty()){
		outQueue.push_back(opStack.top());
		opStack.pop();
	}
	while (count > 1) {
		outQueue.push_back('.');
		count--;
	}
	std::string postfix(outQueue.begin(),outQueue.end());
	cout << endl;
	cout << postfix << endl;
  return postfix;
}

int main(int argc, char *argv[]) { 
	string tmp;
	string postfix;
	while (getline(cin,tmp)){
		postfix = convert_to_postfix(tmp);
	}
	
	return 0; 


}
