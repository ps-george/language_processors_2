#ifndef ast_functions_hpp
#define ast_functions_hpp

#include "ast_expression.hpp"
#include "ast_operators.hpp"

#include <cmath>

class Function
    : public Expression
{
private:
    const Expression *arg;
protected:
    Function(const Expression *_arg)
        : arg(_arg)
    {}
public:

    virtual ~Function()
    {
        delete arg;
    }

    virtual const char * getFunction() const =0;

    const Expression *getArg() const
    { return arg; }

    virtual bool is_opf() const override{
      return true;
    }
    
    virtual void print() const override
    {
        std::cout<<getFunction()<<"( ";
        arg->print();
        std::cout<<" )";
    }
    
    virtual void print_xml() const override
    {
        std::cout<<"<Function id=\""<<getFunction()<<"\">\n";
          this->tab(true);
            arg->print_xml();
          this->tab(false);
        std::cout<<"</Function>\n";
    }
};

class LogFunction
    : public Function
{
public:
    LogFunction(const Expression *_arg)
        : Function(_arg)
    {}

    virtual const char *getFunction() const override
    { return "log"; }
    
    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
      return log(this->getArg()->evaluate(bindings));
    }
    
    virtual double evaluate() const override
    {
      return log(this->getArg()->evaluate());
    }
    
    virtual const Expression *differentiate(
        const std::string &variable
    ) const override
    { 
        return new DivOperator(this->getArg()->differentiate(variable), this->getArg());
    }
    
    virtual const Expression *shrink() const {
      if (this->getArg()->is_opf()){
        return new LogFunction(this->getArg()->shrink());
      }
      else if (this->getArg()->is_number()){
        if (this->getArg()->evaluate()==1){
          change();
          return new Number(0);
        }
      }
      return this;
    };
};

class ExpFunction
    : public Function
{
public:
    ExpFunction(const Expression *_arg)
        : Function(_arg)
    {}

    virtual const char *getFunction() const override
    { return "exp"; }
    
    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
      return exp(this->getArg()->evaluate(bindings));
    }
    
    virtual double evaluate() const override
    {
      return exp(this->getArg()->evaluate());
    }
    
    virtual const Expression *differentiate(
        const std::string &variable
    ) const override
    { 
        return new MulOperator(this->getArg()->differentiate(variable), new ExpFunction(this->getArg()));
    }
    
    virtual const Expression *shrink() const {
      if (this->getArg()->is_opf()){
        return new ExpFunction(this->getArg()->shrink());
      }
      else if (this->getArg()->is_number()){
        if (this->getArg()->evaluate()==0){
          change();
          return new Number(1);
        }
      }
      return this;
    };  
};

class SqrtFunction
    : public Function
{
public:
    SqrtFunction(const Expression *_arg)
        : Function(_arg)
    {}

    virtual const char *getFunction() const override
    { return "sqrt"; }
    
    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
      return sqrt(this->getArg()->evaluate(bindings));
    }
    
    virtual double evaluate() const override
    {
      return sqrt(this->getArg()->evaluate());
    }
    virtual const Expression *differentiate(
        const std::string &variable
    ) const override
    { 
        return new DivOperator(new MulOperator(new Number(0.5),this->getArg()->differentiate(variable)), new SqrtFunction(this->getArg()));
    }
    
    virtual const Expression *shrink() const {
      int v = 0;
      if (this->getArg()->is_opf()){
        return new SqrtFunction(this->getArg()->shrink());
      }
      else if (this->getArg()->is_number()){
        v = this->getArg()->evaluate();
        // If sqrt is a whole number
        int s = sqrt(v);
        if ((s*s) == v) {
          change();
          return new Number(sqrt(v));
        }
      }
      return this;
    };   
};


#endif
