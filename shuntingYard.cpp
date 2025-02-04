#include <deque>
#include <iostream>
#include <stack>
#include <stdexcept>
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
deque<string> tokenize(const string &inputAsString)
{
   deque<string> tokens;
   string valueTokenBuffer = "";

   for (size_t i = 0; i < inputAsString.length(); i++)
   {
      const string symbol(1, inputAsString[i]);
      const size_t lastIndex = inputAsString.length() - 1;

      if (isNumeric(symbol))
      {
         valueTokenBuffer += inputAsString[i];
      }

      if (!isNumeric(symbol) || i == lastIndex)
      {
         if (!valueTokenBuffer.empty())
         {
            tokens.push_back(valueTokenBuffer);
            valueTokenBuffer.clear();
         }

         if ((symbol == "-" && tokens.empty()) ||
             (symbol == "-" && (tokens.empty() || (!isNumeric(tokens.back()) &&
                                                   tokens.back() != ")"))))
         {
            valueTokenBuffer = '-';
         }
         else if (symbol != " " && (!isNumeric(symbol) || symbol == ")"))
         {
            tokens.push_back(symbol);
         }
      }
   }
   return tokens;
}

bool validAlgebraicNotation(deque<string> algebraicNotation)
{
   int openParentheses = 0;
   size_t previousNumeric = 0;
   size_t previousOperator = 0;
   string errorMessage;

   if (algebraicNotation.empty())
   {
      errorMessage = "empty input";
   }
   else if (!isNumeric(algebraicNotation.front()) &&
            algebraicNotation.front() != "(")
   {
      errorMessage = "starts with an operator";
   }
   else if (!isNumeric(algebraicNotation.back()) &&
            algebraicNotation.back() != ")")
   {
      errorMessage = "ends with an operator";
   }
   else
   {

      for (string token : algebraicNotation)
      {
         if (token == "(")
         {
            ++openParentheses;
         }
         else if (token == ")")
         {
            --openParentheses;
         }
         else if (isNumeric(token))
         {
            size_t decimalCount = 0;
            for (char ch : token)
            {
               if (ch == '.')
               {
                  ++decimalCount;
               }
            }
            if (token.length() == 1 && decimalCount)
            {
               errorMessage = "isolated decimal";
               break;
            }

            if (decimalCount > 1)
            {
               errorMessage = "multiple decimals";
               break;
            }
            ++previousNumeric;
            if (previousOperator)
            {
               --previousOperator;
            }
         }
         else
         {
            ++previousOperator;
            if (!previousNumeric)
            {
               errorMessage = "consecutive operators";
               break;
            }
            --previousNumeric;
         }

         if (openParentheses < 0)
         {
            errorMessage = "missing open parenthesis";
            break;
         }

         if (previousNumeric > 1)
         {
            errorMessage = "consecutive numeric values";
            break;
         }
      }
   }

   if (!errorMessage.length() && openParentheses)
   {
      errorMessage = "missing closing parenthesis";
   }

   if (errorMessage.length())
   {
      print("Invalid Expression: " + errorMessage);
      return false;
   }
   return true;
}

deque<string> shuntingYard(deque<string> inputQueue)
{
   stack<char> operatorStack;
   deque<string> outputQueue;
   unordered_map<char, size_t> operatorRank;
   operatorRank['+'] = 1;
   operatorRank['-'] = 1;
   operatorRank['*'] = 2;
   operatorRank['/'] = 2;
   operatorRank['('] = 0;

   for (string &token : inputQueue)
   {

      if (isNumeric(token))
      {
         outputQueue.push_back(token);
         continue;
      }

      const char operatorAsChar = token[0];

      if (operatorAsChar == ')')
      {
         while (operatorStack.top() != '(')
         {

            outputQueue.push_back(string(1, operatorStack.top())),
                operatorStack.pop();
         }
         operatorStack.pop();
         continue;
      }

      while (operatorAsChar != '(' && !operatorStack.empty() &&
             operatorRank.at(operatorAsChar) <
                 operatorRank.at(operatorStack.top()))
      {

         outputQueue.push_back(string(1, operatorStack.top())),
             operatorStack.pop();
      }

      operatorStack.push(operatorAsChar);
   }

   while (!operatorStack.empty())
   {

      outputQueue.push_back(string(1, operatorStack.top())),
          operatorStack.pop();
   }

   return outputQueue;
}

double evalExpression(deque<string> reversePolishNotation)
{
   stack<double> result;

   for (const string &token : reversePolishNotation)
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
            if (!operandB && operandA)
            {
               throw invalid_argument("ERROR: unable to divide by zero");
            }
            result.push(operandA / operandB);

         default:
            throw invalid_argument("ERROR: unrecognized non-numeric");
         }
      }
   }
   return result.top();
}

int main()
{
   deque<string> algebraicNotation;
   do
   {
      const string inputAsString = getString("Enter Expression: ");
      algebraicNotation = tokenize(inputAsString);
   } while (!validAlgebraicNotation(algebraicNotation));

   const deque<string> reversePolishNotation = shuntingYard(algebraicNotation);

   try
   {
      const double result = evalExpression(reversePolishNotation);
      print("Result: " + to_string(result));
   }
   catch (const std::exception &e)
   {
      std::cerr << e.what() << '\n';
   }
}
