#include "ast.hpp"

#include <regex>
#include <string>
#include <map>

extern bool cond;
bool cond = false;

extern std::string rtrn;
std::string rtrn = "0";

void CompileC(std::ostream &dst, TreePtr program, std::map<std::string, int>& init) {
  std::regex reNum("^-?[0-9]+$");
  std::regex reId("^[a-z][a-z0-9]*$");
  
  if (regex_match(program->type, reNum)) {
    // if the last thing isn't an if
    dst << program->type;
    if (!cond){
      dst << ';'; 
    }
    rtrn = program->type;  
  } else if (program->type == "Param") {
    // If it's a parameter... ignore it? Not supporting parameter.
    dst << "getParam(" << program->value << ",argv, argc)";
    
    
  } else if (program->type == "Seq") {
    // If it's a sequence, print
    for (unsigned i = 0; i < program->branches.size(); i++) {
      CompileC(dst,program->branches[i], init);
      dst << ";\n";
    }
    

  } else if (program->type == "Input") {
    // Ignore
    dst << "getInput()";
    
    
  } else if (program->type == "Add") {
    // Compile the left
    CompileC(dst,program->branches.at(0),init);
    dst << "+";
    CompileC(dst,program->branches.at(1),init);


  } else if (program->type == "LessThan") {
    // Compile the left
    CompileC(dst,program->branches.at(0),init);
    dst << "<";
    CompileC(dst,program->branches.at(1),init);
    
    
  } else if (program->type == "Sub") {
    // Compile the left
    CompileC(dst,program->branches.at(0),init);
    dst << "-";
    // Compile the right
    CompileC(dst,program->branches.at(1),init);


  } else if (program->type == "Assign") {
    // Assign the value of the assign branch to destReg
    if (!init.count(program->value)){
      dst << "int ";
      init[program->value] = 1;
    }
    dst << program->value << " = ";
    CompileC(dst,program->branches.at(0),init);


  } else if (program->type == "While") {
    dst << "while (";
    cond = true;
    CompileC(dst,program->branches.at(0),init);
    cond = false;
    dst << "){\n";
    CompileC(dst,program->branches.at(1),init);
    dst << ";\n}";
    
    
  } else if (program->type == "If") {
    dst << "if (";
    cond = true;
    CompileC(dst,program->branches.at(0),init);
    cond = false;
    dst << "){\n";
    CompileC(dst,program->branches.at(1),init);
    dst << ";\n} else {\n";

    CompileC(dst,program->branches.at(2),init);
    dst << ";\n}";
    
    
  } else if (program->type == "Output") {
    dst << "std::cout << ";
    CompileC(dst,program->branches.at(0),init);
    dst << " << std::endl;";
    //dst << " << std::endl;";
    
  } else if (regex_match(program->type, reId)) {
    // if the last thing doesn't have children, print the semi colon
    dst << program->type;
    rtrn = program->type;
  } else {
    throw std::runtime_error("Unknown construct '" + program->type + "'");
  }
}

TreePtr deepestChild(TreePtr node){
  if (node->branches.size()>0){
    deepestChild(node->branches.at((node->branches.size()-1)));
  }
  return node;
}

void GenerateC(std::ostream &dst,TreePtr program) {
  dst << "#include <iostream>\n#include <stdlib.h>\n#include <stdio.h>\n";
  dst << "int getParam(int num, char * argv[], int argc) { \nif (argc<=num+1){\nstd::cout << \"Not enough input parameters provided to access parameter \" << num+1 << std::endl;\n exit(EXIT_FAILURE);\n}\nreturn strtol(argv[num+1], NULL, 10);\n}\n";
  dst << "int getInput() { int c;\nstd::cin >> c;\n return c;\n }\n";
  dst << "int main(int argc, char* argv[]) {\n";
  std::map<std::string, int> init;
  // Compile the whole thing
  CompileC(dst, program, init);
  dst << "\nreturn " + rtrn;
  //CompileC(dst, deepestChild(last), init);
  dst << ";\n}";
  // Put the result out -> ideally want to return the last thing that happened.
  //dst << "\nreturn" << program->branches.at(program->branches.size()-1)->value << "; }\n";
  //dst << "\nreturn 0; } "<< "\n";
}
