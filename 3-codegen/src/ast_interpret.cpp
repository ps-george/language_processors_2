#include "ast.hpp"

#include <regex>

int32_t Interpret(
    InterpretContext &context, // Contains the parameters and variable bindings
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    //std::cerr << program->type << std::endl ;
    //! Primitive returns constant value
    if( regex_match(program->type, reNum) ){
        return std::atol(program->type.c_str());
      
    //! Primitive returns value in command-line arg parameters
    }else if(program->type=="Param"){
        unsigned index=atol(program->value.c_str());
        auto value=context.params.at(index);
        return value;
    
    //! Prints out the value of what it points to stdout
    }else if(program->type=="Output"){
        int32_t val=Interpret(context, program->branches.at(0));
        std::cout<<val<<std::endl;
        return val;
        
    // Handle other constructs
    //! Input is cin
    }else if (program->type=="Input"){
      int val;
      std::cin >> val;
      return val;
      
    //! Add adds left and right
    }else if (program->type=="Add"){
      return Interpret(context, program->branches.at(0)) 
             + Interpret(context, program->branches.at(1));
    
    //! Sub does left - right   
    }else if (program->type=="Sub"){
      return Interpret(context, program->branches.at(0)) 
             - Interpret(context, program->branches.at(1));
    
    //! LessThan does left > right, returns 1 if true else 0
    }else if (program->type=="LessThan"){
      return Interpret(context, program->branches.at(0)) 
             < Interpret(context, program->branches.at(1));
    //! Assign creates binding for variable          
    }else if (program->type=="Assign"){
      int val = Interpret(context, program->branches.at(0));
      std::cerr << program->value << std::endl;
      context.bindings[program->value] = val;
      return val;
    //! Contains a sequence of instructions
    }else if (program->type=="Seq"){
      int val = 0;
      for (auto &it : program->branches){
        val = Interpret(context,it);
      }
      return val;
      
    }else if (program->type=="While"){
      int val = 0;
      while (Interpret(context, program->branches.at(0))){
        val = Interpret(context, program->branches.at(1));
      }
      return val;
      
    }else if (program->type=="If"){
      if (Interpret(context, program->branches.at(0))){
         return Interpret(context, program->branches.at(1));
      }
      else {
        return Interpret(context, program->branches.at(2));
      }
    // Check for things matching reId
    //! Primitive returns value in bindings
    }else if( regex_match(program->type, reId) ){
      std::cerr << " id=" << program->type;
      auto value=context.bindings.at(program->type);
      return value;
    }else{
      throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}
