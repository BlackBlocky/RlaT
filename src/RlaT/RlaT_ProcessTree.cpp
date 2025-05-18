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

    
    // Process the Line based on its type
    if(_mainElement->getType() == ProcessElementType::LITERAL || tokens[0][0] == '(') {
        generateLiteralAST(tokens, tokenLength);
    }

    // Check if the first element trows an error
    if(_mainElement->getType() == ProcessElementType::ERROR) {
        stringstream ss;
        ss << tokens[0] << " is undefined";
        rootScript->outputErrorString(ss.str());
        return;
    }

    // Debug
    stringstream ss;
    ss << (int)_mainElement->getType();
    this->rootScript->outputString(ss.str());

    printToConsole();
}

void RlaT_ProcessTree::printToConsole() {
    r_printStep(*_mainElement, 0);
}

void RlaT_ProcessTree::r_printStep(RlaT_ProcessElement& current, int depth) {
    // Create line for current
    stringstream ss;
    for(int i = 0; i < depth; i++) ss << "..";
    ss << toString(current.getType()) << " -> ";
    switch(current.getType()) {
        case ProcessElementType::LITERAL: {
            ss << any_cast<RlaT_Data>(current.getValue()).toString();
            break;
        }
        case ProcessElementType::OPERATION: {
            ss << toString(any_cast<OperatorType>(current.getValue()));
            break;
        }
    }
    cout << ss.str() << std::endl;

    // Start lines for childs
    for(size_t i = 0; i < current.getChildsSize(); i++) r_printStep(current.getChild(i), depth + 1);
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


std::vector<std::pair<std::shared_ptr<RlaT_ProcessElement>, int>> RlaT_ProcessTree::generateElementDepthMap(const std::string* tokens, const size_t tokenLength) {
    // Create a Element for every Elementable token.
    // Store every element in a map, while the key is the element and the value is the depth
    vector<pair<shared_ptr<RlaT_ProcessElement>, int>> tokenElementDepthMap;
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
                    std::move(createElementFromIndex(tokens, i)),
                    currentDepth
                ));
                break;
            }
        }

    }

    return tokenElementDepthMap;
}

struct RlaT_ProcessTree::OpFragment {
    shared_ptr<RlaT_ProcessElement> leftLiteral;
    shared_ptr<RlaT_ProcessElement> rightLiteral;
    shared_ptr<OpFragment> leftFragment;
    shared_ptr<OpFragment> rightFramgent;

    bool leftIsLiteral;
    bool rightIsLiteral;

    shared_ptr<RlaT_ProcessElement> operatorElement;

    OpFragment(
        shared_ptr<RlaT_ProcessElement> lL, shared_ptr<OpFragment> lF, bool lIL,
        shared_ptr<RlaT_ProcessElement> rL, shared_ptr<OpFragment> rF, bool rIL,
        shared_ptr<RlaT_ProcessElement> operatorElement, vector<pair<shared_ptr<OpFragment>, shared_ptr<OpFragment>>>& usesLog)
            : leftLiteral(move(lL)), leftFragment(lF), leftIsLiteral(lIL),
              rightLiteral(move(rL)), rightFramgent(rF), rightIsLiteral(rIL),
              operatorElement(move(operatorElement)) { 
        
        // Go through the logs Array backwards. Check if the framgent is already used by any higher framgent, if it is the case,
        // set the framgent to the higher framgnet.
        // Repeat until no more used are found

        while (searchHigherFragmentAndReplace(leftFragment, usesLog)) ;
        while (searchHigherFragmentAndReplace(rightFramgent, usesLog)) ;
    }

    // Returns true until nothing higher is found
    // NOTE: first is used, second is user
    bool searchHigherFragmentAndReplace(shared_ptr<OpFragment>& original, vector<pair<shared_ptr<OpFragment>, shared_ptr<OpFragment>>>& usesLog) {
        if(usesLog.size() == 0) return false;
        
        for(size_t i = usesLog.size(); i-- > 0;) {
            cout << usesLog.size() << " size\n";
            cout << i << "INDEX \n";
            if(usesLog[i].first.get() == original.get()) {
                original = usesLog[i].second;
                return true;
            }
        }

        return false;
    }
};

