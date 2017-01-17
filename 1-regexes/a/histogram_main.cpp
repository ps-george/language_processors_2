#include "histogram.hpp"

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iomanip>

typedef std::unordered_map<std::string,unsigned> histogram_type;


// Define the instance of the variable that is declared in the header
TokenValue yylval;

int main()
{
    histogram_type histogram;
    double sum=0;
    std::cout.setf(std::ios::fixed,std::ios::floatfield);
    std::cout.precision(3);
    while(1){
        TokenType type=(TokenType)yylex();

        if(type==None){
            break; // No more tokens

        }else if(type==Number){
            // We have a number. The value will be
            // in yylval.numberValue
            sum += yylval.numberValue;
            // TODO: add to sum

        }else if(type==Word){
            // We have a string. The value is in a string
            // _pointed to_ by yylval.stringValue. Note that
            // the string is allocated by the lexer, but
            // deallocated by us.

            // add yylval.wordValue to histogram
            std::string s = (std::string)(*yylval.wordValue);
            //std::cerr << "Converted yylval pointer to string: " << s << std::endl;
            int hc = histogram.count(s);
            if (hc) {
              // found
              histogram[s] = hc + 1;
            } else {
              histogram.insert({s,1});
              // not found
            }
            //std::cerr << "Inserted word into histogram" << std::endl;
            // Free the pointer yylval.wordValue to stop leaks
            delete yylval.wordValue;
            //std::cout << "Freed pointer to yylval.stirngValue to stop leaks\n";
        }else{
            assert(0); // There are only three token types.
            return 1;
        }
    }


    // TODO: print out `sum` to std::cout with three decimal digits
    std::cout << sum << std::endl;

    // This sorts the histogram TODO: Find out how
    std::vector<std::pair<std::string,double>> sorted(histogram.begin(), histogram.end());
    //std::cerr << "Put histogram into vector called sorted." << std::endl;

    std::sort(sorted.begin(), sorted.end(), [](const std::pair<std::string,double> &a, const std::pair<std::string,double> &b){
        if(a.second > b.second)
            return true;
        if(a.second < b.second)
            return false;
        return a.first < b.first;
    });

    auto it=sorted.begin();
    while(it!=sorted.end()){
        std::string name=it->first;
        unsigned count=it->second;
        // Print out `name` and `count` to std::cout
        std::cout << "\"" << name << "\" " << count << std::endl;

        ++it;
    }

    return 0;
}
