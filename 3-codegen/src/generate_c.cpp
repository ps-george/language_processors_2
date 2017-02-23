#include "ast.hpp"

#include <cstdlib>
// #include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    std::ofstream out;
    std::ifstream in;
    
    if (argc < 3){
      std::cerr << "Not enough arguments provided." << std::endl;
      return 1;
    }
    else if (argc == 3){
      in.open(argv[1]);
      if (!in.is_open()){
        std::cerr << "Couldn't open input file." << std::endl;
        return 1;
      }
      out.open(argv[2]);
      if (!in.is_open()){
        std::cerr << "Couldn't open output file." << std::endl;
        return 1;
      }
    }
    else {
      std::cerr << "Too many arguments provided." << std::endl;
      return 1;
    }
    TreePtr src=Parse(in);
    
    // Generate mips code instead of pretty print
    GenerateC(out, src);
    
    ///PrettyPrint(out, src);
    
    return 0;
}
