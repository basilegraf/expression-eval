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
    
    // Individual token patterns
    std::array<std::string, NUMBER_TOKEN_TYPE> patterns 
    {
        R"(,)",                            // COMMA
        R"([a-zA-Z]+\w*\s*\()",            // FUNCTION
        R"([a-zA-Z]+\w*)",                 // VARIABLE
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
    
    
    struct Token
    {
        Token()
        {
            for (int k = 0; k<NUMBER_TOKEN_TYPE; k++)
            {
                std::cout << patterns[k] << "\n";
            }
            std::cout << "\n";
            std::cout << pattern << "\n";
            
            std::string sexpr(R"(3*f(x+1,2*(-sin(z) + 1+3))+2)");
            std::cout << sexpr << "\n";
            
            std::regex re("a+", std::regex_constants::extended);
            std::string str("baabababaaaabba");
            
            ////////////////
            // repeated search (see also std::regex_iterator)
            std::smatch sm;
            while(regex_search(str, sm, re))
            {
                std::cout << sm.str() << '\n';
                str = sm.suffix();
                std::cout << str << '\n';
            }


            std::cout << "/////////////////// pattern: " << pattern << "\n";
            std::regex re1(pattern, std::regex_constants::extended);
            std::string str1(R"(3.5*f(x+1,2.*(-sin(z) + .1+3))+2)");
            auto re_begin = 
                std::sregex_iterator(str1.begin(), str1.end(), re1);
            auto re_end = std::sregex_iterator();
         
            std::cout << "Found " 
                      << std::distance(re_begin, re_end) 
                      << " mathes:\n";
         
            for (std::sregex_iterator i = re_begin; i != re_end; ++i) 
            {
                std::smatch match = *i;                                                 
                std::string match_str = match.str(); 
                std::cout << match_str << '\n';
            } 
            
            
            ////////////////
            return;
        };
        
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