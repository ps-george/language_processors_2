#include <iostream>
#include <string>
#include <vector>
/*
The program takes as an input argument an ASCII regular expression, which can contain the following constructs:

    Character literals (but not character ranges)
    One-or-more (but not zero-or-more)
    Grouping
    Alternation
    
    
    |, a-z, A-Z, ( ), [], +
    Neither the regular expression nor input strings will contain whitespace.
*/

// TODO: Create datastructure to represent regex
struct Regex{
  Regex();
  Regex(const std::string& s);
  bool match(const std::string& s) const;
  
      
};

struct Node{
  int c;
  Node *next;
  Node *next1;
  char valid;
};

// TODO: Write match function which takes regex data structure and a string and tried to match the string

int main(int argc, char *argv[])
{
  // Decode command line args
  std::string rgx_str;
  Regex rgx;
  
  std::cout << argc - 1 << " arguments provided." <<std::endl;
  if (argc==1){
    std::cerr << "No Regex string provided." << std::endl;
    return 1;
  }
  if (argc==2){
    rgx_str = argv[1];
  }
  else {
    std::cerr << "Too many arguments." << std::endl;
    return 1;
  }
  // Parse command line string into vector of expressions/operators
  std::string tmp;
  std::vector<char> rgx_v;
  for(char& c : rgx_str) {
    tmp.push_back(c);
  }
  
  // Iterate through list and create a NFA
  for (int i = 0;i<rgx_v.size();++i){
    switch(rgx_v[i]){
      case '(':
        
      break;
      case ')':
        
      break;
      case '+':
        
      break;
      case '|':
        
      break;
      case '\\':
        
      break;
      default:
      // TODO: If it is a-b, A-Z or 0-9, create a node.
      
      
      break;
    }
  }
  // TODO: Create regex data_structure
  
  // Take input
  //std::string tmp;
  while( std::getline(std::cin, tmp) ){
    //   Apply the Regex and print out the new string
    if (rgx.match(tmp)){
      std::cout << "Match" << std::endl;
    }
    else {
      std::cout << "NoMatch" << std::endl;
    }
  }
  
  return 0;
}
