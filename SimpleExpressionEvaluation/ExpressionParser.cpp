#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <regex>
#include <iterator>
#include <stdexcept>
#include <cmath>

#include "ExpressionParser.h"


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
        R"(,)",                                     // COMMA
        R"([a-zA-Z]+[a-zA-Z0-9]*\s*\()",            // FUNCTION     note: \w does not work
        R"([a-zA-Z]+[a-zA-Z0-9]*(\[[0-9]+\])*)",    // VARIABLE     note: \w does not work
        R"([0-9]*\.[0-9]+|[0-9]+\.[0-9]*|[0-9]+)",  // NUMBER       note: \d does not work
        R"(\+|\-|\*|/|=)",                            // OPARITHM
        R"(\()",                                    // PAROPEN
        R"(\))"                                     // PARCLOSE
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
                if (tokStr.compare("=") == 0)
                {
                    nary = 2;
                    precedence = 1;
                }
                else if ((tokStr.compare("*") == 0) || (tokStr.compare("/") == 0))
                {
                    nary = 2;
                    precedence = 4;
                }
                else if ((tokStr.compare("+") == 0) || (tokStr.compare("-") == 0))
                {
                    nary = 2;
                    precedence = 2;
                    // Detect unary - or + from the type of the previous token
                    if (
                    (prevTokType == expreval::eTokenType::OPARITHM) || 
                    (prevTokType == expreval::eTokenType::COMMA) ||
                    (prevTokType == expreval::eTokenType::FUNCTION) ||
                    (prevTokType == expreval::eTokenType::PARENOPEN) ||
                    (prevTokType == expreval::eTokenType::NUMBER_TOKEN_TYPE)) // this means there was no previous token
                    {
                        nary = 1;
                        precedence = 3;
                    }
                }
            }
            else if (type == expreval::eTokenType::PARENOPEN)
            {
                precedence = 0; //Lowest precedence
            }
            
            
            // std::cout << tokStr << "\tMatched " << cTokenName[k] << "\tnary = " << nary << "\t prev: "<< (int)prevTokType <<"\n";
            
            break;
        }
    }
    return;
}


expreval::TreeNode::TreeNode() :
    children(0),
    symbolName("")
{
}

void expreval::TreeNode::Print()
{
    if (children.size() > 0)
    {
        std::string separator = "(";
        if (token.type == expreval::eTokenType::FUNCTION)
        {
            separator =  "";
        }         
        std::cout << token.str;
        for (auto it = children.begin(); it != children.end(); it++)
        {
            std::cout << separator;
            it->Print();
            separator = ",";
        }
        std::cout << ")";
    }
    else
    {
        // This is a leaf, just print the token string
        std::cout << token.str;
    }
    
}



