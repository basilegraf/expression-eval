#ifndef SIMPLE_EXPR_EVAL
#define SIMPLE_EXPR_EVAL

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <regex>
#include <iterator>

namespace expreval
{
    
    
    
    
    
    struct Token
    {
        Token();
        static std::vector<Token> Tokenize(std::string sexpr);
        
        unsigned int type;
        int precedence;
        int nary;
    };
    
    template<class T>
    class ShuntingYard
    {
        ShuntingYard()
        {
            
        }
        
    };

}


#endif