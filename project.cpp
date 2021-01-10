/**
*
* Solution to course project # 02
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

//checks if char is a digit
bool isDigit (char c)
{
    return (c >= '0' && c <= '9');
}

//checks if char is an operator
bool isOp (char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '^' || c == '/' || c == '(' || c == ')');
}

//checks if the input is invalid
int invalidInput (string s)
{
    //checks if there are characters other than the following
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] != '+' && s[i] != '-' && s[i] != '/' && s[i] != '*' && s[i] != '^' && s[i] != '(' && s[i] != ')' &&
           s[i] != '0' && s[i] != '1' && s[i] != '2' && s[i] != '3' && s[i] != '4' && s[i] != '5' && s[i] != '6' &&
           s[i] != '7' && s[i] != '8' && s[i] != '9' && s[i] != ' ')
            return 1;
    }

    //checks if the brackets are empty and if we have an operator after '('
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == '(')
        {
            int n = i + 1;
            while (s[n] == ' ')
            {
                n++;
            }
            if (s[n] == ')' || s[n] == '*' || s[n] == '/' || s[n] == '+' || s[n] == '^')
            {
                return 1;
            }
        }
    }

    //dividing by 0
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == '/')
        {
            int n = i + 1;
            while (s[n] == ' ')
            {
                n++;
            }
            if (s[n] == '0')
            {
                return 1;
            }
        }
    }

    //checks if string contains only spaces
    int character = 0;
    while (s[character] == ' ')
    {
        character++;
    }
    // - 3 because i am adding '+0' at the end of the string
    if (character > s.length() - 3)
    {
        return 1;
    }

    //checks if the first element is an operator other than '-' and '('
    if (s[0] == '+' || s[0] == '/' || s[0] == '*' || s[0] == '^' || s[0] == ')')
    {
        return 1;
    }

    //checks for spaces between numbers
    for (int i = 0; i < s.length(); i++)
    {
        if (isDigit(s[i]))
        {
            if (s[i+1] == ' ')
            {
                int n = i + 2;
                while (s[n] == ' ')
                {
                    n++;
                }
                if (isDigit(s[n]) || s[n] == '(')
                {
                    return 1;
                }
            }
        }
    }

    //checks if the operator is followed by another operator (except '(')
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == '+' || s[i] == '-' || s[i] == '/' || s[i] == '*' || s[i] == '^')
        {
            int n = i + 1;
            while (s[n] == ' ')
            {
                n++;
            }

            if (s[n] == '+' || s[n] == '-' || s[n] == '/' || s[n] == '*' || s[n] == '^' || s[n] == ')')
            {
                return 1;
            }
        }
    }

    //checks if string is empty
    if (s.empty())
    {
        return 1;
    }

//    checks if brackets are balanced
    stack<char> brackets_stack;

    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == '(')
        {
            brackets_stack.push(s[i]);
        }
        else if (s[i] == ')')
        {
            if (brackets_stack.empty())
            {
                return 1;
            }
            else
            {
                brackets_stack.pop();
            }
        }
    }
    if (!brackets_stack.empty())
    {
        return 1;
    }

    return 0;
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
            else if (vals.empty())
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
                    if (ops.top() != '(')
                    {
                        //example: 100 / (5-5)
                        if (ops.top() == '/' && val == 0){
                            cout << "NaN";
                            exit(-1);
                        }
                        else
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

    stack<double> opposite_vals;
    stack<char> opposite_ops;

    vals.push(val);

    //creating stacks where the operators and numbers are in the right order
    while (!vals.empty())
    {
        double transfer = 0;
        transfer = vals.top();
        opposite_vals.push(transfer);
        vals.pop();
    }

    while (!ops.empty())
    {
        char transfer_ops = 0;
        transfer_ops = ops.top();
        opposite_ops.push(transfer_ops);
        ops.pop();
    }

    //calculating until there are no operators left in the operators stack
    val = opposite_vals.top();
    opposite_vals.pop();

    while (!opposite_ops.empty())
    {
        double second_val = opposite_vals.top();
        opposite_vals.pop();
        char op = opposite_ops.top();
        val = operate(val, second_val, op);
        opposite_ops.pop();
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
        cout << "NaN" << endl;
        return -1;
    }
    else
    {
        getline(file, firstRow);
        file.close();
    }

    firstRow.resize(firstRow.size()+1,'+');
    firstRow.resize(firstRow.size()+1,'0');

    if (invalidInput(firstRow))
    {
        cout << "NaN" << endl;
    }
    else
    {
        cout << evaluate(firstRow) << endl;
    }

    return 0;
}
