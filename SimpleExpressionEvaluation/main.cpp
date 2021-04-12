#include <stdio.h>
#include <vector>
#include <exception>

#include <string>

#include <cmath>

#include <chrono>

#include "ExpressionParser.h"
#include "ExpressionCompiler.h"




void func(double* z, double* x, double* y)
{
    using namespace std;
    double aaa = -3.5543*cos(-(*x) + 1 + 2.*(-sin(+y[0]) + .1+y[1]))+ 2;
    *z=-cos(aaa + (*x)*y[0]);
}


// Timing functions
inline std::chrono::steady_clock::time_point tic()
{
    return std::chrono::steady_clock::now();
}

inline double toc(std::chrono::steady_clock::time_point tic_begin)
{
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    double durEval = std::chrono::duration_cast<std::chrono::microseconds> (end - tic_begin).count();
    return 1.0e-6 * durEval;
}

int main(int argc, char **argv)
{
    try
    {
        
        //std::string sexpr(R"(z=-3.5543*f(-x[3] +1, 2.*(-sin(+y[0]) + .1+y[1]))+ 2)");
        //std::string sexpr(R"(x + f(2,m) * 3+ z)");
        //std::string sexpr(R"( ((((a+2*x+1)))) = (1+3)*1)");
        
        std::string sexpr = 
            "aaa=-3.5543*cos(-x + 1 + 2.*(-sin(+y[0]) + .1+y[1]))+ 2;"
            "z=-cos(aaa + x*y[0])";
        
        /*std::cout << "Expression: " << sexpr << "\n";
        
        // Parse expression
        expreval::Parser parser(sexpr);
        auto tree = parser.parse();
        std::cout << "Expression tree:\n";
        tree.Print();
        std::cout << "\n";*/
        
        // Compile expression
        expreval::Compiler compiler;
        
        expreval::expr_val_t z = 0.0;
        expreval::expr_val_t x = 0.4;
        expreval::expr_val_t y[2] = {0.2, 0.5};
        
        compiler.RegisterSymbol("z", z);
        compiler.RegisterSymbol("x", x);
        compiler.RegisterSymbol("y", y, 2);
        
        //compiler.AddExpressionTree(tree);
        //compiler.Compile();
        compiler.CompileExpression(sexpr);
        
        x = 0.4;
        auto t0 = tic();
        int nRuns = 100000;
        for (int k = 0; k<nRuns; k++)
        {
            x += 1.0e-4;
            compiler.Evaluate();
        }
        double durEval = toc(t0);
        std::cout << "Time difference = " << 1.0e6 * durEval / nRuns << "[us]" << std::endl;
        
        x = 0.4;
        expreval::expr_val_t zTest = 0;
        t0 = tic();
        for (int k = 0; k<nRuns; k++)
        {
            x += 1.0e-4;
            //zTest = -3.5543*(std::cos)(-x + 1 + 2.*(-(std::sin)(+y[0]) + .1+y[1]))+ 2;
            func(&zTest, &x, y);
        }
        double durNative = toc(t0);
        std::cout << "Time difference = " << 1.0e6 * durNative / nRuns << "[us]" << std::endl;
        std::cout << "Ratio = " << double(durEval) / double(durNative) << std::endl;
        
        std::cout << z << " " << zTest << "\n";
        
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