// Generate the AST using the Shunting Yard Algorithm
void RlaT_ProcessTree::generateLiteralAST(const std::string* tokens, const size_t tokenLength) {
    std::vector<std::pair<std::shared_ptr<RlaT_ProcessElement>, int>> elementDepthMap = generateElementDepthMap(tokens, tokenLength);

    // Debug
    for(size_t i = 0; i < elementDepthMap.size(); i++) {
        pair<std::shared_ptr<RlaT_ProcessElement>, int>& item = elementDepthMap[i];
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
        if(elementDepthMap[i].second > highestDepth) highestDepth = elementDepthMap[i].second;
    }
    int operatorsCount = 0;
    for(size_t i = 0; i < elementDepthMap.size(); i++) {
        if(elementDepthMap[i].first->getType() == ProcessElementType::OPERATION) operatorsCount++;
    }

    // Create calculation blocks
    // Example: 1+(5*2+10*3)   =>  0= [5*2], 1= [10*3], 2= [(0)+(1)], ...
    int currentPrio = highestPrio;
    int currentDepth = highestDepth;

    // The index of the Elements is matching of the operatorNr. Example -> Operation nr. 1 is at index 0
    shared_ptr<OpFragment> fragmentArray[operatorsCount];
    // Here its sorted by the creating order -> Deepest is first, last Block is last.
    // NOTE: Its going to be inverted later!
    vector<shared_ptr<OpFragment>> sortedFragementArray;

    // Stores if a higher fragemnt uses a deeper fragemnt. For Example, of the +fragment(id=1)  at 1 * 2 + 2
    // uses the *fragment(id=0), it will be logged like this: 0,1 (left used, right user)
    vector<pair<shared_ptr<OpFragment>, shared_ptr<OpFragment>>> fragmentUseLog;

    // Looping from highest depth to lowest
    while(currentDepth >= 0) {

        // Go though every element from left to right. First fo the the highest prio, then one lower etc.
        // So same Prios on the same depth have a higher prio if they are more left
        currentPrio = highestPrio;
        while(currentPrio > -1) {
            int currentOperatorNumber = -1; // The first Operator will be 0
            for(size_t i = 0; i < elementDepthMap.size(); i++) {
                // Check if in correct Depth
                pair<std::shared_ptr<RlaT_ProcessElement>, int>& item = elementDepthMap[i];
                if(item.first->getType() == ProcessElementType::OPERATION) currentOperatorNumber++;
                if(item.second != currentDepth) continue;

                // Note: Its not possible that a operator is at index 0
                // Creating a block if the operator matches the current Prio
                // If its a number its going to be skipped
                if(item.first->getType() != ProcessElementType::OPERATION) continue;
                int elementOperatorPrio = c_operatorPriorityMap.at(any_cast<OperatorType>(item.first->getValue()));
                if(elementOperatorPrio != currentPrio) continue;

                // Create the Fragment/Block -----------------------
                bool isLeftAlreadyFragmented  = currentOperatorNumber != 0                   && fragmentArray[currentOperatorNumber - 1] != nullptr;
                bool isRightAlreadyFragmented = currentOperatorNumber != operatorsCount - 1 && fragmentArray[currentOperatorNumber + 1] != nullptr;

                // Note: The Constructor will search for higher fragments using the log. So if a fragment already used the given fragent, it will be replaced with the
                // fragment that used the fragemnt. used framgent = user framgent
                fragmentArray[currentOperatorNumber] = make_shared<OpFragment>(
                    (isLeftAlreadyFragmented) ? nullptr : elementDepthMap[i - 1].first, (isLeftAlreadyFragmented) ? fragmentArray[currentOperatorNumber - 1] : nullptr, !isLeftAlreadyFragmented,
                    (isRightAlreadyFragmented) ? nullptr : elementDepthMap[i + 1].first, (isRightAlreadyFragmented) ? fragmentArray[currentOperatorNumber + 1] : nullptr, !isRightAlreadyFragmented,
                    item.first,
                    fragmentUseLog
                );
                sortedFragementArray.push_back(fragmentArray[currentOperatorNumber]);

                // Log fragment-uses if needed
                if(isLeftAlreadyFragmented) fragmentUseLog.push_back(make_pair(fragmentArray[currentOperatorNumber]->leftFragment, fragmentArray[currentOperatorNumber]));
                if(isRightAlreadyFragmented) fragmentUseLog.push_back(make_pair(fragmentArray[currentOperatorNumber]->rightFramgent, fragmentArray[currentOperatorNumber]));

                cout << "Op: " << currentOperatorNumber << "\n";
            }
            currentPrio--;
        }
        currentDepth--;
    }

    // Inverting/Reversing the SortedFramgentArray
    std::reverse(sortedFragementArray.begin(), sortedFragementArray.end());

    for(size_t i = 0; i < operatorsCount; i++) {
        cout << "Framgent " << i << ": " << sortedFragementArray[i]->leftIsLiteral << " " << toString(any_cast<OperatorType>(sortedFragementArray[i]->operatorElement->getValue())) << " " << sortedFragementArray[i]->rightIsLiteral << "\n";
    }

    // Convert the first framgent
    _mainElement = move(r_createComputionTreeFromFragments(sortedFragementArray[0]));
}
shared_ptr<RlaT_ProcessElement> RlaT_ProcessTree::r_createComputionTreeFromFragments(shared_ptr<OpFragment> fragment) {
    shared_ptr<RlaT_ProcessElement> current = move(fragment->operatorElement);
    current->addChild((fragment->leftIsLiteral) ? fragment->leftLiteral : r_createComputionTreeFromFragments(fragment->leftFragment));
    current->addChild((fragment->rightIsLiteral) ? fragment->rightLiteral : r_createComputionTreeFromFragments(fragment->rightFramgent));

    return current;
}

} // namespace internal
} // namespace RlaT
