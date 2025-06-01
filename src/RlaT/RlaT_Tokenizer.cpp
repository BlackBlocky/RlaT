#include "RlaT_Tokenizer.h"
#include <algorithm>
#include <unordered_set>
#include <regex>
#include <unordered_map>

namespace RlaT {
namespace internal {

std::vector<Token> Tokenizer::createTokensFromString(const std::string& line, int lineNumber) {
    std::vector<Token> result;
    
    // Make a copy of the sting and remove spaces and brackets, tabs etc. with std::isspace, while keeping them in strings
    std::string unformatedLine = removeFormattingExceptInStrings(line);

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
            // Create one for the splitter (Dont do it for Spaces)
            if(currentChar != ' ') {
                result.push_back(
                    Tokenizer::createToken(std::string(1, currentChar), currentCharIndex, lineNumber)
                );
            }        

            currentCharIndex++;
            currentTokenStart = currentCharIndex;            
            continue;
        }

        // Just move forward until we find a operator or something
        currentCharIndex++;
    }

    return result;
}

std::string Tokenizer::removeFormattingExceptInStrings(const std::string& s) {
    std::string result;
    bool insideString = false;

    // Remove formatting in the line of code (!!Strings are current also effected)
    std::string noTabsStrings = s;
    std::replace(noTabsStrings.begin(), noTabsStrings.end(), '\t', ' ');
    std::replace(noTabsStrings.begin(), noTabsStrings.end(), '\n', ' ');

    for(size_t i = 0; i < noTabsStrings.size(); i++) {
        char c = noTabsStrings[i];
        // Check if we hit a ", while keeping escape sequenzes in mind
        if(c == '\"' && ((i == 0) || (i > 0 && noTabsStrings[i - 1] != '\\'))) insideString = !insideString;

        // We are inside a String -> Keep Formatting
        if(insideString) {
            if(c == '\\') continue;
            result += c;
            continue;
        }

        // We are outside a String -> Remove Formatting
        // If it is a Space, only remove it when there is already a Space left from it
        if(!std::isspace(static_cast<unsigned char>(c))) result += c;
        else if(i != 0 && c == ' ' && !std::isspace(static_cast<unsigned char>(noTabsStrings[i-1]))) result += c;
    }

    return result;
}

Token Tokenizer::createToken(const std::string& content, int charNumber, int lineNumber) {
    return Token(content, Tokenizer::determineTokenType(content), lineNumber, charNumber);
}

TokenType Tokenizer::determineTokenType(const std::string& s) {
    // Const check Values
    static const std::regex stringRegex("^\".*\"$");
    static const std::regex intRegex("^\\d*$");

    static const std::regex identifierRegex("^[a-zA-Z_]+\\w*$");

    static const std::pair<std::regex, TokenType> checks[] = {
        {stringRegex, TokenType::STRING},
        {intRegex, TokenType::INTEGER},

        {identifierRegex, TokenType::IDENTIFIER}
    };

    static const std::unordered_map<char, TokenType> charToOperatorMap = {
        {'+', TokenType::OPERATOR_ADD},
        {'-', TokenType::OPERATOR_SUB},
        {'*', TokenType::OPERATOR_MUL},
        {'/', TokenType::OPERATOR_DIV},

        {'=', TokenType::OPERATOR_SET},
        {'.', TokenType::OPERATOR_DOT},
        {';', TokenType::END_COMMAND},

        {'(', TokenType::LEFT_PAREN},
        {')', TokenType::RIGHT_PAREN},
        {'{', TokenType::LEFT_BRACE},
        {'}', TokenType::RIGHT_BRACE},
        {'[', TokenType::LEFT_BRACKET},
        {']', TokenType::RIGHT_BRACKET},

        {',', TokenType::COMMA}
    };

    // Determine the Literal Type
    for(const std::pair<std::regex, TokenType> c : checks) {
        if(std::regex_match(s, c.first)) {
            return c.second;
        }
    }

    // Determine 1-char Tokens
    if(charToOperatorMap.count(s[0])) return charToOperatorMap.at(s[0]);

    // No regex is matching
    return TokenType::INVALID;
}

}
}