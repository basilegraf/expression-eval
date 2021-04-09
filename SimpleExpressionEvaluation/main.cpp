#include <stdio.h>
#include <vector>
#include <exception>


#include "SimpleExprEval.h"

int main(int argc, char **argv)
{
    try
    {
	printf("hello world! \n");
    
    auto tk = expreval::Token();
    
    //////////////////////////
    std::string sexpr(R"(3.5*f(x[3]+1,2.*(-sin(z) + .1+y[15]))+2)");
    auto tokens = expreval::Token::Tokenize(sexpr);
    
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR: " << e.what() << "\n";
        int a;
        std::cin >> a;
    }
    int a;
    std::cin >> a;
    return 0;
}

