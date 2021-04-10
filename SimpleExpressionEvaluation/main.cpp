#include <stdio.h>
#include <vector>
#include <exception>



#include "SimpleExprEval.h"

int main(int argc, char **argv)
{
    try
    {
    
    std::string sexpr(R"(-3.5*f(-x[3] +1,2.*(-sin(+z) + .1+y[15]))+ 2)");
    //std::string sexpr(R"(x + f(2,m) * 3+ z)");
    
    std::cout << "Expression: " << sexpr << "\n";
    
    auto shYrd = expreval::ShuntingYard(sexpr);
    auto tree = shYrd.parse();
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

