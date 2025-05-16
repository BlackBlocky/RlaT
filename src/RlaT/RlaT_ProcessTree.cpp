#include "RlaT_ProcessTree.h"

#include "RlaT_ProcessElement.h"
#include "RlaT_Script.h"

using namespace std;
using namespace RlaT;

namespace RlaT {
namespace internal {
    
RlaT_ProcessTree::RlaT_ProcessTree(const std::string* tokens, const size_t tokenLength, RlaT_Script* rootScript) {
    rootScript = rootScript;

    _mainElement = createElementFromIndex(tokens, 0);
    if(_mainElement->getType() == ProcessElementType::ERROR) {
        rootScript->outputErrorString("wd");
    }
}

unique_ptr<RlaT_ProcessElement> RlaT_ProcessTree::createElementFromIndex(const std::string* tokens, const int index) {
    // check if it is a keyword
    if(RlaT_ProcessElement::processKeywordToType.count(tokens[index]) == 1) {
        return make_unique<RlaT_ProcessElement>(RlaT_ProcessElement::processKeywordToType.at(tokens[index]));
    }
    // check if its a string
    else if(&tokens[index][0] == "\"") {
        return make_unique<RlaT_ProcessElement>(ProcessElementType::LITERAL);
    }

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

} // namespace internal
} // namespace RlaT
