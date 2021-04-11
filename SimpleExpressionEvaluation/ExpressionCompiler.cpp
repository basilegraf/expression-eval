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
    _nameCounter(0)
{
}

void expreval::Compiler::RegisterSymbol(std::string name, expr_val_t& var)
{
    // Register a scalar symbol
    _checkSymbolNameAvailable(name);
    _symbolMap[name] = &var;
}

void expreval::Compiler::RegisterSymbol(std::string name, expr_val_t* var, unsigned int size)
{
    // Register an array of symbols
    for (unsigned int k = 0; k < size; k++)
    {
        std::string elemName = name + "[" + std::to_string(k) + "]";
        _checkSymbolNameAvailable(elemName);
        _symbolMap[elemName] = &(var[k]);
    }
}

void expreval::Compiler::_addPrivateSymbol(std::string name)
{
    // Register symbol with a provisory null pointer
    _checkSymbolNameAvailable(name);
    _symbolMap[name] = nullptr;
}

void expreval::Compiler::_checkSymbolNameAvailable(std::string name)
{
    auto it = _symbolMap.find(name);
    if (it != _symbolMap.end())
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
