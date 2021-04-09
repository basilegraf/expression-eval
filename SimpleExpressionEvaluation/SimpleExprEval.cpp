#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <regex>
#include <iterator>

#include "SimpleExprEval.h"


namespace
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
    
    // Individual token patterns
    std::array<std::string, NUMBER_TOKEN_TYPE> patterns 
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

expreval::Token::Token()
{

    /*std::cout << "pattern: " << pattern << "\n";
    std::regex re1(pattern, std::regex_constants::extended);
    std::string str1(R"(3.5*f(x[3]+1,2.*(-sin(z) + .1+y[15]))+2)");
    auto re_begin =
        std::sregex_iterator(str1.begin(), str1.end(), re1);
    auto re_end = std::sregex_iterator();

    std::cout << "Found "
              << std::distance(re_begin, re_end)
              << " matches:\n";

    for (std::sregex_iterator i = re_begin; i != re_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        std::cout << match_str << '\n';
    }*/

    return;
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
                
                break;
            }
            
        }
    }
    
    return tokens;
}