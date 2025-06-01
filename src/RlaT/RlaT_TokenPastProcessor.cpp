#include "RlaT_TokenPastProcessor.h"

#include <iostream>

namespace RlaT {
namespace internal {

struct TokenPastProcessor::MergeRule {
    TokenType lToken;
    TokenType rToken;
    TokenType result;
};

const std::array<TokenPastProcessor::MergeRule, 2> TokenPastProcessor::mergeRules {{
    {TokenType::OPERATOR_ADD, TokenType::OPERATOR_ADD, TokenType::INCREMENT},
    {TokenType::OPERATOR_SUB, TokenType::OPERATOR_SUB, TokenType::DECREMENT}
}};

// This Function merges Tokens like '+' '+' together, or '-' '23', that the lexer missed.
// It will also detect basic syntax errors
void TokenPastProcessor::processTokenVector(std::vector<Token>& tokenList) {
    if(tokenList.size() <= 1) return;

    for(size_t i = 1; i < tokenList.size(); i++) {
        // Get a Matching rule for the current and last Token
        const TokenPastProcessor::MergeRule* matchingRule = findMatchingRule(tokenList[i-1].getType(), tokenList[i].getType());
        if(matchingRule == nullptr) continue;

        // A rule was found!
        // Create the new Token that is the result of the rules
        Token newToken(tokenList[i-1].getContent() + tokenList[i].getContent(), matchingRule->result, tokenList[i-1].getLineNumber(), tokenList[i-1].getCharNumber());

        // Remove the current two tokens from the tokenList
        tokenList.erase(tokenList.begin() + i-1);
        tokenList.erase(tokenList.begin() + i-1);
        tokenList.insert(tokenList.begin() + i-1, newToken);

        i--;
    }
}

// Returns a nullptr if no matching rule was found
const TokenPastProcessor::MergeRule* TokenPastProcessor::findMatchingRule(TokenType& a, TokenType& b) {
    for(size_t i = 0; i < mergeRules.size(); i++) {
        const TokenPastProcessor::MergeRule& currentRule = mergeRules[i];

        if(currentRule.lToken == a && currentRule.rToken == b) return &currentRule;
    }

    return nullptr;
}

}
}