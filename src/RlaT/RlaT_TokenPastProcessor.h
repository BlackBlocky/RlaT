#pragma once

#include <vector>
#include "RlaT_Token.h"
#include <unordered_map>
#include <array>

namespace RlaT {
namespace internal {

class TokenPastProcessor {
public:
    static void processTokenVector(std::vector<Token>& tokenList);
private:
    // This Map contains all possible merge combinations
    struct MergeRule;
    static const std::array<MergeRule, 2> mergeRules;

    static const MergeRule* findMatchingRule(TokenType& a, TokenType& b);
};

}
} // namespace RlaT
