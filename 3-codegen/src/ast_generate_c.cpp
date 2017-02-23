#include "ast.hpp"

#include <regex>
#include <string>

void CompileC(std::ostream &dst, TreePtr program) {
  std::regex reNum("^-?[0-9]+$");
  std::regex reId("^[a-z][a-z0-9]*$");

  if (regex_match(program->type, reNum)) {
    // if it's a constant, it's a constant
    dst << program->type;

  } else if (program->type == "Param") {
    // If it's a parameter... ignore it? Not supporting parameter.
    dst << "getParam(" << program->value << ")";
  } else if (program->type == "Seq") {
    // If it's a sequence, print
    for (unsigned i = 0; i < program->branches.size(); i++) {
      CompileC(dst,program->branches[i]);
      dst << ";\n";
    }

  } else if (program->type == "Input") {
    // Ignore
    dst << "getInput()";
  } else if (program->type == "Add") {
    // Compile the left
    CompileC(dst,program->branches.at(0));
    dst << "+";
    CompileC(dst,program->branches.at(1));

  } else if (program->type == "LessThan") {
    // Compile the left
    CompileC(dst,program->branches.at(0));
    dst << "<";
    // Compile the right
    CompileC(dst,program->branches.at(1));

  } else if (program->type == "Sub") {
    // Compile the left
    CompileC(dst,program->branches.at(0));
    dst << "-";
    // Compile the right
    CompileC(dst,program->branches.at(1));

  } else if (program->type == "Assign") {
    // Assign the value of the assign branch to destReg
    dst << "int " << program->value << " = ";
    CompileC(dst,program->branches.at(0));
    //dst << ';';

  } else if (program->type == "While") {
    dst << "while (";
    CompileC(dst,program->branches.at(0));
    dst << "){\n";
    // else do the rest of the code
    CompileC(dst,program->branches.at(1));
    dst << "\n}";
  } else if (program->type == "If") {
    dst << "if (";
    CompileC(dst,program->branches.at(0));
    dst << "){\n";
    CompileC(dst,program->branches.at(1));
    dst << "\n} else {\n";

    CompileC(dst,program->branches.at(2));
    dst << "\n}";
  } else if (program->type == "Output") {
    dst << "printf(\"%s\\n\",";
    CompileC(dst,program->branches.at(0));
    dst << ")";
    //dst << ";";


  } else if (regex_match(program->type, reId)) {
    dst << program->type;
  } else {
    throw std::runtime_error("Unknown construct '" + program->type + "'");
  }
}

void GenerateC(std::ostream &dst,TreePtr program) {
  dst << "int getParam(int num) { return 0; }\n";
  dst << "int main(int argc, char* argv[]) {\n";
  // Compile the whole thing
  CompileC(dst,program);
  // Put the result out
  dst << "\nreturn 0; } "<< "\n";
}
