#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <regex>
#include <iterator>
#include <stdexcept>
#include <cmath>

#include "ExpressionParser.h"
#include "ExpressionCompiler.h"


namespace
{
    // Arithmetics
    void OperatorUnaryPlus(expreval::expr_val_t** vals)
    {
        *vals[0] = *vals[1];
    }
    void OperatorUnaryMinus(expreval::expr_val_t** vals)
    {
        *vals[0] = -*vals[1];
    }
    void OperatorBinaryPlus(expreval::expr_val_t** vals)
    {
        *vals[0] = *vals[1] + *vals[2];
    }
    void OperatorBinaryMinus(expreval::expr_val_t** vals)
    {
        *vals[0] = *vals[1] - *vals[2];
    }
    void OperatorBinaryTimes(expreval::expr_val_t** vals)
    {
        *vals[0] = *vals[1] * *vals[2];
    }
    void OperatorBinaryDivide(expreval::expr_val_t** vals)
    {
        *vals[0] = *vals[1] / *vals[2];
    }
    void OperatorBinaryEqual(expreval::expr_val_t** vals)
    {
        *vals[0] = *vals[1];
    }
    // Trigonometry
    void OperatorSin(expreval::expr_val_t** vals)
    {
        *vals[0] = std::sin(*vals[1]);
    }
    void OperatorCos(expreval::expr_val_t** vals)
    {
        *vals[0] = std::cos(*vals[1]);
    }
    void OperatorTan(expreval::expr_val_t** vals)
    {
        *vals[0] = std::tan(*vals[1]);
    }
    void OperatorASin(expreval::expr_val_t** vals)
    {
        *vals[0] = std::asin(*vals[1]);
    }
    void OperatorACos(expreval::expr_val_t** vals)
    {
        *vals[0] = std::acos(*vals[1]);
    }
    void OperatorATan(expreval::expr_val_t** vals)
    {
        *vals[0] = std::atan(*vals[1]);
    }
    void OperatorATan2(expreval::expr_val_t** vals)
    {
        *vals[0] = std::atan2(*vals[1], *vals[2]);
    }
    
    
    // Operator map
    std::map<std::string, decltype(&OperatorBinaryPlus)> OperatorMap 
    { 
        {"+u",      &OperatorUnaryPlus}, 
        {"-u",      &OperatorUnaryMinus}, 
        {"+b",      &OperatorBinaryPlus}, 
        {"-b",      &OperatorBinaryMinus}, 
        {"*",       &OperatorBinaryTimes}, 
        {"/",       &OperatorBinaryDivide}, 
        {"=",       &OperatorBinaryEqual}, 
        {"sin",     &OperatorSin}, 
        {"cos",     &OperatorCos}, 
        {"tan",     &OperatorTan}, 
        {"asin",    &OperatorASin}, 
        {"acos",    &OperatorACos}, 
        {"atan",    &OperatorATan}, 
        {"atan2",   &OperatorATan2}, 
    };
    
    
    
    
}


expreval::Compiler::Compiler() :
    _nameCounter(0),
    _registeredTrees(0)
{
}

void expreval::Compiler::AddExpressionTree(TreeNode tree)
{
    _registerTreeAndSetSymbols(tree, true);
}

void expreval::Compiler::RegisterSymbol(std::string name, expr_val_t& var)
{
    // Register a scalar symbol
    _checkSymbolNameAvailable(name);
    expreval::Symbol sym(&var);
    _symbolMap[name] = sym;
}

void expreval::Compiler::RegisterSymbol(std::string name, expr_val_t* var, unsigned int size)
{
    // Register an array of symbols
    for (unsigned int k = 0; k < size; k++)
    {
        std::string elemName = name + "[" + std::to_string(k) + "]";
        _checkSymbolNameAvailable(elemName);
        expreval::Symbol sym(&(var[k]));
        _symbolMap[elemName] = sym;
    }
}

void expreval::Compiler::_addPrivateSymbol(std::string name)
{
    // Register symbol with a provisory null pointer
    _checkSymbolNameAvailable(name);
    expreval::Symbol sym;
    _symbolMap[name] = sym;
}

