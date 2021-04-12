#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <sstream>
#include <regex>
#include <iterator>
#include <stdexcept>
#include <cmath>

#include "ExpressionParser.h"
#include "ExpressionCompiler.h"


namespace
{
    // Arithmetics
    static void OperatorUnaryPlus(expreval::expr_val_t** vals)
    {
        *vals[0] = *vals[1];
    }
    static void OperatorUnaryMinus(expreval::expr_val_t** vals)
    {
        *vals[0] = -*vals[1];
    }
    static void OperatorBinaryPlus(expreval::expr_val_t** vals)
    {
        *vals[0] = *vals[1] + *vals[2];
    }
    static void OperatorBinaryMinus(expreval::expr_val_t** vals)
    {
        *vals[0] = *vals[1] - *vals[2];
    }
    static void OperatorBinaryTimes(expreval::expr_val_t** vals)
    {
        *vals[0] = *vals[1] * *vals[2];
    }
    static void OperatorBinaryDivide(expreval::expr_val_t** vals)
    {
        *vals[0] = *vals[1] / *vals[2];
    }
    static void OperatorBinaryEqual(expreval::expr_val_t** vals)
    {
        *vals[0] = *vals[1];
    }
    // Trigonometry
    static void OperatorSin(expreval::expr_val_t** vals)
    {
        *vals[0] = std::sin(*vals[1]);
    }
    static void OperatorCos(expreval::expr_val_t** vals)
    {
        *vals[0] = std::cos(*vals[1]);
    }
    static void OperatorTan(expreval::expr_val_t** vals)
    {
        *vals[0] = std::tan(*vals[1]);
    }
    static void OperatorASin(expreval::expr_val_t** vals)
    {
        *vals[0] = std::asin(*vals[1]);
    }
    static void OperatorACos(expreval::expr_val_t** vals)
    {
        *vals[0] = std::acos(*vals[1]);
    }
    static void OperatorATan(expreval::expr_val_t** vals)
    {
        *vals[0] = std::atan(*vals[1]);
    }
    static void OperatorATan2(expreval::expr_val_t** vals)
    {
        *vals[0] = std::atan2(*vals[1], *vals[2]);
    }
    
    static void OperatorPow(expreval::expr_val_t** vals)
    {
        *vals[0] = std::pow(*vals[1], *vals[2]);
    }
    static void OperatorSquare(expreval::expr_val_t** vals)
    {
        *vals[0] = *vals[1] * *vals[1];
    }
    static void OperatorCube(expreval::expr_val_t** vals)
    {
        *vals[0] = *vals[1] * *vals[1] * *vals[1];
    }
    static void OperatorSqrt(expreval::expr_val_t** vals)
    {
        *vals[0] = std::sqrt(*vals[1]);
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
        {"pow",     &OperatorPow}, 
        {"square",  &OperatorSquare}, 
        {"cube",    &OperatorCube}, 
        {"sqrt",    &OperatorSqrt}, 
    };
    
    
    
    
}


expreval::Compiler::Compiler() :
    _alreadyCompiled(false),
    _nameCounter(0),
    _registeredTrees(0),
    _privateVariables(nullptr),
    _privateVariablesSize(0)
{
}

expreval::Compiler::~Compiler()
{
    if (_privateVariables != nullptr)
    {
        delete[] _privateVariables;
        _privateVariables = nullptr;
    }
}

void expreval::Compiler::AddExpressionTree(TreeNode tree)
{
    _registerTreeAndSetSymbols(tree, true);
}

void expreval::Compiler::RegisterSymbol(std::string name, expr_val_t& var)
{
    // Register a scalar symbol
    expreval::Symbol sym(&var);
    _symbolMap[name] = sym;
}

void expreval::Compiler::RegisterSymbol(std::string name, expr_val_t* var, unsigned int size)
{
    // Register an array of symbols
    for (unsigned int k = 0; k < size; k++)
    {
        std::string elemName = name + "[" + std::to_string(k) + "]";
        expreval::Symbol sym(&(var[k]));
        _symbolMap[elemName] = sym;
    }
}

