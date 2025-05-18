#pragma once

#include <string>
#include <memory>
#include <vector>
#include <array>
#include <unordered_map>

#include "RlaT_ProcessElement.h"
#include "RlaT_TypedValue.h"

namespace RlaT {
class RlaTScript;

namespace internal {

/*
    The ProcessTree is based on the AST concept.

    A ProcessTree is the Process for one single line in gerneral.
    If the line is Function call, it will put all the function code into this Process Tree, which means,
    the ProcessTree will be a function.

    If its just a simple line of code, it will just execute it. If it is a Function, it will just add the
    Process the Instance to the Functions of the root script.
*/
class ProcessTree {
public:
    ProcessTree(const std::string* tokens, const size_t tokenLength, RlaTScript* rootScript);

    void printToConsole();

private:
    static const std::string c_functionKeyword;
    static const std::unordered_map<OperatorType, int> c_operatorPriorityMap;

    RlaTScript* rootScript;

    std::shared_ptr<ProcessElement> _mainElement;


    std::unique_ptr<ProcessElement> createElementFromIndex(const std::string* tokens, const int index);
    std::vector<ProcessElement> getAllElements(const std::string* tokens, const size_t tokenLength, const int depth);

    DataType isTokenALiteral(std::string token);
    OperatorType isTokenAOperator(std::string token);
    bool isTokenADatatype(std::string token);
    bool isTokenAFunction(std::string token);
    bool isTokenAVariable(std::string token);

    std::vector<std::pair<std::shared_ptr<ProcessElement>, int>> generateElementDepthMap(const std::string* tokens, const size_t tokenLength);
    std::shared_ptr<ProcessElement> generateBinaryOperationAST(const std::string* tokens, const size_t tokenLength);

    struct OpFragment;
    std::shared_ptr<ProcessElement> r_createComputionTreeFromFragments(std::shared_ptr<OpFragment> fragment);

    void r_printStep(ProcessElement& current, int depth);
};

} // namespace internal
} // namespace Rla