std::vector<expreval::Token> expreval::Token::Tokenize(std::string sexpr)
{
    // Re for general token pattern
    std::regex reTokens(pattern, std::regex_constants::extended);
    
    auto re_begin =
        std::sregex_iterator(sexpr.begin(), sexpr.end(), reTokens);
    auto re_end = std::sregex_iterator();

    /*std::cout << "Tokenize: Found "
              << std::distance(re_begin, re_end)
              << " matches:\n";*/
              
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




expreval::Parser::Parser(std::string sexpr) :
    _expression(sexpr),
    _tokens(0),
    _re_pow(R"(pow\s*\()", std::regex_constants::extended)
{
}


expreval::TreeNode expreval::Parser::_apply(Token optor, std::vector<TreeNode> opands)
{
    expreval::TreeNode tree;
    // Keep operator token in root node
    tree.token = optor;
    // Copy operands to children (reversing order since they where popped from a stack)
    tree.children.resize(opands.size());
    std::reverse_copy(std::begin(opands), std::end(opands), std::begin(tree.children));
    return tree;
}


void expreval::Parser::_popAndApply()
{
    // Get operator from operator stack
    _checkOperatorStack();
    expreval::Token optor = _operators.top();
    _operators.pop(); // note: pop() does not return anything...
    // Get nary operands from operand stack
    std::vector<TreeNode> opands(0);
    for (int k = 0; k < optor.nary; k++)
    {
        _checkOperandStack();
        opands.push_back(_operands.top());
        _operands.pop();
    }
    // Apply operator to operands
    expreval::TreeNode tree = _apply(optor, opands);
    // Push result (tree) to operand stack
    _operands.push(tree);
}


void expreval::Parser::_applyFunction(Token func)
{
    // Intercept special cases
    if (std::regex_match(func.str, _re_pow)) // pow() function
    {
        if ((func.functionArgs.size() == 2) && (func.functionArgs.at(1).token.type == expreval::NUMBER))
        {
            expreval::expr_val_t exponent = std::stod(func.functionArgs.at(1).token.str);
            if (exponent == 2.0)
            {
                // change pow(...,2) to square(...)
                func.functionArgs.pop_back(); // remove second argument
                func.str = "square(";
            }
            else if (exponent == 3.0)
            {
                // change pow(...,2) to square(...)
                func.functionArgs.pop_back(); // remove second argument
                func.str = "cube(";
            }
            // TODO: Deal with other integer powers, or use --fast-math compiler option (dangerous?)
        }
    }
    
    // Set nary (even if not used later)
    func.nary = func.functionArgs.size();
    
    expreval::TreeNode funcTree;
    funcTree.token = func;
    // Copy function arguments to child nodes
    funcTree.children.resize(func.functionArgs.size());
    std::copy(std::begin(func.functionArgs), std::end(func.functionArgs), std::begin(funcTree.children));
    // Push function tree to operand stack
    _operands.push(funcTree);
}


void expreval::Parser::_checkOperatorStack()
{
    if (_operators.empty())
    {
        std::string message = "Error while parsing expression \n\t" + _expression + "\nEmpty operator on stack."; 
        throw std::logic_error(message.c_str());
    }
}

void expreval::Parser::_checkOperandStack()
{
    if (_operands.empty())
    {
        std::string message = "Error while parsing expression \n\t" + _expression + "\nEmpty operand on stack."; 
        throw std::logic_error(message.c_str());
    }
}



expreval::TreeNode expreval::Parser::parse()
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
            _checkOperatorStack();
            while (_operators.top().type != expreval::eTokenType::FUNCTION)
            {
                _popAndApply();
            }
            // The top of the operand stack is now the next argument for that function call, add it to its list
            _checkOperatorStack();
            _checkOperandStack();
            _operators.top().functionArgs.push_back(_operands.top());
            _operands.pop();
        }
        
        else if (token.type == expreval::eTokenType::PARENOPEN)
        {
            // Push on operator stack
            _operators.push(token);
        }
        
        else if (token.type == expreval::eTokenType::PARENCLOSE)
        {
            // Pop and apply until we see a "(" or "fun("
            _checkOperatorStack();
            while ((_operators.top().type != expreval::eTokenType::PARENOPEN) && (_operators.top().type != expreval::eTokenType::FUNCTION))
            {
                _popAndApply();
                _checkOperatorStack(); // Check for next while loop condition or next top
            }
            // Open paren "(" or "fun(" is on the top now
            auto topOperator = _operators.top();
            _operators.pop();
            // If "(" discard, if "fun(" apply its arg list and push to operands
            if (topOperator.type == expreval::eTokenType::FUNCTION)
            {
                // The top of the operand stack is now the next argument for that function call, add it to it list
                _checkOperandStack(); 
                topOperator.functionArgs.push_back(_operands.top());
                _operands.pop();
                // Add function call to operand tree
                _applyFunction(topOperator);
            }
        }
    }
    
    // When all tokens are read, pop and apply all remaining optors (and 
    // opands)
    while (!_operators.empty())
    {
        _popAndApply();
    }
    
    // Operand should contain the desired tree
    _checkOperandStack();
    if (_operands.size() != 1)
    {
        std::cout << "Operand stack size: " << _operands.size() << "\n";
        while (!_operands.empty())
        {
            std::cout << "\t";
            _operands.top().Print();
            _operands.pop();
            std::cout << "\n";
        }
        throw std::logic_error( "Operand stack ended up with a number of elements different than 1." );
    }
    
    _tree = _operands.top();
    return _tree;
}

