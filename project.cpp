/**
*
* Solution to course project # <номер на вариант>
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2020/2021
*
* @author Yordanka Stoyanova
* @idnumber 62512
* @compiler GCC
*
* <main file>
*
*/


#include <iostream>
#include <math.h>
#include <stack>
#include <string>
#include <fstream>

using namespace std;

//check if the input is invalid
int invalidInput (string s)
{
    //invalid input if the operator is followed by an operator ( except '(' and ')' )
    for (int i = 0; i < s.length() - 1; i++)
    {
        if ((s[i] == '+' || s[i] == '-' || s[i] == '/' || s[i] == '*' || s[i] == '^') &&
            (s[i+1] == '+' || s[i+1] == '-' || s[i+1] == '/' || s[i+1] == '*' || s[i+1] == '^'))
            return 1;
    }

    //invalid input if there are characters other than the following
    for (int i = 0; i < s.length(); i++)
    {
        if(s[i] != '+' && s[i] != '-' && s[i] != '/' && s[i] != '*' && s[i] != '^' && s[i] != '(' && s[i] != ')' &&
           s[i] != '0' && s[i] != '1' && s[i] != '2' && s[i] != '3' && s[i] != '4' && s[i] != '5' && s[i] != '6' &&
           s[i] != '7' && s[i] != '8' && s[i] != '9' && s[i] != ' ')
            return 1;
    }

    //invalid input if the first element is an operator other than '-' and '('
    if (s[0] == '+' || s[0] == '/' || s[0] == '*' || s[0] == '^' || s[0] == ')')
    {
        return 1;
    }

    //checking for spaces between digits
    for(int i = 0; i < s.length(); i++)
    {
        if (s[i] == '0' || s[i] == '1' || s[i] == '2' || s[i] == '3' || s[i] == '4' || s[i] == '5' || s[i] == '6'|| s[i] == '7'|| s[i] == '8'|| s[i] == '9')
        {
            if(s[i+1] == ' ')
            {
                int n = i + 2;
                while(s[n] == ' ')
                {
                    n++;
                }
                if (s[n] == '0' || s[n] == '1' || s[n] == '2' || s[n] == '3' || s[n] == '4' || s[n] == '5' || s[n] == '6'||s[n] == '7'|| s[n] == '8'|| s[n] == '9')
                {
                    return 1;
                }
            }
        }
    }

    //spaces between operators
    for(int i = 0; i < s.length(); i++)
    {
        if (s[i] == '+' || s[i] == '-' || s[i] == '/' || s[i] == '*' || s[i] == '^')
        {
            if(s[i+1] == ' ')
            {
                int n = i + 2;
                while(s[n] == ' ')
                {
                    n++;
                }

                if (s[n] == '+' || s[n] == '-' || s[n] == '/' || s[n] == '*' || s[n] == '^')
                {
                    return 1;
                }
            }
        }
    }

    //checking if all brackets are closed
    int brackets = 0;
    for(int i = 0; i < s.length(); i++)
    {
        if (s[i] == '(')
        {
            brackets++;
        }
        if (s[i] == ')')
        {
            brackets--;
        }
    }
    if (brackets != 0)
    {
        return 1;
    }

    return 0;
}

//check if char is a digit
bool isDigit (char c)
{
    return (c >= '0' && c <= '9');
}

//check if char is an operator
bool isOp (char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '^' || c == '/' || c == '(' || c == ')');
}

//determine the precedence of an operator
int getPrecedence (char c)
{
    switch (c)
    {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
        case '(':
        case ')': return 4;
        default: return -1;
    }
}

//evaluate an arithmetic expression
double operate (double val1, double val2, char op)
{
    if (op == '+')
        return val1 + val2;
    else if (op == '-')
        return val1 - val2;
    else if (op == '*')
        return val1 * val2;
    else if (op == '^')
        return pow(val1,val2);
    else
        return val1 / val2;
}

//evaluate a string
double evaluate (string s)
{
    stack<double> vals;
    stack<char> ops;

    double val = 0.0;
    int position = 0;


    while (position < s.length())
    {
        char spot = s[position];

        if (isDigit(spot))
        {
            val = (val * 10) + (double)(spot - '0');
        }

        else if (isOp(spot))
        {
            if (spot == '(')
            {
                ops.push(spot);
                val = 0;
            }
            else if(vals.empty())
            {
                vals.push(val);
                ops.push(spot);
                val = 0;
            }
            else if (spot == ')')
            {
                vals.push(val);
                while (ops.top() != '(')
                {
                    spot = ops.top();
                    ops.pop();
                    val = vals.top();
                    vals.pop();
                    double prev = vals.top();
                    vals.pop();
                    val = operate(prev, val, spot);
                    vals.push(val);
                }
                ops.pop();
                vals.pop();

            }
            else
            {
                char prev = ops.top();
                if (getPrecedence(spot) > getPrecedence(prev))
                {
                    vals.push(val);
                    ops.push(spot);
                    val = 0;
                }

                else
                {
                    if(ops.top() != '(')
                    {
                        double prev_val = vals.top();
                        vals.pop();
                        double prev_op = ops.top();
                        ops.pop();
                        prev_val = operate(prev_val, val, prev_op);
                        vals.push(prev_val);
                        ops.push(spot);
                        val = 0;
                    }
                    else
                    {
                        vals.push(val);
                        ops.push(spot);
                        val = 0;
                    }
                }
            }
        }
        position ++;
    }

    while (!ops.empty())
    {
        double prev = vals.top();
        vals.pop();
        char spot = ops.top();
        ops.pop();
        val = operate(prev, val, spot);
    }

    return val;
}

int main()
{
    ifstream file;

    file.open("Sample.txt");
    string firstRow;

    //checks to see if the file opens properly
    if (!file)
    {
        cout << "Error: Could not find the requested file." << endl;
        return -1;
    }
    else
    {
        getline(file, firstRow);
        file.close();
    }

    if (invalidInput(firstRow))
    {
        cout << "Error: Invalid input!" << endl;
    }
    else
    {
        cout << "Answer is: " << evaluate(firstRow) << endl;
    }

    return 0;
}
