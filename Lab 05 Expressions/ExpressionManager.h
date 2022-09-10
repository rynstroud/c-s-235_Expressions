//
//  ExpressionManager.cpp
//  Lab 05 Expressions
//
//  Created by C/R

#ifndef EXPRESSION_MANAGER_H
#define EXPRESSION_MANAGER_H
#include <stack>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include "ExpressionManagerInterface.h"

class ExpressionManager : public ExpressionManagerInterface
{
private:
    std::string expression;
    std::string postStr;
    std::stack<std::string> preFix;
    std::stack<std::string> inFix;
    int val;
    const std::string operators = "([{ -+  */%";
    const std::string realOp = "-+*/%";
    const std::string OPEN = "([{";
    const std::string CLOSE = ")]}";

public:
    ExpressionManager(void) {}
    ExpressionManager(std::string exp) : expression(exp), postStr(""), val(0) {}
    ~ExpressionManager(void) {}

    bool isBalanced();
    int value(void);
    std::string infix(void);
    std::string postfix(void);
    std::string toString(void) const;
    
    // Overrides the extraction operator
    friend std::ostream& operator<<(std::ostream& os, const ExpressionManager& myClass)
    {
       return os << myClass.toString();
    }
};

#endif    // EXPRESSION_MANAGER_H


//Checking to see if the expression's openers and closers are balanced
bool ExpressionManager::isBalanced()
{
    std::stack<char> s;
    bool balanced = true;
    string::const_iterator iter = expression.begin();
    while (balanced && (iter != expression.end()))
    {
        char nextCh = *iter;
        if (OPEN.find(nextCh) != std::string::npos) s.push(nextCh);
        else
        {
            if (CLOSE.find(nextCh) != std::string::npos)
            {
                if (s.empty()) balanced = false;
                else
                {
                    char topCh = s.top();
                    s.pop();
                    balanced = (OPEN.find(topCh) == CLOSE.find(nextCh));
                }
            }
        }
        ++iter;
    }
    return balanced && s.empty();
}


//Return the integer value of the infix expression
int ExpressionManager::value(void)
{
    std::stack<int> op; //operand stack
    std::stack<std::string> temp;
    int rightOp; //right operand
    int leftOp; //left operand
    int valInt; //int version of nextVal
    if (!isBalanced()) throw std::string("Unbalanced");
    std::istringstream iss(postStr);
    std::string nextVal = "";
    while (iss >> nextVal)
    {
        if (isdigit(nextVal.at(0)))
        {
            valInt = std::stoi (nextVal);
            op.push(valInt);
        }
        else if (operators.find(nextVal) != std::string::npos)
        {
            if (op.size() < 2) throw std::string("Missing Operand");
            else
            {
                rightOp = op.top();
                op.pop();
                leftOp = op.top();
                op.pop();
                
                //deals with each operator possible
                if (nextVal == "-")
                {
                    val = leftOp - rightOp;
                }
                else if (nextVal == "+")
                {
                    val = leftOp + rightOp;
                }
                else if (nextVal == "*")
                {
                    val = leftOp * rightOp;
                }
                else if (nextVal == "/")
                {
                    val = leftOp / rightOp;
                }
                else if (nextVal == "%")
                {
                    val = leftOp % rightOp;
                }
                op.push(val);
            }
        }
    }
    if (!op.empty()) val = op.top(); //covers cased of just one operand
    return val;
}

