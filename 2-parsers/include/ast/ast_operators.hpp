#ifndef ast_operators_hpp
#define ast_operators_hpp

// To satisfy my linter...
#include "ast/ast_expression.hpp"

#include <string>
#include <iostream>

class Operator
    : public Expression
{
private:
    const Expression *left;
    const Expression *right;
protected:
    Operator(const Expression *_left, const Expression *_right)
        : left(_left)
        , right(_right)
    {}
public:

    virtual ~Operator()
    {
        delete left;
        delete right;
    }

    virtual const char *getOpcode() const =0;

    const Expression *getLeft() const
    { return left; }

    const Expression *getRight() const
    { return right; }

    virtual void print() const override
    {
        std::cout<<"( ";
        left->print();
        std::cout<<" ";
        std::cout<<getOpcode();
        std::cout<<" ";
        right->print();
        std::cout<<" )";
    }
    
    virtual void print_xml() const override
    {
        std::cout<<"<Operator id =\""<<<getOpcode()<<"\">\n\t";
        std::cout<<"<Left\n\t>"
          left->print();
        std::cout<<"<\n\t/Left>"
        std::cout<<getOpcode();
        std::cout<<"<Right\n\t>"
          right->print();
        std::cout<<"\n\t</Right>"
        std::cout<<"</Operator>";
    }
};

class AddOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "+"; }
public:
    AddOperator(const Expression *_left, const Expression *_right)
        : Operator(_left, _right)
    {}
      
    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
      return getLeft()->evaluate(bindings) + getRight()->evaluate(bindings);
    }

    virtual const Expression *differentiate(
        const std::string &variable
    ) const override
    { 
        return new AddOperator(this->getLeft()->differentiate(variable),this->getRight()->differentiate(variable));
    }
};

class SubOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "-"; }
public:
    SubOperator(const Expression *_left, const Expression *_right)
        : Operator(_left, _right)
    {}
    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
      return this->getLeft()->evaluate(bindings) - this->getRight()->evaluate(bindings);
    }
    
    virtual const Expression *differentiate(
        const std::string &variable
    ) const override
    { 
        return new SubOperator(this->getLeft()->differentiate(variable),this->getRight()->differentiate(variable));
    }
};

class MulOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "*"; }
public:
    MulOperator(const Expression *_left, const Expression *_right)
        : Operator(_left, _right)
    {}
    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
      return this->getLeft()->evaluate(bindings) * this->getRight()->evaluate(bindings);
    }
    
    virtual const Expression *differentiate(
        const std::string &variable
    ) const override
    { 
        return new AddOperator(
          new MulOperator(
            this->getLeft()->differentiate(variable),
            this->getRight()),
          new MulOperator(
            this->getLeft(),
            this->getRight()->differentiate(variable)));
    }   
};

class DivOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    { return "/"; }
public:
    DivOperator(const Expression *_left, const Expression *_right)
        : Operator(_left, _right)
    {}

    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
      return this->getLeft()->evaluate(bindings) / this->getRight()->evaluate(bindings);
    }   

    virtual const Expression *differentiate(
        const std::string &variable
    ) const override
    { 
        return new DivOperator(
          new SubOperator(
            new MulOperator(
              this->getLeft()->differentiate(variable),
              this->getRight()),
            new MulOperator(
              this->getRight()->differentiate(variable),
              this->getLeft())
          ),
          new MulOperator(
            this->getRight(),
            this->getRight()
          )
        )
        ;
    }   
};

#endif
