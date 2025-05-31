#include "RlaT_Tokenizer.h"
#include <algorithm>
#include <unordered_set>
#include <regex>

namespace RlaT {
namespace internal {

std::vector<Token> Tokenizer::createTokensFromString(const std::string& line, int lineNumber) {
    std::vector<Token> result;
    
    // Make a copy of the sting and remove spaces and brackets, tabs etc. with std::isspace
    std::string unformatedLine = line;
    unformatedLine.erase(std::remove_if(unformatedLine.begin(), unformatedLine.end(), [](unsigned char c) {
        return std::isspace(c);
    }), 
    unformatedLine.end());

    // Tokenize
    size_t currentCharIndex = 0;
    size_t currentTokenStart = 0;
    bool inStringMode = false;
    while(currentCharIndex < unformatedLine.length()) {
        char& currentChar = unformatedLine[currentCharIndex];

        // Check if we need to switch into string-mode (ignore everything until a '"')
        if(currentChar == '\"') inStringMode = !inStringMode;
        if(inStringMode) {
            currentCharIndex++;
            continue;
        }

        // Create the Token if the currentChar is an operator
        if(SPLITTERS.count(currentChar)) { // 0 if not, 1 if yes     
            // Create one for the text before (if its not a lonely Operator)
            if(currentCharIndex != currentTokenStart) { // Current char isnt the token start
                result.push_back(          
                    Tokenizer::createToken(unformatedLine.substr(currentTokenStart, currentCharIndex - currentTokenStart), currentTokenStart, lineNumber)
                );
            }        
            // Create one for the splitter
            result.push_back(
                Tokenizer::createToken(std::string(1, currentChar), currentCharIndex, lineNumber)
            );
            

            currentCharIndex++;
            currentTokenStart = currentCharIndex;            
            continue;
        }

        // Just move forward until we find a operator or something
        currentCharIndex++;
    }

    return result;
}

Token Tokenizer::createToken(std::string content, int charNumber, int lineNumber) {
    return Token(content, Tokenizer::determineTokenType(content), lineNumber, charNumber);
}

TokenType Tokenizer::determineTokenType(std::string& s) {
    // Const check Values
    static const std::regex stringRegex("^\".*\"$");
    static const std::regex intRegex("^\\d*$");

    static const std::regex opAddRegex("^\\+$");
    static const std::regex opSubRegex("^-$");
    static const std::regex opMulRegex("^\\*$");
    static const std::regex opDivRegex("^/$");
    static const std::regex opSetRegex("^=$");

    static const std::regex opEndRegex("^;$");

    static const std::regex leftParenRegex("^\\($");
    static const std::regex rightParenRegex("^\\)$");
    static const std::regex leftBraceRegex("^\\{$");
    static const std::regex rightBraceRegex("^\\}$");
    static const std::regex leftBracketRegex("^\\[$");
    static const std::regex rightBracketRegex("^\\]$");

    static const std::regex identifierRegex("^[a-zA-Z_]+\\w*$");

    static const std::pair<std::regex, TokenType> checks[] = {
        {stringRegex, TokenType::STRING},
        {intRegex, TokenType::INTEGER},

        {opAddRegex, TokenType::OPERATOR_ADD},
        {opSubRegex, TokenType::OPERATOR_SUB},
        {opMulRegex, TokenType::OPERATOR_MUL},
        {opDivRegex, TokenType::OPERATOR_DIV},

        {opSetRegex, TokenType::OPERATOR_SET},
        {opEndRegex, TokenType::END_COMMAND},

        {leftParenRegex, TokenType::LEFT_PAREN},
        {rightParenRegex, TokenType::RIGHT_PAREN},
        {leftBraceRegex, TokenType::LEFT_BRACE},
        {rightBraceRegex, TokenType::RIGHT_BRACE},
        {leftBracketRegex, TokenType::LEFT_BRACKET},
        {rightBracketRegex, TokenType::RIGHT_BRACKET},

        {identifierRegex, TokenType::IDENTIFIER}
    };

    // Determine the Literal Type
    for(const std::pair<std::regex, TokenType> c : checks) {
        if(std::regex_match(s, c.first)) {
            return c.second;
        }
    }

    // No regex is matching
    return TokenType::INVALID;
}

}
}