// deque<string> tokenizer(string inputString)
// {
//    deque<string> tokens;
//    string valueTokenBuffer = "";
//    bool previousTokenIsOperator = false;
//    bool isNegative = false;

//    for (size_t i = 0; i < inputString.length(); i++)
//    {

//       const char symbol = inputString[i];
//       const string token(1, symbol);

//       if (isNumeric(symbol) || i == inputString.length() - 1)
//       {
//          valueTokenBuffer += inputString.substr(i, 1);
//          previousTokenIsOperator = false;
//       }
//       else
//       {
//          // HANDLE VALUE TOKEN
//          if (valueTokenBuffer.length())
//          {
//             if (isNegative)
//             {
//                valueTokenBuffer = '-' + valueTokenBuffer;
//                isNegative = false;
//             }
//             tokens.push_back(valueTokenBuffer);
//             valueTokenBuffer.clear();
//          }

//          // HANDLE NON-NUMERIC TOKEN
//          if (symbol == '-' && previousTokenIsOperator)
//          {
//             isNegative = true;
//             continue;
//          }

//          previousTokenIsOperator = true;
//          tokens.push_back(token);
//       }
//    }

//    // HANDLE LAST VALUE TOKEN
//    if (valueTokenBuffer.length())
//    {
//       if (isNegative)
//       {
//          valueTokenBuffer = '-' + valueTokenBuffer;
//       }
//       tokens.push_back(valueTokenBuffer);
//    }
//    return tokens;
// }
