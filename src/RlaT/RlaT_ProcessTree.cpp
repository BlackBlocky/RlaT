#include "RlaT_ProcessTree.h"

#include "RlaT_ProcessElement.h"
#include "RlaT_Script.h"
#include "RlaT_Data.h"
#include <sstream>
#include <regex>

using namespace std;
using namespace RlaT;

namespace RlaT {
namespace internal {
    
RlaT_ProcessTree::RlaT_ProcessTree(const std::string* tokens, const size_t tokenLength, RlaT_Script* rootScript) {
    this->rootScript = rootScript;

    _mainElement = createElementFromIndex(tokens, 0);
    if(_mainElement->getType() == ProcessElementType::ERROR) {
        stringstream ss;
        ss << tokens[0] << " is undefined";
        rootScript->outputErrorString(ss.str());
        return;
    }

    stringstream ss;
    ss << (int)_mainElement->getType();
    this->rootScript->outputString(ss.str());
}

unique_ptr<RlaT_ProcessElement> RlaT_ProcessTree::createElementFromIndex(const std::string* tokens, const int index) {
    const string& token = tokens[index];
    
    if(isTokenALiteral(token) != DataType::EMPY) {
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
}

DataType RlaT_ProcessTree::isTokenALiteral(string token) {
    static const regex int_regex(R"(^[+-]?\d+$)"); // 1, +12, -123, ...
    if(regex_match(token, int_regex)) {
        return DataType::INTEGER;
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


} // namespace internal
} // namespace RlaT
