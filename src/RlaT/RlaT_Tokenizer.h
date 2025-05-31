#pragma once

#include "RlaT_Token.h"
#include <vector>
#include <string>
#include <unordered_set>

namespace RlaT {
namespace internal {

class Tokenizer {
public:
    static std::vector<Token> createTokensFromString(const std::string& line, int lineNumber);
private:
    static std::string removeFormattingExceptInStrings(const std::string& s);
    static Token createToken(std::string content, int charNumber, int lineNumber);
    static TokenType determineTokenType(std::string& s);

    inline static const std::unordered_set<char> SPLITTERS = {'(', ')', '{', '}', '[', ']', ';', '=', '+', '-', '*', '/', '.', ','};
};

}
}