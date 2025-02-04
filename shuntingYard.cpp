#include <deque>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>

#include "io.h"

using namespace std;

bool isNumeric(string token)
{
   if (!token.empty() && (isdigit(token[0]) || token[0] == '.'))
   {
      return true;
   }
   return token.size() > 1 && (isdigit(token[1]) || token[1] == '.');
}

// Removes unary negation operator (-) and whitespace
deque<string> tokenize(string inputString)
{
   deque<string> tokens;
   string valueTokenBuffer = "";

   for (size_t i = 0; i < inputString.length(); i++)
   {
      const string symbol(1, inputString[i]);
      const size_t lastIndex = inputString.length() - 1;

      if (isNumeric(symbol))
      {
         valueTokenBuffer += inputString[i];
      }

      if (!isNumeric(symbol) || i == lastIndex)
      {
         if (!valueTokenBuffer.empty())
         {
            tokens.push_back(valueTokenBuffer);
            valueTokenBuffer.clear();
         }

         if (symbol == "-" && (tokens.empty() || (!isNumeric(tokens.back()) &&
                                                  tokens.back() != ")")))
         {
            valueTokenBuffer = '-';
         }
         else if (symbol != " " && (i != lastIndex || symbol == ")"))
         {
            tokens.push_back(symbol);
         }
      }
   }
   return tokens;
}

deque<string> shuntingYard(deque<string> inputStack)
{
   stack<char> operatorStack;
   deque<string> outputQueue;
   unordered_map<char, size_t> operatorRank;
   operatorRank['+'] = 1;
   operatorRank['-'] = 2;
   operatorRank['*'] = 3;
   operatorRank['/'] = 4;
   operatorRank['('] = 0;

   for (size_t i = 0; i < inputStack.size(); i++)
   {

      if (isNumeric(inputStack[i]))
      {
         outputQueue.push_back(inputStack[i]);
         continue;
      }

      const char operatorChar = inputStack[i][0];

      if (operatorChar == ')')
      {
         while (operatorStack.top() != '(')
         {
            std::string operatorString(1, operatorStack.top());
            outputQueue.push_back(operatorString), operatorStack.pop();
         }
         operatorStack.pop();
         continue;
      }

      while (operatorChar != '(' && !operatorStack.empty() &&
             operatorRank.at(operatorChar) <=
                 operatorRank.at(operatorStack.top()))
      {
         std::string operatorString(1, operatorStack.top());
         outputQueue.push_back(operatorString), operatorStack.pop();
      }

      operatorStack.push(operatorChar);
   }

   while (!operatorStack.empty())
   {
      std::string operatorString(1, operatorStack.top());
      outputQueue.push_back(operatorString), operatorStack.pop();
   }

   return outputQueue;
}

double evalReversePolishNotation(deque<string> rpn)
{
   stack<double> result;

   for (const string &token : rpn)
   {
      if (isNumeric(token) || (token.length() > 1 && isNumeric(token)))
      {
         result.push(stod(token));
      }
      else
      {
         const double operandB = result.top();
         result.pop();
         const double operandA = result.top();
         result.pop();

         switch (token[0])
         {
         case '+':
            result.push(operandA + operandB);
            break;
         case '-':
            result.push(operandA - operandB);
            break;
         case '*':
            result.push(operandA * operandB);
            break;
         case '/':
            result.push(operandA / operandB);
            break;

         default:
            print("ERROR: switch statement");
            return -1;
         }
      }
   }
   return result.top();
}

int main()
{
   //    const string inputString = getString("Enter Expression: ");
   const double test =
       (((-6.3 / 2.1) + (5.7 - (-3.4))) * (4.9 / (-2.2))) - 7.8;
   const string inputString =
       "(((-6.3 / 2.1) + (5.7 - (-3.4))) * (4.9 / (-2.2))) - 7.8";
   const deque<string> tokens = tokenize(inputString);
   const deque<string> rpn = shuntingYard(tokens);
   const double result = evalReversePolishNotation(rpn);

   print("Result: " + to_string(result) + '\n');
   print("Test: " + to_string(test));
}
