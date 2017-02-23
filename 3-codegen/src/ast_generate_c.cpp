#include "ast.hpp"

#include <regex>
#include <string>
#include <map>

void CompileC(std::ostream &dst, TreePtr program, std::map<std::string, int>& init) {
  std::regex reNum("^-?[0-9]+$");
  std::regex reId("^[a-z][a-z0-9]*$");
  
  if (regex_match(program->type, reNum)) {
    // if it's a constant, it's a constant
    dst << program->type;
    //dst << ";";
  } else if (program->type == "Param") {
    // If it's a parameter... ignore it? Not supporting parameter.
    dst << "getParam(" << program->value << ")";
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
    // Compile the right
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
    //dst << ';';

  } else if (program->type == "While") {
    dst << "while (";
    CompileC(dst,program->branches.at(0),init);
    dst << "){\n";
    CompileC(dst,program->branches.at(1),init);
    dst << ";\n}";
  } else if (program->type == "If") {
    dst << "if (";
    CompileC(dst,program->branches.at(0),init);
    dst << "){\n";
    CompileC(dst,program->branches.at(1),init);
    dst << ";\n} else {\n";

    CompileC(dst,program->branches.at(2),init);
    dst << ";\n}";
  } else if (program->type == "Output") {
    //dst << "printf(\"%s\\n\",";
    dst << "std::cout << ";
    CompileC(dst,program->branches.at(0),init);
    dst << ";";
    //dst << ")";
    //dst << ";";


  } else if (regex_match(program->type, reId)) {
    dst << program->type;
  } else {
    throw std::runtime_error("Unknown construct '" + program->type + "'");
  }
}

void GenerateC(std::ostream &dst,TreePtr program) {
  dst << "#include <iostream>\n#include <stdio.h>\n";
  dst << "int getParam(int num) { return 0; }\n";
  dst << "int getInput() { int c;\nstd::cin >> c;\n return c;\n }\n";
  dst << "int main(int argc, char* argv[]) {\n";
  std::map<std::string, int> init;
  // Compile the whole thing
  CompileC(dst,program, init);
  // Put the result out
  dst << "\nreturn 0; } "<< "\n";
}
