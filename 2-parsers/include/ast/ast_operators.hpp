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
    
    virtual bool is_opf() const override{
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

    virtual const Expression *differentiate(
        const std::string &variable
    ) const override
    { 
        return new AddOperator(this->getLeft()->differentiate(variable),this->getRight()->differentiate(variable));
    }
    
    virtual const Expression *shrink() const 
    {
      // std::cerr << "Shrink add" << std::endl;
      // If both sides are operators, shrink both sides, then shrink the result
      if (this->getLeft()->is_opf() && this->getRight()->is_opf()){
        return new AddOperator(this->getLeft()->shrink(), this->getRight()->shrink());
      }
      
      // If just the left is a function...
      else if (this->getLeft()->is_opf()) {
        // if the right is a number, 
        if (this->getRight()->is_number()){
          if(this->getRight()->evaluate()==0){
            change();
            return this->getLeft();
          }
        }
        return new AddOperator(this->getLeft()->shrink(), this->getRight());
        }
      
      // if the right is a function
      else if (this->getRight()->is_opf()) {
        // if the right is a number, 
        if (this->getLeft()->is_number()){
          if(this->getLeft()->evaluate()==0){
            change();
            return this->getRight();
          }
        }
        return new AddOperator(this->getLeft(), this->getRight()->shrink());
      }
      // both numbers/variables
      else {
        if (this->getLeft()->is_number() && this->getRight()->is_number()) {
          change();
          return new Number(this->getLeft()->evaluate() + getRight()->evaluate());
        }
        // Just left is a number
        else if (this->getLeft()->is_number()){
          if (this->getLeft()->evaluate()==0){
            change();
            return this->getRight();
          }
          return this;
        }
        else if (this->getRight()->is_number()){
          if(this->getRight()->evaluate()==0){
            change();
            return this->getLeft();
          } else {
          return this;
          }
        }
        // Both nvariables
        else {
          return this;
        }
      }
      return this;
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
    
    virtual const Expression *shrink() const 
    {
      // std::cerr << "Shrink sub" << std::endl;
      // If both sides are operators, shrink both sides, then shrink the result
      if (this->getLeft()->is_opf() && this->getRight()->is_opf()){
        return new SubOperator(this->getLeft()->shrink(), this->getRight()->shrink());
      }
      
      // If just the left is a function...
      else if (this->getLeft()->is_opf()) {
        // if the right is a number, 
        if (this->getRight()->is_number()){
          if(this->getRight()->evaluate()==0){
            change();
            return this->getLeft();
          }
        }
        return new SubOperator(this->getLeft()->shrink(), this->getRight());
        }
      
      // if the right is a function
      else if (this->getRight()->is_opf()) {
        return new SubOperator(this->getLeft(), this->getRight()->shrink());
      }
      // both numbers/variables
      else {
        if (this->getLeft()->is_number() && this->getRight()->is_number()) {
          change();
          return new Number(this->getLeft()->evaluate() - getRight()->evaluate());
        }
        // Just left is a number
        else if (this->getLeft()->is_number()){
          return this;
        }
        else if (this->getRight()->is_number()){
          if(this->getRight()->evaluate()==0){
            change();
            return this->getLeft();
          } else {
          return this;
          }
        }
        // Both nvariables
        else {
          return this;
        }
      }
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
    virtual double evaluate(const std::map<std::string,double> &bindings) const override {
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
    
    virtual const Expression *shrink() const 
    {
      // std::cerr << "Shrink mul" << std::endl;
      int v = 0;
      // If both sides are functions/operators, shrink them
      if ((this->getLeft()->is_opf()) && (this->getRight()->is_opf())){
        // std::cerr << "Both operators, shrink both children then shrink result" << std::endl;
        return new MulOperator(this->getLeft()->shrink(), this->getRight()->shrink());
      }
      
      // If just the left is a function...
      else if (this->getLeft()->is_opf()) {
        // std::cerr << "Only left is an operator" << std::endl;
        // if the right is a number
        if (this->getRight()->is_number()){
          v = this->getRight()->evaluate();
          // std::cerr << "Right is " << v << std::endl;
          if(v==0){
            change();
            return new Number(0);
          }
          else if (v==1){
            change();
            return this->getLeft()->shrink();
          }
        }
        // std::cerr << "Shrink left" << std::endl;
        return new MulOperator(this->getLeft()->shrink(), this->getRight());
      }
      // if the right is a function
      else if (this->getRight()->is_opf()) {
        // std::cerr << "Only right is an operator" << std::endl;
        if (this->getLeft()->is_number()){
          v = this->getLeft()->evaluate();
          // std::cerr << "Left is " << v << std::endl;
          if(v==0){
            change();
            return new Number(0);
          }
          else if (v==1){
            change();
            return this->getRight()->shrink();
          }
        }
        return new MulOperator(this->getLeft(), this->getRight()->shrink());
      }
      // Else they are both variables/numbers
      else {
        // std::cerr << "Both left and right are not operators" << std::endl;
        if (this->getLeft()->is_number() && this->getRight()->is_number()) {
          // std::cerr << "Left and right are numbers" << std::endl;
          change();
          return new Number((this->getLeft()->evaluate())*(this->getRight()->evaluate()));
        }
        else if (this->getLeft()->is_number()){
          
          v = this->getLeft()->evaluate();
          // std::cerr << "Left is " <<  v <<std::endl;
          if(v==0){
            change();
            return new Number(0);
          }
          else if (v==1) {
            change();
            return this->getRight();
          }
          else {
              // std::cerr << "No changes" << std::endl;
              return this;
           }
        }
        else if (this->getRight()->is_number()){
          v = this->getRight()->evaluate();
          // std::cerr << "Right is " <<  v <<std::endl;
          if (v==0){
            change();
            return new Number(0);
          }
          else if (v==1) {
            change();
            return this->getLeft();
          }
          else {
            // std::cerr << "No changes" << std::endl;
            return this;
          }
        }
        // Both nvariables
        else {
          // std::cerr << "No changes" << std::endl;
          return this;
        }
      }
      // std::cerr << "No changes" << std::endl;
      return this;
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
    
    virtual const Expression *shrink() const 
    {
      int v = 0;
      // std::cerr << "Shrink Div" << std::endl;
      // If both sides are operators, shrink both sides, then shrink the result
      if (this->getLeft()->is_opf() && this->getRight()->is_opf()){
        return new DivOperator(this->getLeft()->shrink(), this->getRight()->shrink());
      }
      
      // If just the left is a function...
      else if (this->getLeft()->is_opf()) {
        // if the right is a number, 
        if (this->getRight()->is_number()){
          if(this->getRight()->evaluate()==1){
            change();
            return this->getLeft();
          }
        }
        return new DivOperator(this->getLeft()->shrink(), this->getRight());
        }
      
      // if the right is a function
      else if (this->getRight()->is_opf()) {
        if (this->getLeft()->is_number()){
          v = this->getLeft()->evaluate();
          if(v==0){
            change();
            return new Number(0);
          }
        }
        return new DivOperator(this->getLeft(), this->getRight()->shrink());
      }
      // both numbers/variables
      else {
        if (this->getLeft()->is_number() && this->getRight()->is_number()) {
          return new Number(this->getLeft()->evaluate() / getRight()->evaluate());
        }
        // Just left is a number
        else if (this->getLeft()->is_number()){
          if(this->getLeft()->evaluate()==0){
            change();
            return new Number(0);
          }
          return this;
        }
        else if (this->getRight()->is_number()){
          if(this->getRight()->evaluate()==1){
            change();
            return this->getLeft();
          } else {
          return this;
          }
        }
        // Both nvariables
        else {
          return this;
        }
      }
    } 
};

#endif
