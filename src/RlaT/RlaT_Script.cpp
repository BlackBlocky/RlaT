#include "RlaT_Script.h"

#include <iostream>
#include "RlaT_TypedValue.h"
#include <any>
#include "RlaT_ProcessTree.h"

using namespace RlaT::internal;

namespace RlaT {


void RlaTScript::ping() {
    outputString("Stating ping!");

    /*
    TypedValue var1 = TypedValue(DataType::INTEGER, 15);
    TypedValue var2 = TypedValue(DataType::INTEGER, 10);

    TypedValue result = TypedValue::evaluate(var1, var2, OperatorType::ADD);
    if(result.getType() == DataType::ERROR) {
        outputErrorString(result.toString());
    }

    outputString(result.toString());*/

    std::string test[] = {"(", "(", "1", "+", "2", ")", ")", "+", "3"};
    ProcessTree tree = ProcessTree(test, 9 , this);

    outputString("Done.");
    
}

void RlaTScript::setOutputMethod(std::function<void(std::string)> func) {
    this->outputFunction = func;
}
void RlaTScript::setOutputErrorMethod(std::function<void(std::string)> func) {
    this->outputErrorFunction = func;
}

void RlaTScript::outputString(std::string output) {
    this->outputFunction(output);
}
void RlaTScript::outputErrorString(std::string output) {
    this->outputErrorFunction(output);
}


}

