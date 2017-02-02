#ifndef ast_expression_hpp
#define ast_expression_hpp

#include <string>
#include <iostream>
#include <map>

class Expression
{
private:
  static int tabcount;
public:
    virtual ~Expression()
    {}

    virtual void print() const =0;
    virtual void print_xml() const =0;
    //! Evaluate the tree using the given mapping of variables to numbers
    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const =0;

    //! Return a new tree which is the derivative w.r.t. given variable
    virtual const Expression *differentiate(
        const std::string &variable
    ) const =0;
    
    static void tab(bool open) {
      if (open){
        tabcount = tabcount + 1;
      }
      else {
        tabcount = tabcount - 1;
      }
      //std::cout << tabcount;
      for (int i = 0; i < tabcount; i++){
        std::cout << '\t';
      }
    }
    
    void tab() const {
      //std::cout << "n" << tabcount;
      for (int i = 0; i < tabcount; i++){
        std::cout << '\t';
      }
    }
};


#endif
