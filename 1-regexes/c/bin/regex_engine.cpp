
#include <string>
#include <iostream>
#include "re2postfix.hpp"
#include "penny_machine.hpp"

using namespace std;

int main(){
  string re;
  string postfix;
  string in;
  int total = 0;
  // Get the regular expression
  getline(cin,re);
  // Convert it to postfix
  postfix = re2postfix(re);
  cerr << postfix << endl;
  // Construct the machine
  
  Machine m(postfix);
  // Each time there's a new line
  int matches = 0;
  while (getline(cin, in)) {
    // If there is no input, stop and print out statistics
    if (!in[0]) break;
    
    total = 0;
    m.start();
    for (int i = 0; i<in.length(); i++){
      matches = 0;
      // cout << i+1 << "th character: " << in[i] << endl;
      matches = m.input_char(in[i]);
      total += matches;
      // cout << "Matches: " << matches << " Total Matches thus far: " << total << endl;
    }
    // last character must be a match
    if (matches>0) cout << "Match" << endl;
    else cout << "NoMatch" << endl;
    cerr << "Total matches: " << total << endl;
    cerr << "Total loops: " << m.get_loopcount() << endl;
  }
  
  cerr << "Total loops: " << m.get_loopcount() << endl;
}
