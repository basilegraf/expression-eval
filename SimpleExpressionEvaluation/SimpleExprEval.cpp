#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <regex>
#include <iterator>
#include <stdexcept>

#include "SimpleExprEval.h"


namespace
{
    char const* cTokenName[] =
    {
        "COMMA",
        "FUNCTION",
        "VARIABLE",
        "NUMBER",
        "OPARITHM",
        "PARENOPEN",
        "PARENCLOSE"
    };
    
    // Individual token patterns
    std::array<std::string, expreval::NUMBER_TOKEN_TYPE> patterns 
    {
        R"(,)",                            // COMMA
        R"([a-zA-Z]+\w*\s*\()",            // FUNCTION
        R"([a-zA-Z]+\w*(\[[0-9]+\])*)",                 // VARIABLE
        R"([0-9]*\.[0-9]+|[0-9]+\.[0-9]*|[0-9]+)",        // NUMBER note: \d does not work
        R"(\+|\-|\*|/)",                   // OPARITHM
        R"(\()",                           // PAROPEN
        R"(\))"                            // PARCLOSE
    };
    
    // Full pattern
    std::string pattern = R"(()" + 
        patterns[0] + R"(|)" + 
        patterns[1] + R"(|)" + 
        patterns[2] + R"(|)" + 
        patterns[3] + R"(|)" + 
        patterns[4] + R"(|)" + 
        patterns[5] + R"(|)" + 
        patterns[6] + R"())";
}

expreval::Token::Token(std::string tokStr,  expreval::eTokenType prevTokType) :
    str(tokStr),
    type(expreval::eTokenType::NUMBER_TOKEN_TYPE),
    precedence(-1),
    nary(0), // in case of function, nary will be determined during parsing
    functionArgs(0)
{
    
    for (unsigned int k = 0; k < NUMBER_TOKEN_TYPE; k++)
    {
        // RegEx for k'th token type
        std::regex reToken(patterns[k], std::regex_constants::extended);
        
        if (std::regex_match(tokStr, reToken))
        {            
            type = expreval::eTokenType(k);
            
            // If token is an arithmetic operator
            if (type == expreval::eTokenType::OPARITHM)
            {
                if ((tokStr.compare("*") == 0) || (tokStr.compare("/") == 0))
                {
                    nary = 2;
                    precedence = 3;
                }
                else if ((tokStr.compare("+") == 0) || (tokStr.compare("-") == 0))
                {
                    nary = 2;
                    precedence = 1;
                    // Detect unary - or + from the type of the previous token
                    if (
                    (prevTokType == expreval::eTokenType::OPARITHM) || 
                    (prevTokType == expreval::eTokenType::COMMA) ||
                    (prevTokType == expreval::eTokenType::FUNCTION) ||
                    (prevTokType == expreval::eTokenType::PARENOPEN) ||
                    (prevTokType == expreval::eTokenType::NUMBER_TOKEN_TYPE)) // this means there was no previous token
                    {
                        nary = 1;
                        precedence = 2;
                    }
                }
            }
            else if (type == expreval::eTokenType::PARENOPEN)
            {
                precedence = 0; //Lowest precedence
            }
            
            
            std::cout << tokStr << "\tMatched " << cTokenName[k] << "\tnary = " << nary << "\t prev: "<< (int)prevTokType <<"\n";
            
            break;
        }
    }
    return;
}


expreval::TreeNode::TreeNode() :
    //parent(nullptr),
    children(0)
{
}



