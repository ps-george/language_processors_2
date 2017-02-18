#include "ast.hpp"

#include <regex>

int32_t Interpret(
    InterpretContext &context, // Contains the parameters and variable bindings
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    //std::cerr << program->type << std::endl ;
    if( regex_match(program->type, reNum) ){
        return std::atol(program->type.c_str());
    // TODO : Check for things matching reId
    // 
    }else if( regex_match(program->type, reId) ){
      std::cerr << " id=" << program->type;
      auto value=context.bindings.at(program->type);
      return value;
    }else if(program->type=="Param"){
        unsigned index=atol(program->value.c_str());
        auto value=context.params.at(index);
        return value;
        
    }else if(program->type=="Output"){
        int32_t val=Interpret(context, program->branches.at(0));
        std::cout<<val<<std::endl;
        return val;
        
    // TODO: Handle other constructs
    
    // Input - Add to context
    }else if (program->type=="Input"){
      int val;
      std::cin >> val;
      return val;
      
    }else if (program->type=="Add"){
      return Interpret(context, program->branches.at(0)) 
             + Interpret(context, program->branches.at(1));
    
    
    }else if (program->type=="Sub"){
      return Interpret(context, program->branches.at(0)) 
             - Interpret(context, program->branches.at(1));
    
    }else if (program->type=="LessThan"){
      return Interpret(context, program->branches.at(0)) 
             < Interpret(context, program->branches.at(1));
    }else if (program->type=="Assign"){
      int val = Interpret(context, program->branches.at(0));
      std::cerr << program->value << std::endl;
      context.bindings[program->value] = val;
      return val;
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
    }else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}
