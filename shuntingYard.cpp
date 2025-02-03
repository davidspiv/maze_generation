#include <deque>
#include <iostream>
#include <stack>
#include <string>

#include "io.h"

using namespace std;

bool isNumericChar(char ch) { return isdigit(ch) || ch == '.'; }

int getOperatorPrecedence(string nonNumeric) {
  switch (nonNumeric[0]) {
    case '+':
      return 1;
    case '-':
      return 2;
    case '*':
      return 3;
    case '/':
      return 4;
    case '(':
      return 0;

    default:
      print("ERROR: unrecognized nonNumeric");
      return -1;
  }
}

deque<string> shuntingYard(string inputString) {
  stack<string> holding;
  deque<string> output;
  string valueBuffer = "";
  bool negativeFlag = false;
  bool operatorFlag = false;

  for (size_t i = 0; i < inputString.length(); i++) {
    if (isNumericChar(inputString[i])) {
      valueBuffer += inputString.substr(i, 1);
      operatorFlag = false;
    } else {
      const string nonNumeric(1, inputString[i]);

      // HANDLE VALUE
      if (valueBuffer.length()) {
        if (negativeFlag) {
          valueBuffer = '-' + valueBuffer;
        }
        negativeFlag = false;
        output.push_back(valueBuffer);
        valueBuffer.clear();
      }

      // HANDLE NON-NUMERICS
      if (nonNumeric == "-" && operatorFlag) {
        negativeFlag = true;
        continue;
      }

      if (nonNumeric == ")") {
        while (holding.top() != "(") {
          output.push_back(holding.top()), holding.pop();
        }
        holding.pop();
        continue;
      }

      operatorFlag = true;

      while (nonNumeric != "(" && !holding.empty() &&
             getOperatorPrecedence(nonNumeric) <=
                 getOperatorPrecedence(holding.top())) {
        output.push_back(holding.top()), holding.pop();
      }
      holding.push(nonNumeric);
    }
  }

  // HANDLE LAST VALUE
  if (valueBuffer.length()) {
    if (negativeFlag) {
      valueBuffer = '-' + valueBuffer;
    }
    output.push_back(valueBuffer);
  }

  // FLUSH HOLDING STACK
  while (!holding.empty()) {
    output.push_back(holding.top()), holding.pop();
  }

  return output;
}

double evalReversePolishNotation(deque<string> rpn) {
  stack<double> result;

  for (const string &token : rpn) {
    if (isNumericChar(token[0]) ||
        (token.length() > 1 && isNumericChar(token[1]))) {
      result.push(stod(token));
    } else {
      const double operandB = result.top();
      result.pop();
      const double operandA = result.top();
      result.pop();

      switch (token[0]) {
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

int main() {
  const double answer = (4 - 3 + -2 * 7) + 8 / (6 + -0.003) / -2.2;
  const string inputString = "(4-3+-2*7)+8/(6+-0.003)/-2.2";

  const deque<string> rpn = shuntingYard(inputString);
  const double result = evalReversePolishNotation(rpn);

  print("CALC: " + to_string(result) + '\n');

  print("ANSWER: " + to_string(answer) + '\n');
}
