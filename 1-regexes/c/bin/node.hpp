#ifndef NODE_HPP_ 
#define NODE_HPP_

#include <list>
#include <vector>
#include <stack>
#include <iostream>

using namespace std;

typedef class Node *nodePtr;

class Arrow {
public:
	Arrow(nodePtr n1, char c1, bool e) : n(n1),c(c1),epsilon(e){};
  bool match(const char &c1) const;
  nodePtr n;
  char c;
  bool epsilon;
};

class Node {
public:
	Node();
	Node(bool final);
	void create_arrow(nodePtr n, char c, bool e);
	bool is_blank() const;
	std::vector<Arrow> arrows;
	unsigned int size;
	bool end;
};

Node::Node(){
	end = false;
	size = 0;
}

Node::Node(bool final){
	end = final;
	size = 0;
}

void Node::create_arrow(nodePtr n, char c, bool e){
	arrows.push_back(Arrow(n,c,e));
	size++;
}

// A fragment consists of a start state and a list of end states
class Frag {
public:
	Frag::Frag(char c);
	Frag create_frag(char c);
	nodePtr create_end_node();
	nodePtr start;
	std::vector<Node> endNodes;
	unsigned int i;
};

nodePtr Frag::create_end_node(){
	Node end(true);
	endNodes.push_back(end);
	i++;
	return &(endNodes[i-1]);
}

Frag::Frag(char c){
	i = 0;
	switch(c){
	case '+':

		break;
	default: // a character
		// Create the start node
		start = new Node();
		// Create the end node
		nodePtr endPtr = create_end_node();
		// Create an arrow between the two, using the character
		start->create_arrow(endPtr, c, false);
		break;
	}
}

//
int main(){
	// Create a stack of fragments
	std::stack<Frag> fragments;
	// Read a character
	char c = 'a';
	// Create a fragment from the character
	Frag tmp_frag(c);
	// Push the fragment onto the stack
	fragments.push(tmp_frag);
	// read another character
	c = 'b';
	Frag tmp_frag(c);
	fragments.push(tmp_frag);
	// read another character

}



#endif
  
