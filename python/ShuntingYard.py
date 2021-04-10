#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Apr  2 09:17:09 2021

reference see:
    https://www.reedbeta.com/blog/the-shunting-yard-algorithm/
    http://www.engr.mun.ca/~theo/Misc/exp_parsing.htm
    http://journal.stuffwithstuff.com/2011/03/19/pratt-parsers-expression-parsing-made-easy/

@author: basile
"""
import re
import collections

# Patterns for each tokens (order is important)
patterns = ((r",",                        'COMMA'),
            (r"[a-zA-Z]+\w*\s*\(",        'FUNCTION'),
            (r"[a-zA-Z]+\w*(?:\[\d+\])*", 'VARIABLE'),
            (r"\d*\.\d+|\d+\.\d*|\d+",'NUMBER'),
            (r"\+|\-|\*|/",               'OPARITHM'),
            (r"\(",                       'PARENOPEN'),
            (r"\)",                       'PARENCLOSE'))

# Full pattern to detect any of the above tokens
fullpattern = r"(" + patterns[0][0]
for p in patterns[1:]:
    fullpattern += r"|" + p[0]
fullpattern += r")"

# compile full pattern and sub patterns
tokre = re.compile(fullpattern)
toksre = []
for p in patterns:
    toksre.append((re.compile(p[0]), p[1]))
           

# Arithmetic Operator precedence
precedence = {r"+":1, r"-":1 , r"*":3, r"/":3}

# Simple token class
class Token:
    def __init__(self, tkStr = None):
        self.str = tkStr
        if tkStr != None:
            # Match token type
            for tre in toksre:
                if tre[0].match(tkStr) != None:
                    self.type = tre[1]
                    break
            if self.type == 'OPARITHM':
                self.prec = precedence[self.str]
                self.nary = 2
            elif self.type == 'PARENOPEN':
                self.prec = 0 # lowest precedence
                self.nary = 1 # ?
            else:
                self.prec = -1
                self.nary = -1
            self.funargs = [] # list of arguments in case of function token

     
def tokenize(sexpr):
    tt = tokre.findall(sexpr)
    tok = []
    prev = Token()
    prev.type = 'NONE'
    for t in tt:
        newToken = Token(t)
        # Determine nary for arithmetic operators
        if newToken.type == 'OPARITHM':
            newToken.nary = 2
            if (newToken.str == '-') | (newToken.str == '+'):
                if (prev.type == 'OPARITHM') | (prev.type == 'COMMA') | (prev.type == 'FUNCTION') | (prev.type == 'PARENOPEN') | (prev.type == 'NONE'):
                    newToken.nary = 1
                    newToken.prec = 2
        tok.append(newToken)
        prev = newToken
    return tok




sexpr = r"sin( -max ( a2_a[4], (3.14 + 2) ) / 3 * pi + .2* 5.)"
sexpr = r"3 * 4 + 5 * 6"
#sexpr = r"+3 + 4 * 5++++++6"
sexpr = r"-5-6*-2/4"
sexpr = r"3*4+1+6*1"
sexpr = r"3*(4+(1+6)*1)"

sexpr = r"3*f(x+1,2*(-sin(z) + 1+3))+2"

#sexpr = r"1 + 2 + 3 + 4 + 5 + 6 + 7"


    
myNewTokens = tokenize(sexpr)
for mt in myNewTokens:
    print("%s\t\t%s\t\t%d" % (mt.str, mt.type, mt.nary))
    
    
class ShuntingYard:
    def __init__(self, expr):
        self.expr = expr
        self.operators = collections.deque()
        self.operands = collections.deque()
        self.tokens = tokenize(expr)
        
    # Build a sub-tree from an operator and list of operands (i.e. apply operator to operands)
    def apply(self, optor, opands):
        tree = optor.str + "(" 
        cma = ''
        for opa in reversed(opands):
            tree += cma + opa
            cma = ','
        tree += ")"
        return tree
        
    # Pop operator and operands from stacks and apply. Push result to operand stack
    def popAndApply(self):
        optor = self.operators.pop()
        opands = []
        for k in range(optor.nary):
            opands.append(self.operands.pop())
        tree = self.apply(optor, opands)
        self.operands.append(tree)
        
    def applyFunction(self, tokFun):
        funTree = r"FUN::" + tokFun.str
        for arg in tokFun.funargs:
            funTree += r"{" + arg + r"}"
        funTree += r")"
        self.operands.append(funTree)
        
    def parse(self):
        self.operators = collections.deque()
        self.operands = collections.deque()
        # Read tokens from eft to right
        for tok in self.tokens:
            
            # If token is an operand, push it to the operand stack
            if (tok.type == 'NUMBER') | (tok.type == 'VARIABLE'):
                self.operands.append(tok.str)
                
            # If token is an operator:
            elif tok.type == 'OPARITHM':
                # While there's an optor with precedence >= the precedence of 
                # the token, pop it and pop it's opands, apply it and push 
                # result to the opand stack
                while (len(self.operators)>0):
                    topopeator = self.operators[-1]
                    if (topopeator.prec >= tok.prec):
                        # Additional condition for unary operators
                        if (tok.nary == 1) & (topopeator.nary != 1) :
                            break
                        self.popAndApply()
                    else:
                        break
                
                # Then push the token (operator) to the operator stack
                self.operators.append(tok)
                
                
            elif tok.type == 'FUNCTION':
                # Push function to operator stack (along with it to-be-filled argument list)
                self.operators.append(tok) 
                
            elif tok.type == 'COMMA':
                while self.operators[-1].type != 'FUNCTION':
                    self.popAndApply() 
                # The top of the operand stack is now the next argument for that function call, add it to it list
                self.operators[-1].funargs.append(self.operands.pop())
           
            elif tok.type == 'PARENOPEN':
                # Push on operator stack
                self.operators.append(tok)
                
            elif tok.type == 'PARENCLOSE':
                # Pop and apply until we see a "(" or "fun("
                while (self.operators[-1].type != 'PARENOPEN') & (self.operators[-1].type != 'FUNCTION'):
                    self.popAndApply()
                # Open paren "(" or "fun(" is on the top now
                topOp = self.operators.pop()
                # If "(" discard, if "fun(" apply its arg list and push to operands
                if topOp.type == 'FUNCTION':
                    # The top of the operand stack is now the next argument for that function call, add it to it list
                    topOp.funargs.append(self.operands.pop())
                    # Add function call to the operand tree
                    self.applyFunction(topOp)
                  
                    

                                
                
        
        # When all tokens are read, pop and apply all remaining optors (and 
        # opands)
        while len(self.operators) > 0:
            self.popAndApply()
        
        
sy = ShuntingYard(sexpr)        
sy.parse()
print(sy.operators)
print(sy.operands)