std::vector<std::string> expreval::Compiler::_split(const std::string& s, char delim)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) 
    {
        elems.push_back(item);
    }
    return elems;
}

void expreval::Compiler::_addPrivateSymbol(std::string name)
{
    // Register symbol with a provisory null pointer
    _checkSymbolNameAvailable(name);
    expreval::Symbol sym;
    _symbolMap[name] = sym;
}

void expreval::Compiler::_removePrivateSymbol(std::string name)
{
    
    if (!_isSymbolNameRegistered(name))
    {
        throw std::logic_error("Cannot remove unregistered symbol.");
    }
    // Allow removing only if still assigned to a nullptr
    auto& sym = _symbolMap[name];
    if (sym.address != nullptr)
    {
        throw std::logic_error("Cannot remove a symbol that was already assigned a variable address.");
    }
    _symbolMap.erase(name);
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

bool expreval::Compiler::_isLeaf(TreeNode& tree)
{
    return std::begin(tree.children) == std::end(tree.children);
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
    // Note: leading # is not allowed in parser variable names
    std::string newName = "#tmp" + std::to_string(_nameCounter);
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
        //
        // NOTE: This has the side effect that an expression of the form x = y will generate an error. Indeed, the the symbol 
        // y will be removed and replaced by x. So that y will be missing when e.g. compiling the previous expressions. 
        // However, such an assignement is not very usefull...
        if (tree.children.at(0).token.type != expreval::VARIABLE)
        {
            throw std::range_error("The left-hand side of the root expression assignment operator should be a variable");
        }
        // Symbol name from assignement lhs
        symName = tree.children.at(0).symbolName;
        // Copy it to assignement rhs Symbol name
        _removePrivateSymbol(tree.children.at(1).symbolName);
        tree.children.at(1).symbolName = symName; 
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
            case expreval::COMMA :
            case expreval::PARENOPEN :
            case expreval::PARENCLOSE :
            case expreval::NUMBER_TOKEN_TYPE :
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

// Go through the symbol table. For every symbol with a nullptr, create a private variable and associate it to it
void expreval::Compiler::_createPrivateVariables()
{
    // Prevent doing this twice
    if (_privateVariables != nullptr)
    {
        throw std::logic_error("Private variables where already created. Are you trying to compile again?");
    }
    
    // Count number of symbols with nullptr
    unsigned int numNullptr = 0;
    for (auto symIt = std::begin(_symbolMap); symIt != std::end(_symbolMap); symIt++)
    {
        if (symIt->second.address == nullptr)
        {
            numNullptr++;
        }
    }
    
    // Create private variables array
    _privateVariables = new expreval::expr_val_t[numNullptr];
    _privateVariablesSize = numNullptr;
    
    // Walk through symbols again and associate addresses and constant values
    unsigned int k = 0;
    for (auto symIt = std::begin(_symbolMap); symIt != std::end(_symbolMap); symIt++)
    {
        if (symIt->second.address == nullptr)
        {
            auto& sym = symIt->second;
            sym.address = &_privateVariables[k];
            if (sym.type == expreval::CONSTANT_SYMBOL)
            {
                _privateVariables[k] = sym.constantValue;
            }
            k++;
        }
    }
}


// Compile expression trees into one list of operations
void expreval::Compiler::Compile()
{
    // Make sure we do this only once
    if (_alreadyCompiled)
    {
        throw std::runtime_error("Can compile only once.");
    }
    _alreadyCompiled = true;
    
    // Create private variables and associate them with the 'nullptr' symbol 
    _createPrivateVariables();
    for (auto treeIt = std::begin(_registeredTrees); treeIt != std::end(_registeredTrees); treeIt++)
    {
        _compileTree(*treeIt);
    }
}

void expreval::Compiler::_compileTree(TreeNode& tree)
{
    // Start with childrens
    for (auto chldIt = std::begin(tree.children); chldIt != std::end(tree.children); chldIt++)
    {
        _compileTree(*chldIt);
    }
    
    // Add current node's operation
    std::string opKey = "";
    unsigned int nary = 0;
    bool addOperation = false;
    switch (tree.token.type)
    {
        case expreval::OPARITHM :
        {
            if (tree.token.str.compare("=") == 0)
            {
                throw std::logic_error("Not expecting an assignment operator in tree to compile");
            }
            
            if ((tree.token.str.compare("+") == 0) || (tree.token.str.compare("-") == 0))
            {
                if (tree.token.nary == 1)
                {
                    opKey = tree.token.str + "u";
                    nary = 1;
                }
                else if  (tree.token.nary == 2)
                {
                    opKey = tree.token.str + "b";
                    nary = 2;
                }
            }
            else
            {
                opKey = tree.token.str;
                nary = 2;
            }
            addOperation = true;
            break;
        }
        
        case expreval::FUNCTION :
        {
            opKey = tree.token.str;
            if (opKey.back() == '(') 
            {
                opKey.pop_back();
            }
            nary = (unsigned int) tree.children.size();
            addOperation = true;
            break;
        }
        
        case expreval::VARIABLE :
        {
            // Nothing to do
            break;
        }
        case expreval::NUMBER :
        {
            // Nothing to do
            break;
        }
        case expreval::COMMA :
        case expreval::PARENOPEN :
        case expreval::PARENCLOSE :
        case expreval::NUMBER_TOKEN_TYPE :
        default :
        {
            throw std::logic_error("Expression tree to compile contains an unexpected token type.");
        }
    }
    
    if (addOperation)
    {
        expreval::Operation op;
        
        // Checks
        if (OperatorMap.find(opKey) == OperatorMap.end())
        {
            std::string message = "Function with name " + opKey + " not found in operator map (not implemented).";
            throw std::range_error(message);
        }
        if (nary > expreval::MAX_NUMBER_OPERANDS)
        {
            std::string message = "Function with name " + opKey + " with " + std::to_string(nary) + " arguments exceeds maximum number of arguments MAX_NUMBER_OPERANDS = " + std::to_string(MAX_NUMBER_OPERANDS);
            throw std::range_error(message);
        }
        
        // Set function address
        op.function = OperatorMap[opKey];
        
        // Set return value address
        if (!_isSymbolNameRegistered(tree.symbolName))
        {
            throw std::logic_error("Return value symbol not found.");
        }
        expreval::Symbol& symRetVal = _symbolMap[tree.symbolName];
        op.vals[0] = symRetVal.address;
        
        // Set arguments addresses
        for (unsigned int k = 0; k < nary; k++)
        {
            if (!_isSymbolNameRegistered(tree.children.at(k).symbolName))
            {
                throw std::logic_error("Argument symbol not found.");
            }
            expreval::Symbol& symArgVal = _symbolMap[tree.children.at(k).symbolName];
            op.vals[k+1] = symArgVal.address;
        }
        
        _operations.push_back(op);
    }
}

void expreval::Compiler::Evaluate()
{
    if (!_alreadyCompiled)
    {
        throw std::runtime_error("Expression was not compiled.");
    }
    
    // Execute all operations
    for (auto opIt = std::begin(_operations); opIt != std::end(_operations); ++opIt)
    {
        opIt->function(opIt->vals);
    }
}

// Note: Register variables first
void expreval::Compiler::CompileExpression(std::string expr)
{
    // Build a vector of expressions
    auto _expressions = _split(expr, ';');
    for (auto itExpr = std::begin(_expressions); itExpr != std::end(_expressions); itExpr++)
    {
        // Parse expression
        expreval::Parser parser(*itExpr);
        auto tree = parser.parse();
        // Add sub expression tree
        AddExpressionTree(tree);
    }
    Compile();
}
