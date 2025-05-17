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
const std::unordered_map<OperatorType, int> RlaT_ProcessTree::c_operatorPriorityMap = {
    {OperatorType::SET, 3},
    {OperatorType::MUL, 2},
    {OperatorType::DIV, 2},
    {OperatorType::ADD, 1},
    {OperatorType::SUB, 1}
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
            return make_unique<RlaT_ProcessElement>(ProcessElementType::FUNCTIONHEADER, false);
        
        switch(datatypeCheckResult) {
            case DataType::INTEGER: {
                return make_unique<RlaT_ProcessElement>(
                    ProcessElementType::LITERAL,                     // type
                    RlaT_Data(DataType::INTEGER, std::stoi(token))   // value
                );
            }
        }
    }

    OperatorType operatorTypeCheckResult = isTokenAOperator(token);
    if(operatorTypeCheckResult != OperatorType::NONE) {
        return make_unique<RlaT_ProcessElement>(ProcessElementType::OPERATION, operatorTypeCheckResult);
    }

    if(isTokenADatatype(token)) {

    }

    if(isTokenAFunction(token)) {
        
    }

    if(isTokenAVariable(token)) {

    }

    // The token is undefined
    return make_unique<RlaT_ProcessElement>(ProcessElementType::ERROR, "Token \"" + token + "\" is undefined");
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

OperatorType RlaT_ProcessTree::isTokenAOperator(std::string token) {
    switch(token[0]) {
        case '+': return OperatorType::ADD;
        case '-': return OperatorType::SUB;
        case '*': return OperatorType::MUL;
        case '/': return OperatorType::DIV;
        case '=': return OperatorType::SET;
    }

    return OperatorType::NONE;
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

    // Debug
    for(size_t i = 0; i < elementDepthMap.size(); i++) {
        pair<std::unique_ptr<RlaT_ProcessElement>, int>& item = elementDepthMap[i];
        stringstream ss;
        if(item.first->getType() == ProcessElementType::LITERAL)
            ss << to_string(item.second) << " - " << toString(item.first->getType()) << " => " << any_cast<RlaT_Data>(item.first->getValue()).toString();
        else if(item.first->getType() == ProcessElementType::OPERATION)
            ss << to_string(item.second) << " - " << toString(item.first->getType()) << " => " << toString(any_cast<OperatorType>(item.first->getValue()));
        std::cout << ss.str() << std::endl;
    }

    // Set constants and find the highest depth
    static const int highestPrio = 2;
    int highestDepth = 0;
    for(size_t i = 0; i < elementDepthMap.size(); i++) {
        if(elementDepthMap[i].second > highestPrio) highestDepth = elementDepthMap[i].second;
    }

    // Create the calcuation expression
    // And Replace the primaryElement with lowest Prio
    int currentPrio = highestPrio;
    int currentDepth = 0;
    while(currentDepth <= highestDepth) {
        // Go though every element from left to right. First fo the the highest prio, then one lower etc.
        // So same Prios on the same depth have a higher prio if they are more left
        currentPrio = highestPrio;
        while(currentPrio > -1) {
            for(size_t i = 0; i < elementDepthMap.size(); i++) {
                pair<std::unique_ptr<RlaT_ProcessElement>, int>& item = elementDepthMap[i];
                if(item.second != currentDepth) continue;


            }

            currentPrio--;
        }

        currentDepth++;
    }

    // Create the calcuation expression
    
    bool bracketFound = false;
    
    do {

    } while(bracketFound);
}

} // namespace internal
} // namespace RlaT
