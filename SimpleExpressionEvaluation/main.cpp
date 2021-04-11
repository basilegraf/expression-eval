#include <stdio.h>
#include <vector>
#include <exception>



#include "ExpressionParser.h"

int main(int argc, char **argv)
{
    try
    {
    
    std::string sexpr(R"(z=-3.5543*f(-x[3] +1,2.*(-sin(+z) + .1+y3[150]))+ 2)");
    //std::string sexpr(R"(x + f(2,m) * 3+ z)");
    //std::string sexpr(R"( ((((a+2*x+1)))) = (1+3)*1)");
    
    std::cout << "Expression: " << sexpr << "\n";
    
    auto parser = expreval::Parser(sexpr);
    auto tree = parser.parse();
    std::cout << "Expression tree:\n";
    tree.Print();
    std::cout << "\n";
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

