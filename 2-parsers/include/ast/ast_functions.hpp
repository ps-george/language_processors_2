#ifndef ast_functions_hpp
#define ast_functions_hpp

#include "ast_expression.hpp"

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

    virtual void print() const override
    {
        std::cout<<getFunction()<<"( ";
        arg->print();
        std::cout<<" )";
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
};


#endif
