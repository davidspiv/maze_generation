#include <deque>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>

#include "io.h"

using namespace std;

struct Token
{
   string text;
   bool isNumeric;
};

bool isNumeric(char ch)
{
   return isdigit(ch) || ch == '.';
}

// Separates operators and numerics. Also removes unary negation operator (-)
// and whitespace.
deque<Token> tokenize(string inputString)
{
   deque<Token> tokens;
   string valueTokenBuffer = "";

   for (size_t i = 0; i < inputString.length(); i++)
   {
      const char symbol = inputString[i];
      const size_t lastIndex = inputString.length() - 1;

      if (isNumeric(symbol))
      {
         valueTokenBuffer += inputString.substr(i, 1);
      }

      if (!isNumeric(symbol) || i == lastIndex)
      {
         if (valueTokenBuffer.length())
         {
            tokens.push_back({valueTokenBuffer, 1});
            valueTokenBuffer.clear();
         }

         if (symbol == '-' && (!i || !tokens.back().isNumeric))
         {
            valueTokenBuffer = '-' + valueTokenBuffer;
         }
         else if (symbol != ' ' && i != lastIndex)
         {
            const string operatorString(1, symbol);

            tokens.push_back({operatorString, 0});
         }
      }
   }
   return tokens;
}

// Outputs result in reverse polish notation
deque<string> shuntingYard(deque<Token> inputStack)
{
   stack<char> operatorStack;
   deque<string> outputQueue;
   unordered_map<char, size_t> operatorMap;
   operatorMap['+'] = 1;
   operatorMap['-'] = 2;
   operatorMap['*'] = 3;
   operatorMap['/'] = 4;
   operatorMap['('] = 0;

   // Tokenizes AND directs tokens within one loop of the input string.
   // Should be separated for more robust implementation (ie. to allow for
   // multiple character operators)
   for (size_t i = 0; i < inputStack.size(); i++)
   {
      const char tokenChar = inputStack[i].text[0];

      if (tokenChar == ')')
      {
         while (operatorStack.top() != '(')
         {
            std::string operatorString(1, operatorStack.top());
            outputQueue.push_back(operatorString), operatorStack.pop();
         }
         operatorStack.pop();
         continue;
      }

      while (tokenChar != '(' && !operatorStack.empty() &&
             operatorMap.at(tokenChar) <= operatorMap.at(operatorStack.top()))
      {
         std::string operatorString(1, operatorStack.top());
         outputQueue.push_back(operatorString), operatorStack.pop();
      }

      operatorStack.push(tokenChar);
   }

   // FLUSH OPERATOR STACK
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
      if (isNumeric(token[0]) || (token.length() > 1 && isNumeric(token[1])))
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
   //   const string inputString = getString("Enter Expression: ");
   //    const double test =
   //        (((-6.3 / 2.1) + (5.7 - (-3.4))) * (4.9 / (-2.2))) - 7.8;
   const string inputString = "1--1";
   const deque<Token> tokens = tokenize(inputString);
   const deque<string> rpn = shuntingYard(tokens);

   for (string token : rpn)
   {
      print(token);
   }
   //    const double result = evalReversePolishNotation(rpn);

   //    print("Result: " + to_string(result) + '\n');
   //    print("Test: " + to_string(test));
}