void expreval::Compiler::_addPrivateConstantSymbol(std::string name, expreval::expr_val_t value)
{
    // Register symbol with a provisory null pointer, but store constant value
    _checkSymbolNameAvailable(name);
    expreval::Symbol sym;
    sym.type = expreval::CONSTANT_SYMBOL;
    sym.constantValue = value;
    _symbolMap[name] = sym;
}



bool expreval::Compiler::_isSymbolNameRegistered(std::string name)
{
    auto it = _symbolMap.find(name);
    return (it != _symbolMap.end());
}

void expreval::Compiler::_checkSymbolNameAvailable(std::string name)
{
    if (_isSymbolNameRegistered(name))
    {
        std::string message = "The symbol \'" + name + "\' is already registered.";
        throw std::range_error(message);
    }
}

std::string expreval::Compiler::_getNewName()
{
    // Note: leading underscore is not allowed in parser variable names
    std::string newName = "_tmp" + std::to_string(_nameCounter);
    _nameCounter++;
    return newName;
}

// Recursivelly walk a tree and register one symbol per node
// a) The symbol name of the node is set
// b) If the symbol is not in the symbol table yet, it is added
// The symbol name is taken from the variable name in case of variables and a
// new name and is created in other cases.
void expreval::Compiler::_registerTreeAndSetSymbols(expreval::TreeNode& treeOriginal, bool isRoot)
{
    // If we are dealing with the root tree, make a copy
    expreval::TreeNode treeCopy; // empty (small) tree
    if (isRoot)
    {
        treeCopy = treeOriginal;
    }
    expreval::TreeNode& tree = isRoot ? treeCopy : treeOriginal;
    
    // Impose root nodes to be assignment operator "="
    // This mean we impose expressions of the form 'VARIABLE = ...'
    if (isRoot)
    {
        if (tree.token.str.compare("=") != 0)
        {
            throw std::range_error("Expression tree root should be an assignment operator (\'=\').");
        }
        if (tree.children.size() != 2)
        {
            throw std::logic_error("Assignment operator node does not have two children.");
        }
    }
    else
    {
        if (tree.token.str.compare("=") == 0)
        {
            throw std::range_error("Expression tree nodes other than root cannot be an assignment operator (\'=\').");
        }
    }
    
    // First register children symbols
    for (auto it = std::begin(tree.children); it != std::end(tree.children); it++)
    {
        _registerTreeAndSetSymbols(*it, false); // false: do not add sub-trees to the list
    }
    // Now register this tree node symbol
    std::string symName;
    if (isRoot)
    {
        // We implement a shortcut for the root assignment operator: 
        // we copy the symbol name of the first child (it was already registered and it is the value to be assigned anyways)
        // Next we will only keep the 2nd child sub-tree (i.e. the expression to assign to the first child variable)
        if (tree.children.at(0).token.type != expreval::VARIABLE)
        {
            throw std::range_error("The left-hand side of the root expression assignment operator should be a variable");
        }
        symName = tree.children.at(0).symbolName;
        if (!_isSymbolNameRegistered(symName))
        {
            throw std::logic_error("The left-hand side of the root expression assignment operator was not registered");
        }
    }
    else
    {
        switch (tree.token.type)
        {
            case expreval::VARIABLE :
            {
                symName = tree.token.str;
                if (!_isSymbolNameRegistered(symName))
                {
                    _addPrivateSymbol(symName);
                }
                break;
            }
            case expreval::OPARITHM :
            {
                symName = _getNewName();
                _addPrivateSymbol(symName);
                break;
            }
            case expreval::FUNCTION :
            {
                symName = _getNewName();
                _addPrivateSymbol(symName);
                break;
            }
            case expreval::NUMBER :
            {
                symName = _getNewName();
                expreval::expr_val_t constantValue = std::stod(tree.token.str);
                _addPrivateConstantSymbol(symName, constantValue);
                break;
            }
            default :
            {
                throw std::logic_error("Expression tree contains an unexpected token type.");
            }
        }
    }
    
    // Finally set the tree root's symbol name
    tree.symbolName = symName;
    
    if (isRoot)
    {
        // Copy the root assignement left hand side sub-tree to the list
        _registeredTrees.push_back(tree.children.at(1));
    }
}


