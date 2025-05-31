#include "RlaT_Token.h"

namespace RlaT {
namespace internal {

Token::Token(std::string content, TokenType type, int lineNr, int charNr) {
    this->content = content;
    this->type = type;
    this->lineNumber = lineNr;
    this->charNumber = charNr;
}

int& Token::getLineNumber() {
    return lineNumber;
}

int& Token::getCharNumber() {
    return charNumber;
}

std::string& Token::getContent() {
    return content;
}

TokenType& Token::getType() {
    return type;
}

}
}