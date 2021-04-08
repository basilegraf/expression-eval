#include <stdio.h>
#include <vector>
#include <exception>


#include "SimpleExprEval.hpp"

int main(int argc, char **argv)
{
    try
    {
	printf("hello world! \n");
    
    auto tk = expreval::Token();
    
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

