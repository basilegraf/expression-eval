/*
 * Parse an infix mathematical expression using the Shunting Yard Algorithm.
 * The parser creates an expression tree with operators in the nodes and operands in the leaves.
 * 
 * Operators:
 *  Binary                               +, -, *, /, =
 *  Unary                                +, -
 *  Function calls like                  f(x), g12(x,3,y)
 * Operands:
 *  Numbers in the form                  12, 12., .12, 12.345
 *  Variables like                       x, y10, xyz4n3
 *  Variables (array elements) like      x[0], y10[30], xyz[2]
 * 
 * The parsing is not robust against syntax errors.
 * 
 * Reference: 
 *  https://www.reedbeta.com/blog/the-shunting-yard-algorithm/
 * 
 * */

#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

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
        TreeNode();
        void Print();
        
        Token token; 
        std::vector<TreeNode> children;
        std::string symbolName; // used by compiler only
    };
    
    class Parser
    {
    public:
        Parser(std::string expression);
        TreeNode parse();                                // Build tree from input expression
    private:        
        TreeNode _apply(Token optor, std::vector<TreeNode> opands);    // Build a sub-tree from an operator and list of operands (i.e. apply operator to operands)
        void _popAndApply();                         // Pop operator and operands from stacks and apply. Push result to operand stack
        void _applyFunction(Token func);             // Build function sub-tree
        void _checkOperatorStack(); // error if empty
        void _checkOperandStack(); // error if empty
        std::stack<Token> _operators;
        std::stack<TreeNode> _operands;
        std::string _expression;
        std::vector<Token> _tokens;
        TreeNode _tree;
    };

}


#endif