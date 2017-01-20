#ifndef RE2POSTFIX_HPP_
#define RE2POSTFIX_HPP_

#include <string>
#include <vector>
#include <stack>

// Convert infix to postfix function
std::string re2postfix(std::string re) {
  // Create the std::stacks
  std::stack<char> opStack;
  std::vector<char> outQueue;
  std::stack<std::pair<int, int>> counter;

  int num_char = 0;
  int num_alt = 0;
  // For each character in the std::string
  for (int i = 0; i < re.length(); i++) {
    char c = re[i];
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
      counter.push(std::pair<int, int>(num_alt, num_char));
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

  // exhaust all remaining binary operators from the virtual std::stack
  while ((num_char) > 1) {
    outQueue.push_back('.');
    num_char--;
  }
  while (num_alt > 0) {
    outQueue.push_back('|');
    num_alt--;
  }

  // tranform outQueue std::vector into std::string
  std::string postfix(outQueue.begin(), outQueue.end());
  return postfix;
}

#endif
