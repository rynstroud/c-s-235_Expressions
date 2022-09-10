//
//  main.cpp
//  Lab 05 Expressions
//
//  Created by C/R

#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <vector>
#include "ExpressionManager.h"

std::string getCommand(std::string line)
{
    size_t r = 0;
    while(isalpha(line[r])) ++r;
    std::string command = line.substr(0, r);
    return command;
}

int main(int argc, const char * argv[]) {
    //Checking for correct number of arguments
    if (argc < 3)
    {
        std::cerr << "Please provide the name of input and output files, thanks\n";
        return 1;
    }
       
    //Checking if input file opens
    std::ifstream in(argv[1]);
    if (!in)
    {
        std::cerr << "Failure to open input file: " << argv[1] << std::endl;
        return 2;
    }
       
    //Checking if output file opens
    std::ofstream out(argv[2]);
    if (!out)
    {
        in.close();
        std::cerr << "Failure to open output file: " << argv[1] << std::endl;
        return 3;
    }
    
    std::vector<ExpressionManager> expList;
    std::string initCommand = " ";
       for (std::string line; getline(in, line);)
        {
            std::istringstream iss(line);
            try
            {
                std::string command = getCommand(line);
                out << command;
                
                if (command == "Expression")
                {
                    size_t p = 0;
                    while(isalpha(line[p])) ++p;
                    std::string expStr = line.substr(p + 2, line.size());
                    ExpressionManager expression(expStr);
                    expList.push_back(expression);
                    out << ": " << expStr << std::endl;
                    continue;
                }

                if (line == "Infix:")
                {
                    try {
                        {
                            if (!expList.empty())
                            {
                                out << ": " << expList.back().infix();
                            }
                            else
                            {
                                throw std::string(" Empty!");
                            }
                        }
                    } catch (std::string errorString)
                    {
                        out << errorString;
                    }
                }
                
                else if (line == "Postfix:")
                {
                    try
                    {
                        if (!expList.empty())
                        {
                            out << ": " << expList.back().postfix();
                        }
                        else
                        {
                            throw std::string(" Empty!");
                        }
                    } catch (std::string errorString)
                    {
                        out << " " << errorString;
                    }
                }
                
                else if (line == "Value:")
                {
                    if (!expList.empty())
                    {
                        out << ": " << expList.back().value();
                    }
                    else
                    {
                        throw std::string(" Empty!");
                    }
                }
                
                else if (line == "Prefix:")
                {
                    if (!expList.empty())
                    {
                        out << ": " << expList.back().prefix();
                    }
                    else
                    {
                        throw std::string(" Empty!");
                    }
                }
                
                else
                {
                    throw std::string("**Error: " + line);
                }
                out << std::endl;
            } catch(std::string errorString)
                {
                    out <<  errorString << std::endl;
                }

        }
    
    return 0;
}
