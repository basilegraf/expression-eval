#ifndef SIMPLE_EXPR_EVAL
#define SIMPLE_EXPR_EVAL

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <regex>
#include <iterator>
#include <stack>

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
    public:
        Token(std::string = "", eTokenType prevTokType = NUMBER_TOKEN_TYPE);
        static std::vector<Token> Tokenize(std::string sexpr);
    
        std::string str;
        eTokenType type;
        int precedence;
        int nary;
        std::vector<TreeNode> functionArgs; // Only used if type == FUNCTION
    };
    
    struct TreeNode
    {
        expr_val_t* value;
        Token token; 
        TreeNode();
        void Print();
        //TreeNode* parent; // Needed?
        std::vector<TreeNode> children;
    };
    
    class ShuntingYard
    {
    public:
        ShuntingYard(std::string expression);
        TreeNode parse();                                // Build tree from input expression
    private:        
        TreeNode _apply(Token optor, std::vector<TreeNode> opands);    // Build a sub-tree from an operator and list of operands (i.e. apply operator to operands)
        void _popAndApply();                         // Pop operator and operands from stacks and apply. Push result to operand stack
        void _applyFunction(Token func);             // Build function sub-tree
        
        std::stack<Token> _operators;
        std::stack<TreeNode> _operands;
        std::string _expression;
        std::vector<Token> _tokens;
        TreeNode _tree;
    };

}


#endif