std::vector<expreval::Token> expreval::Token::Tokenize(std::string sexpr)
{
    // Re for general token pattern
    std::regex reTokens(pattern, std::regex_constants::extended);
    
    auto re_begin =
        std::sregex_iterator(sexpr.begin(), sexpr.end(), reTokens);
    auto re_end = std::sregex_iterator();

    std::cout << "Tokenize: Found "
              << std::distance(re_begin, re_end)
              << " matches:\n";
              
    std::vector<expreval::Token> tokens(0);
    
    // Go through all matched tokens
    auto lastTokType = expreval::eTokenType::NUMBER_TOKEN_TYPE; // Used to mean there was no last token
    for (std::sregex_iterator i = re_begin; i != re_end; ++i) {
    
        std::smatch match = *i;
        std::string tokStr = match.str();
    
        tokens.emplace_back(tokStr, lastTokType);
        lastTokType = tokens.back().type; // Current token    
    }
    
    return tokens;
}




expreval::ShuntingYard::ShuntingYard(std::string sexpr) :
    _expression(sexpr),
    _tokens(0)
{
    
}


expreval::TreeNode expreval::ShuntingYard::_apply(Token optor, std::vector<TreeNode> opands)
{
    expreval::TreeNode tree;
    // Keep operator token in root node
    tree.token = optor;
    // Copy operands to children (reversing order since they where popped from a stack)
    std::reverse_copy(std::begin(opands), std::end(opands), std::begin(tree.children));
    return tree;
}


void expreval::ShuntingYard::_popAndApply()
{
    // Get operator from operator stack
    expreval::Token optor = _operators.top();
    _operators.pop(); // note: pop() does not return anything...
    // Get nary operands from operand stack
    std::vector<TreeNode> opands(0);
    for (int k = 0; k < optor.nary; k++)
    {
        opands.push_back(_operands.top());
        _operands.pop();
    }
    // Apply operator to operands
    expreval::TreeNode tree = _apply(optor, opands);
    // Push result (tree) to operand stack
    _operands.push(tree);
}


void expreval::ShuntingYard::_applyFunction(Token func)
{
    expreval::TreeNode funcTree;
    funcTree.token = func;
    // Copy function arguments to child nodes
    std::copy(std::begin(func.functionArgs), std::end(func.functionArgs), std::begin(funcTree.children));
    // Push function tree to operand stack
    _operands.push(funcTree);
}


void expreval::ShuntingYard::parse()
{
    // Empty stacks
    while (!_operands.empty()) _operands.pop();
    while (!_operators.empty()) _operators.pop();
    
    // Init tree root
    _tree = expreval::TreeNode();
    
    // Tokenize expression
    _tokens = expreval::Token::Tokenize(_expression);
    
    // Read tokens from left to right
    for (auto tokIt = _tokens.begin(); tokIt != _tokens.end(); tokIt++)
    {
        Token& token = *tokIt;
        // If token is an operand, push it to the operand stack
        if ((token.type == expreval::eTokenType::NUMBER) || (token.type == expreval::eTokenType::VARIABLE))
        {
            auto opand = expreval::TreeNode();
            opand.token = token;
            _operands.push(opand);
        }
        // If token is an arithmetic operator
        else if (token.type == expreval::eTokenType::OPARITHM)
        {
            // While there's an optor with precedence >= the precedence of 
            // the token, pop it and pop it's opands, apply it and push 
            // result to the opand stack
            while (!_operators.empty())
            {
                auto& topOperator = _operators.top();
                if (topOperator.precedence >= token.precedence)
                {
                    // Additional condition for unary operators
                    if ((token.nary == 1) && (topOperator.nary != 1))
                    {
                        break;
                    }
                    _popAndApply();
                }
                else
                {
                    break;
                }
            }
            // Then push the token (operator) to the operator stack
            _operators.push(token);
        }
        
        else if (token.type == expreval::eTokenType::FUNCTION)
        {
            // Push function to operator stack (along with its to-be-filled argument list)
            _operators.push(token);
        }
        
        else if (token.type == expreval::eTokenType::COMMA)
        {
            while (_operators.top().type != expreval::eTokenType::FUNCTION)
            {
                _popAndApply();
            }
            // The top of the operand stack is now the next argument for that function call, add it to its list
            _operators.top().functionArgs.push_back(_operands.top());
            _operands.pop();
        }
    }
}
