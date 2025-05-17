#pragma once

#include <string>
#include <memory>
#include <vector>
#include <array>
#include <unordered_map>

#include "RlaT_ProcessElement.h"
#include "RlaT_Data.h"

namespace RlaT {
class RlaT_Script;

namespace internal {

/*
    The ProcessTree is based on the AST concept.

    A ProcessTree is the Process for one single line in gerneral.
    If the line is Function call, it will put all the function code into this Process Tree, which means,
    the ProcessTree will be a function.

    If its just a simple line of code, it will just execute it. If it is a Function, it will just add the
    Process the Instance to the Functions of the root script.
*/
class RlaT_ProcessTree {
public:
    RlaT_ProcessTree(const std::string* tokens, const size_t tokenLength, RlaT_Script* rootScript);

private:
    static const std::string c_functionKeyword;
    static const std::unordered_map<OperatorType, int> c_operatorPriorityMap;

    RlaT_Script* rootScript;

    std::unique_ptr<RlaT_ProcessElement> _mainElement;


    static std::unique_ptr<RlaT_ProcessElement> createElementFromIndex(const std::string* tokens, const int index);
    std::vector<RlaT_ProcessElement> getAllElements(const std::string* tokens, const size_t tokenLength, const int depth);

    static DataType isTokenALiteral(std::string token);
    static OperatorType isTokenAOperator(std::string token);
    static bool isTokenADatatype(std::string token);
    static bool isTokenAFunction(std::string token);
    static bool isTokenAVariable(std::string token);

    static std::vector<std::pair<std::unique_ptr<RlaT_ProcessElement>, int>> generateElementDepthMap(const std::string* tokens, const size_t tokenLength);
    static void generateLiteralAST(const std::string* tokens, const size_t tokenLength);
};

} // namespace internal
} // namespace Rla
