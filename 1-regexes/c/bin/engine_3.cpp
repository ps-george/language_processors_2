#include <string>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

struct Node {
	//
};

struct Frag {
	nodePtr start; // Points to the start state
	vector<nodePtr> out; // list of pointers not connected to anything yet
};


nodePtr construct_nfa(char c){
	nodePtr n = new Node;
	return n;
}

int main(int argc, char* argv[]){


  
  return 0;
}
