#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <regex>
#include <iterator>

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
    parent(nullptr),
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
