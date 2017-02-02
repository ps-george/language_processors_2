#ifndef ast_operators_hpp
#define ast_operators_hpp

// To satisfy my linter...
#include "ast/ast_expression.hpp"

#include <string>
#include <iostream>
#include <cmath>

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
    
    virtual bool is_op() const override{
      return true;
    }
    virtual void print_xml() const override
    {
        std::cout<<"<Operator id =\""<<getOpcode()<<"\">\n";
        this->tab(true);
        std::cout<<"<Left pid =\""<<getOpcode()<<"\">\n";
          this->tab(true);
          left->print_xml();
          this->tab(false);
        std::cout<<"</Left>\n";
        this->tab();
        std::cout<<"<Right pid =\""<<getOpcode()<<"\">\n";
          this->tab(true);
          right->print_xml();
          this->tab(false);
        std::cout<<"</Right>\n";
        this->tab(false);
        std::cout<<"</Operator>\n";
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
      return this->getLeft()->evaluate(bindings) + this->getRight()->evaluate(bindings);
    }
    
    virtual double evaluate() const override
    {
      return this->getLeft()->evaluate() + this->getRight()->evaluate();
    }

    virtual const Expression *differentiate(
        const std::string &variable
    ) const override
    { 
        return new AddOperator(this->getLeft()->differentiate(variable),this->getRight()->differentiate(variable));
    }
    
    virtual const Expression *shrink(
    ) const 
    {
      // If both sides are operators, shrink both sides, then shrink the result
      if (this->getLeft()->is_op() && this->getRight()->is_op()){
        return (new AddOperator(this->getLeft()->shrink(), this->getRight()->shrink()))->shrink();
      }
      
      
      // If just the left is a function...
      else if (this->getLeft()->is_op()) {
        // if the right is a number, 
        if (this->getRight()->is_number()){
          if(this->getRight()->evaluate()==0){
            return this->getLeft();
          }
          else { return (new AddOperator(this->getLeft()->shrink(), this->getRight()))->shrink();
          }
        }
      }
      // if the right is a function
      else if (this->getRight()->is_op()) {
        // if the right is a number, 
        if (this->getLeft()->is_number()){
          if(this->getLeft()->evaluate()==0){
            return this->getRight();
          } else {
            return (new AddOperator(this->getLeft(), this->getRight()->shrink()))->shrink();
          }
        }
      }
      // both numbers/variables
      else {
        if (this->getLeft()->is_number() && this->getRight()->is_number()) {
          return new Number(this->getLeft()->evaluate() + getRight()->evaluate());
        }
        else if (this->getLeft()->is_number()){
          if (this->getLeft()->evaluate()==0){
           return this->getRight();
          } else {
          return new AddOperator(this->getLeft(), this->getRight());
          }
        }
        else if (this->getRight()->is_number()){
          if(this->getRight()->evaluate()==0){
            return this->getLeft();
          } else {
          return new AddOperator(this->getLeft(), this->getRight());
          }
        }
        // Both nvariables
        else {
          return new AddOperator(this->getLeft(), this->getRight());
        }
      }
      return new AddOperator(this->getLeft(), this->getRight());
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
    
    virtual double evaluate() const override
    {
      return this->getLeft()->evaluate() - this->getRight()->evaluate();
    }
    
    virtual const Expression *differentiate(
        const std::string &variable
    ) const override
    { 
        return new SubOperator(this->getLeft()->differentiate(variable),this->getRight()->differentiate(variable));
    }
    
    virtual const Expression *shrink(
    ) const 
    {
      return this;
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
    virtual double evaluate() const override
    {
      if ((this->getLeft()->evaluate()==0)||(this->getRight()->evaluate()==0))
        return 0;
      return 1;
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
    
    virtual const Expression *shrink(
    ) const 
    {
      int v = 0;
      // If both sides are functions/operators, shrink them
      if ((this->getLeft()->is_op()) && (this->getRight()->is_op())){
        std::cerr << "Both operators, shrink both children then shrink result" << std::endl;
        return (new MulOperator(this->getLeft()->shrink(), this->getRight()->shrink()))->shrink();
      }
      
      // If just the left is a function...
      else if (this->getLeft()->is_op()) {
        std::cerr << "Only left is an operator" << std::endl;
        // if the right is a number
        if (this->getRight()->is_number()){
          v = this->getRight()->evaluate();
          std::cerr << "Right is " << v << std::endl;
          if(v==0){
            return new Number(0);
          }
          else if (v==1){
            return this->getLeft()->shrink();
          }
        }
        std::cerr << "Shrink left" << std::endl;
        return (new MulOperator(this->getLeft()->shrink(), this->getRight()))->shrink();
      }
      // if the right is a function
      else if (this->getRight()->is_op()) {
        // if the right is a number, 
        std::cerr << "Only right is an operator" << std::endl;
        if (this->getLeft()->is_number()){
          v = this->getLeft()->evaluate();
          std::cerr << "Left is " << v << std::endl;
          if(v==0){
            return new Number(0);
          }
          else if (v==1){
            return this->getRight()->shrink();
          }
        }
        return (new MulOperator(this->getLeft(), this->getRight()->shrink()))->shrink();
      }
      // Else they are both variables/numbers
      else {
        std::cerr << "Both left and right are not operators" << std::endl;
        if (this->getLeft()->is_number() && this->getRight()->is_number()) {
          std::cerr << "Left and right are numbers" << std::endl;
          return new Number((this->getLeft()->evaluate())*(this->getRight()->evaluate()));
        }
        else if (this->getLeft()->is_number()){
          
          v = this->getLeft()->evaluate();
          std::cerr << "Left is " <<  v <<std::endl;
          if(v==0){
            return new Number(0);
          }
          else if (v==1) {
            return this->getRight();
          }
          else {
              std::cerr << "No changes" << std::endl;
              return new MulOperator(this->getLeft(), this->getRight());
           }
        }
        else if (this->getRight()->is_number()){
          v = this->getRight()->evaluate();
          std::cerr << "Right is " <<  v <<std::endl;
          if (v==0){
            return new Number(0);
          }
          else if (v==1) {
            return this->getLeft();
          }
          else {
            std::cerr << "No changes" << std::endl;
            return new MulOperator(this->getLeft(), this->getRight());
          }
        }
        // Both nvariables
        else {
          std::cerr << "No changes" << std::endl;
          return new MulOperator(this->getLeft(), this->getRight());
        }
      }
      std::cerr << "No changes" << std::endl;
      return new MulOperator(this->getLeft(), this->getRight());
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
    
    virtual double evaluate() const override
    {
      return this->getLeft()->evaluate() / this->getRight()->evaluate();
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
    
    virtual const Expression *shrink(
    ) const {
      if (this->getLeft()->evaluate() == 0){
        return new Number(0);
      }
      return this;
    };   
};

#endif
