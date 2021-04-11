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
    const unsigned int MAX_NUMBER_OPERANDS = 2;
    
    enum eSymbolType 
    {
        VARIABLE_SYMBOL,
        CONSTANT_SYMBOL
    };
    
    struct Symbol
    {
        inline Symbol() :
            type(VARIABLE_SYMBOL),
            address(nullptr),
            constantValue((expr_val_t)0)
        {};
        
        inline Symbol(expr_val_t* address) :
            type(VARIABLE_SYMBOL),
            address(address),
            constantValue((expr_val_t)0)
        {};
        
        eSymbolType type;
        expr_val_t* address;
        expr_val_t constantValue;
        // Note: the symbol name is stored as the key of the symbol map
    };
    
    namespace
    {
        static void DummyFunc(expreval::expr_val_t** vals){};
    }
    
    // Stuct representing a single operation, return value and arguments
    struct Operation
    {
        inline Operation() :
            function(nullptr)
        {
            for (unsigned int k = 0; k < MAX_NUMBER_OPERANDS + 1; k++) 
            {
                vals[k] = nullptr;
            }
        };
        
        decltype(&DummyFunc) function;  // Function to call
        expreval::expr_val_t* vals[MAX_NUMBER_OPERANDS + 1];    // Array of pointers to return-value, function-arg0, ..., , function-argN
    };
    
    class Compiler
    {
    public:
        Compiler();
        // Register a scalar symbol like 'x10'
        void RegisterSymbol(std::string name, expr_val_t& var); 
        // Register an array symbol. Example: name = 'y', size = 3 will create symbols y[0], y[1], y[2]
        void RegisterSymbol(std::string name, expr_val_t* var, unsigned int size); 
        // Add an assignement expression
        void AddExpressionTree(TreeNode tree);
        void Compile();
        void Evaluate();
    private:
        void _addPrivateSymbol(std::string name);
        void _addPrivateConstantSymbol(std::string name, expr_val_t value);
        bool _isSymbolNameRegistered(std::string name);
        void _checkSymbolNameAvailable(std::string name);
        std::string _getNewName();
        void _registerTreeAndSetSymbols(TreeNode& tree, bool isRoot = true); 
        void _compileTree(TreeNode& tree);
    
        std::map<std::string, Symbol> _symbolMap;
        unsigned int _nameCounter;
        std::vector<TreeNode> _registeredTrees;
        std::vector<Operation> _operations;
    };
}

#endif