//Return the infix items from the expression
std::string ExpressionManager::infix(void)
{
    std::stack<std::string> s;
    std::stack<std::string> temp;
    std::string var;
    if (!isBalanced()) throw std::string("Unbalanced");
    std::istringstream iss(expression);
    std::string nextVal = "";
    while (iss >> nextVal)
    {
        inFix.push(nextVal);
        if ((OPEN.find(nextVal) != std::string::npos) || (CLOSE.find(nextVal) != std::string::npos))
        {
            continue;
        }
        temp.push(nextVal);
    }
    
    while (temp.size() > 0) 
    {
        if ((OPEN.find(temp.top()) != std::string::npos) || (CLOSE.find(temp.top()) != std::string::npos))
        {
            temp.pop();
            continue;
        }
        s.push(temp.top());
        temp.pop();
    }
    
    //accounts for expressions of one number
    if (s.size() == 1)
    {
        if (!isdigit(s.top().at(0))) throw std::string("Missing Operand");
        else return expression;
    }
    
    //sets var equal to first value on the stack so we can compare
    var = s.top();
    s.pop();
        
    if (s.empty()) return expression;
    if (!isdigit(var.at(0))) throw std::string("Missing Operand");
    while (s.size() > 1)
    {
        if (isdigit(var.at(0)))
        {
            //if ((operators.find(s.top().at(0)) == std::string::npos) && (!isdigit(s.top().at(0))))
            if (operators.find(s.top().at(0)) != std::string::npos)
            {
                var = s.top();
                s.pop();
                continue;
            }
            else if (isdigit(s.top().at(0)))
            {
                throw std::string("Missing Operator");
            }
            else
            {
                throw std::string("Illegal Operator");
            }
        }
        else if (realOp.find(var) != std::string::npos)
        {
            if (isdigit(s.top().at(0)))
            {
                var = s.top();
                s.pop();
                continue;
            }
            else
            {
                throw std::string("Missing Operand");
            }
        }
        else if ((!isdigit(var.at(0))) && (operators.find(var) == std::string::npos))
        {
            throw std::string("Illegal Operator");
        }
        else
        {
            throw std::string("Illegal Operator");
        }
    }
    
    //accounts for opening and closing braces
    if (!isdigit(s.top().at(0)))
    {
        if (OPEN.find(s.top()) == std::string::npos)
        {
            throw std::string("Missing Operand");
        }
    }

    return expression;
}

//Return a postfix representation of the infix expression
std::string ExpressionManager::postfix(void)
{
    if (!isBalanced()) return "Unbalanced";
    std::stack<std::string> op;
    std::stack<std::string> temp;
    
    //Copies the expression stack so it iterates forward
    while (inFix.size() > 0)
    {
        temp.push(inFix.top());
        inFix.pop();
        if (inFix.empty()) break;
    }
    
    while (temp.size() > 1)
    {
        //Pushes digits directly onto the string
        if (isdigit(temp.top().at(0)))
        {
            postStr = postStr + " " + temp.top();
            temp.pop();
        }
        
        
        else if (operators.find(temp.top()) != std::string::npos)
        {
            
            //Pushes openers directly onto the operator stack
            if (OPEN.find(temp.top()) != std::string::npos)
            {
                op.push(temp.top());
                temp.pop();
            }
            
            //deals with operators with lower precedence than what is already on the operator stack
            else if ((op.empty()) || (operators.find(temp.top()) / 4 > operators.find(op.top()) / 4))
            {
                op.push(temp.top());
                temp.pop();
            }
            
            //outputs the operator stack if it has higher precedence
            else if (operators.find(temp.top()) / 4 <= operators.find(op.top()) / 4)
            {
                while (operators.find(temp.top()) / 4 <= operators.find(op.top()) / 4)
                {
                    if (OPEN.find(op.top()) != std::string::npos)
                    {
                        op.pop();
                    }
                    else
                    {
                        postStr = postStr + " " + op.top();
                        op.pop();
                    }
                    if (op.empty()) break;
                }
                op.push(temp.top());
                temp.pop();
            }
        }
        
        //outputs operator stack until it finds the matching opening parenthesis
        else if (CLOSE.find(temp.top().at(0)) != std::string::npos)
        {
            int pos = CLOSE.find(temp.top());
            while (op.top().at(0) != OPEN.at(pos))
            {
                if (OPEN.find(op.top().at(0)) != std::string::npos)
                {
                    op.pop();
                    if (op.empty()) break;
                    continue;
                }
                postStr = postStr + " " + op.top();
                op.pop();
                if (op.empty()) break;
            }
            op.pop();
            if ((!op.empty()) && (OPEN.find(op.top()) == std::string::npos))
            {
                postStr = postStr + " " + op.top();
                op.pop();
            }
            temp.pop();
        }
        
        //just in case
        else
        {
            return "Wait how are you here???";
        }
    }
    
    //deals with the last value and whats left on the operator stack
    if (temp.size() == 1)
    {
        if (CLOSE.find(temp.top()) == std::string::npos)
        {
            postStr = postStr + " " + temp.top();
        }
        else temp.pop();
        while (!op.empty())
        {
            if (OPEN.find(op.top()) != std::string::npos)
            {
                op.pop();
                continue;
            }
            postStr = postStr + " " + op.top();
            op.pop();
        }
    }
    return postStr;
}

//Return the infix vector'd expression items
std::string ExpressionManager::toString(void) const
{
    try
    {
        std::ostringstream os;
        if (expression != "")
        {
            os << "Expression: " << expression;
            return os.str();
        }
        else
        {
            throw std::string ("Empty object i think babeyyyy");
        }
    } catch (std::string& errorString)
    {
        return errorString;
    }
}
