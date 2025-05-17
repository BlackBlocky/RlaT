#pragma once

#include <vector>
#include <map>
#include <string>

namespace RlaT {
namespace internal {

enum class ProcessElementType {
    LITERAL = 0,         // A basic value like 2 or "text" 
    VARIABLE = 1,        // Reference to a variable
    OPERATION = 2,       // Contains a operation like + - = etc
    PRINT = 3,           // Build-In Function => Prints to the Console
    ERROR = 4,           // Error
    FUNCTIONHEADER = 5,  // Function header/defenition
    FUNCTIONCALL = 6     // Function call
};

inline std::string toString(ProcessElementType type) {
    switch (type) {
        case ProcessElementType::LITERAL:        return "LITERAL";
        case ProcessElementType::VARIABLE:       return "VARIABLE";
        case ProcessElementType::OPERATION:      return "OPERATION";
        case ProcessElementType::PRINT:          return "PRINT";
        case ProcessElementType::ERROR:          return "ERROR";
        case ProcessElementType::FUNCTIONHEADER: return "FUNCTIONHEADER";
        case ProcessElementType::FUNCTIONCALL:   return "FUNCTIONCALL";
        default:                                 return "UNKNOWN";
    }
}

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
