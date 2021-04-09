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
    enum eTokenType 
    {
        COMMA = 0,
        FUNCTION,
        VARIABLE,
        NUMBER,
        OPARITHM,
        PARENOPEN,
        PARENCLOSE,
        NUMBER_TOKEN_TYPE
    };
    
    typedef double expr_val_t;
    struct TreeNode;
    
    struct Token
    {
        Token();
        static std::vector<Token> Tokenize(std::string sexpr);
        
        eTokenType type;
        int precedence;
        int nary;
        std::vector<TreeNode> functionArgs; // Only used if type == FUNCTION
    };
    
    struct TreeNode
    {
        expr_val_t* value;
        TreeNode();
        TreeNode* parent; // Needed?
        std::vector<TreeNode> children;
    };
    
    class ShuntingYard
    {
        ShuntingYard()
        {
            
        }
        
    };

}


#endif