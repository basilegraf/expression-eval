#ifndef EXPRESSION_COMPILER_H
#define EXPRESSION_COMPILER_H

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <regex>
#include <iterator>


namespace expreval
{
    class Compiler
    {
    public:
        Compiler();
        void AddExpressionTree(TreeNode tree);
        // Register a scalar symbol like 'x10'
        void RegisterSymbol(std::string name, expr_val_t& var); 
        // Register an array symbol. Example: name = 'y', size = 3 will create symbols y[0], y[1], y[2]
        void RegisterSymbol(std::string name, expr_val_t* var, unsigned int size); 
        void Compile();
        void Evaluate();
    private:
        void _addPrivateSymbol(std::string name);
        void _checkSymbolNameAvailable(std::string name);
        std::string _getNewName();
    
        std::map<std::string, expr_val_t*> _symbolMap;
        unsigned int _nameCounter;
    };
}


#endif