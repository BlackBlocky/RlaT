#pragma once

#include <string>

namespace RlaT {
namespace internal {

enum class TokenType {
    INVALID,
    IDENTIFIER,

    OPERATOR_ADD,  // +
    OPERATOR_SUB,  // -
    OPERATOR_MUL,  // *
    OPERATOR_DIV,  // /
    OPERATOR_SET,  // =

    END_COMMAND,   // ;

    LEFT_PAREN,    // (
    RIGHT_PAREN,   // )
    LEFT_BRACE,    // {
    RIGHT_BRACE,   // }
    LEFT_BRACKET,  // [
    RIGHT_BRACKET, // ]
    
    STRING,
    INTEGER
};

class Token {
public:
    Token(std::string content, TokenType type, int lineNr, int charNr);

    int& getLineNumber();
    int& getCharNumber();
    std::string& getContent(); 
    TokenType& getType();

private:
    int lineNumber; // The Line the Token is loacted
    int charNumber; // The character the Token starts
    std::string content;
    TokenType type;
};

}
}