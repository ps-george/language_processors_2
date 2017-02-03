#ifndef ast_expression_hpp
#define ast_expression_hpp

#include <string>
#include <iostream>
#include <map>

class Expression
{
private:
  static int tabcount;
  static int changed;
public:
    virtual ~Expression()
    {}

    virtual void print() const =0;
    virtual void print_xml() const =0;
    //! Evaluate the tree using the given mapping of variables to numbers
    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const =0;
    
    virtual double evaluate() const =0;

    //! Return a new tree which is the derivative w.r.t. given variable
    virtual const Expression *differentiate(
        const std::string &variable
    ) const =0;
    
    //! Return a new tree without all the zeros
    virtual const Expression *shrink() const =0;
    
    virtual bool is_number() const{
      return false;
    }
    
    virtual bool is_variable() const{
      return false;
    }
    
    virtual bool is_opf() const{
      if ((!(this->is_number())) && (!(this->is_variable()))){
        return true;
      }
      return false;
    }
    
    static void tab(bool open) {
      if (open){
        tabcount = tabcount + 1;
      }
      else {
        tabcount = tabcount - 1;
      }
      //std::cout << tabcount;
      std::cout << std::string(tabcount,'\t');
      
    }
    
    static void change() {
      changed++;
    }
    
    static void reset_changed() {
      changed = 0;
    }
    
    static int has_changed() {
      return changed;
    }
    
    void tab() const {
      std::cout << std::string(tabcount,'\t');
    }
};


#endif
