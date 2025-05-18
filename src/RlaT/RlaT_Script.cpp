#include "RlaT_Script.h"

#include <iostream>
#include "RlaT_Data.h"
#include <any>
#include "RlaT_ProcessTree.h"

using namespace RlaT::internal;

namespace RlaT {


void RlaT_Script::ping() {
    outputString("Its worrrkssss!");

    RlaT_Data var1 = RlaT_Data(DataType::INTEGER, 15);
    RlaT_Data var2 = RlaT_Data(DataType::INTEGER, 10);

    RlaT_Data result = RlaT_Data::evaluate(var1, var2, OperatorType::ADD);
    if(result.getType() == DataType::ERROR) {
        outputErrorString(result.toString());
    }

    outputString(result.toString());

    std::string test[] = {"(", "(", "1", "+", "2", ")", ")", "+", "3"};
    RlaT_ProcessTree tree = RlaT_ProcessTree(test, 9 , this);

    outputString("Done.");
    
}

void RlaT_Script::setOutputMethod(std::function<void(std::string)> func) {
    this->outputFunction = func;
}
void RlaT_Script::setOutputErrorMethod(std::function<void(std::string)> func) {
    this->outputErrorFunction = func;
}

void RlaT_Script::outputString(std::string output) {
    this->outputFunction(output);
}
void RlaT_Script::outputErrorString(std::string output) {
    this->outputErrorFunction(output);
}


}

