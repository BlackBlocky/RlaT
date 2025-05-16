#pragma once

#include <vector>
#include <map>
#include <string>

namespace RlaT {
namespace internal {

enum class ProcessElementType {
    LITERAL = 0,   // A basic value like 2 or "text" 
    VARIABLE = 1,  // Reference to a variable
    OPERATION = 2, // Contains a operation like + - = etc
    PRINT = 3,     // Build-In Function => Prints to the Console
    ERROR = 4      // Error
};

class RlaT_ProcessElement {
public:
    static const std::map<std::string, ProcessElementType> processKeywordToType;

    RlaT_ProcessElement(ProcessElementType type);
    const ProcessElementType getType();
    void addChild(RlaT_ProcessElement*);
    const int getChildsSize();
    const RlaT_ProcessElement& getChild(int index);

private:
    ProcessElementType _type;
    std::vector<RlaT_ProcessElement*> childs;

};

} // namespace internal
} // namespace RlaT
