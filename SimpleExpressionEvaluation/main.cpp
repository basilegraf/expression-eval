#include <stdio.h>
#include <vector>
#include <exception>

#include <string>


#include "ExpressionParser.h"
#include "ExpressionCompiler.h"

int main(int argc, char **argv)
{
    try
    {
        
        //std::string sexpr(R"(z=-3.5543*f(-x[3] +1, 2.*(-sin(+y[0]) + .1+y[1]))+ 2)");
        //std::string sexpr(R"(x + f(2,m) * 3+ z)");
        //std::string sexpr(R"( ((((a+2*x+1)))) = (1+3)*1)");
        
        std::string sexpr(R"(z=-3.5543*cos(-x[3] +1, 2.*(-sin(+y[0]) + .1+y[1]))+ 2)");
        
        std::cout << "Expression: " << sexpr << "\n";
        
        // Parse expression
        expreval::Parser parser(sexpr);
        auto tree = parser.parse();
        std::cout << "Expression tree:\n";
        tree.Print();
        std::cout << "\n";
        
        // Compile expression
        expreval::Compiler compiler;
        
        expreval::expr_val_t z = 0.0;
        expreval::expr_val_t x = 0.4;
        expreval::expr_val_t y[2] = {0.2, 0.5};
        
        compiler.RegisterSymbol("z", z);
        compiler.RegisterSymbol("x", x);
        compiler.RegisterSymbol("y", y, 2);
        
        compiler.AddExpressionTree(tree);
        
        compiler.Compile();
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

