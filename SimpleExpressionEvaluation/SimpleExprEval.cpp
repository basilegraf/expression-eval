#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <regex>
#include <iterator>

#include "SimpleExprEval.h"


namespace
{
    
    
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

expreval::Token::Token() :
    type(VARIABLE),
    precedence(-1),
    nary(0),
    functionArgs(0)
{
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
    // Re for each tokens
    std::vector<std::basic_regex<char>> reToken;
    for (unsigned int k = 0; k < NUMBER_TOKEN_TYPE; k++)
    {
        reToken.emplace_back(patterns[k], std::regex_constants::extended);
    }
    auto re_begin =
        std::sregex_iterator(sexpr.begin(), sexpr.end(), reTokens);
    auto re_end = std::sregex_iterator();

    std::cout << "Tokenize: Found "
              << std::distance(re_begin, re_end)
              << " matches:\n";
              
    std::vector<expreval::Token> tokens(std::distance(re_begin, re_end));
    
    for (std::sregex_iterator i = re_begin; i != re_end; ++i) {
        std::smatch match = *i;
        std::string tokStr = match.str();
        std::cout << tokStr << '\n'; // TODO Remove
        for (unsigned int k = 0; k < NUMBER_TOKEN_TYPE; k++)
        {
            if (std::regex_match(tokStr, reToken.at(k)))
            {
                std::cout << "Matched " << k << "\n";
                tokens.emplace_back();
                tokens.back().type = expreval::eTokenType(k);
                if (tokens.back().type == expreval::eTokenType::OPARITHM)
                {
                    tokens.back().nary = 2;
                    if (tokStr.compare("-") || tokStr.compare("+"))
                    {
                        // TODO
                    }
                }
                break;
            }
            
        }
    }
    
    return tokens;
}
