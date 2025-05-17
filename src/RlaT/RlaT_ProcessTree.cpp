#include "RlaT_ProcessTree.h"

#include "RlaT_ProcessElement.h"
#include "RlaT_Script.h"
#include "RlaT_Data.h"
#include <sstream>
#include <regex>
#include <iostream>

using namespace std;
using namespace RlaT;

namespace RlaT {
namespace internal {
    
const std::string RlaT_ProcessTree::c_functionKeyword = "func";
const std::unordered_map<char, int> RlaT_ProcessTree::c_operatorPriorityMap = {
    {'*', 2},
    {'/', 2},
    {'+', 1},
    {'-', 1}
};

RlaT_ProcessTree::RlaT_ProcessTree(const std::string* tokens, const size_t tokenLength, RlaT_Script* rootScript) {
    this->rootScript = rootScript;

    // Create the element of the first token
    _mainElement = createElementFromIndex(tokens, 0);

    // Check if the first element trows an error
    if(_mainElement->getType() == ProcessElementType::ERROR) {
        stringstream ss;
        ss << tokens[0] << " is undefined";
        rootScript->outputErrorString(ss.str());
        return;
    }

    // Process the Line based on its type
    if(_mainElement->getType() == ProcessElementType::LITERAL) {
        generateLiteralAST(tokens, tokenLength);
    }

    // Debug
    stringstream ss;
    ss << (int)_mainElement->getType();
    this->rootScript->outputString(ss.str());
}

unique_ptr<RlaT_ProcessElement> RlaT_ProcessTree::createElementFromIndex(const std::string* tokens, const int index) {
    const string& token = tokens[index];
    
    DataType datatypeCheckResult = isTokenALiteral(token);
    if(datatypeCheckResult != DataType::EMPY) {
        if(datatypeCheckResult == DataType::FUNCTION)
            return make_unique<RlaT_ProcessElement>(ProcessElementType::FUNCTIONHEADER);
        return make_unique<RlaT_ProcessElement>(ProcessElementType::LITERAL);
    }

    if(isTokenADatatype(token)) {

    }

    if(isTokenAFunction(token)) {
        
    }

    if(isTokenAVariable(token)) {

    }

    // The token is undefined
    return make_unique<RlaT_ProcessElement>(ProcessElementType::ERROR);
}

vector<RlaT_ProcessElement> RlaT_ProcessTree::getAllElements(const std::string* tokens, const size_t tokenLength, const int depth) {
    int currentDepth = 0;

    for(size_t i = 0; i < tokenLength; i++) {
        const string& currentToken = tokens[i];

        if(currentToken == "(") {
            currentDepth++;
            continue;
        }
        else if(currentToken == ")") {
            currentDepth--;
            continue;
        }
        
        if (currentDepth == depth) {

        }
    }

    return vector<RlaT_ProcessElement>(); // just get rid of the warning, still TODO!!
}

DataType RlaT_ProcessTree::isTokenALiteral(string token) {
    static const regex int_regex(R"(^[+-]?\d+$)"); // 1, +12, -123, ...
    if(regex_match(token, int_regex)) {
        return DataType::INTEGER;
    }

    if(token == c_functionKeyword) {
        return DataType::FUNCTION;
    }

    // Its not a datatype
    return DataType::EMPY;
}

bool RlaT_ProcessTree::isTokenADatatype(std::string token) {
    return false;
}

bool RlaT_ProcessTree::isTokenAFunction(std::string token) {
    return false;
}

bool RlaT_ProcessTree::isTokenAVariable(std::string token) {
    return false;
}


std::vector<std::pair<std::unique_ptr<RlaT_ProcessElement>, int>> RlaT_ProcessTree::generateElementDepthMap(const std::string* tokens, const size_t tokenLength) {
    // Create a Element for every Elementable token.
    // Store every element in a map, while the key is the element and the value is the depth
    vector<pair<unique_ptr<RlaT_ProcessElement>, int>> tokenElementDepthMap;
    int currentDepth = 0;

    for(size_t i = 0; i < tokenLength; i++) {
        switch(tokens[i][0]) {
            case '(': case '{': {
                currentDepth++;   
                break;
            }
            case ')': case '}': {
                currentDepth--;
                break;
            }
            default: {
                tokenElementDepthMap.push_back(make_pair(
                    createElementFromIndex(tokens, i),
                    currentDepth
                ));
                break;
            }
        }

    }

    return tokenElementDepthMap;
}

void RlaT_ProcessTree::generateLiteralAST(const std::string* tokens, const size_t tokenLength) {
    std::vector<std::pair<std::unique_ptr<RlaT_ProcessElement>, int>> elementDepthMap = generateElementDepthMap(tokens, tokenLength);

    for(size_t i = 0; i < elementDepthMap.size(); i++) {
        pair<std::unique_ptr<RlaT_ProcessElement>, int>& item = elementDepthMap[i];
        stringstream ss;
        ss << to_string(item.second) << " - " << toString(item.first->getType());
        std::cout << ss.str() << std::endl;
    }

    // Find the Primary 


    // Create the calcuation expression
    int currentDepth = 0;
    bool bracketFound = false;
    
    do {

    } while(bracketFound);
}

} // namespace internal
} // namespace RlaT
