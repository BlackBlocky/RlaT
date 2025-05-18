#pragma once

#include <vector>
#include <map>
#include <string>
#include <any>
#include <memory>

namespace RlaT {
namespace internal {

enum class ProcessElementType {
    LITERAL = 0,         // A basic value like 2 or "text"           // Contains RlaT_Data.h
    VARIABLE = 1,        // Reference to a variable                  // Contains TODO
    BINARYOPERATION = 2, // Contains a operation like + - = etc      // Contains OperationType
    ERROR = 3,           // Error                                    // Contains std::string
    FUNCTIONHEADER = 4,  // Function header/defenition               // Contains TODO
    FUNCTIONCALL = 5     // Function call                            // Contains TODO
};

inline std::string toString(ProcessElementType type) {
    switch (type) {
        case ProcessElementType::LITERAL:         return "LITERAL";
        case ProcessElementType::VARIABLE:        return "VARIABLE";
        case ProcessElementType::BINARYOPERATION: return "BINARYOPERATION";
        case ProcessElementType::ERROR:           return "ERROR";
        case ProcessElementType::FUNCTIONHEADER:  return "FUNCTIONHEADER";
        case ProcessElementType::FUNCTIONCALL:    return "FUNCTIONCALL";
        default:                                  return "UNKNOWN";
    }
}

class RlaT_ProcessElement {
public:
    RlaT_ProcessElement(ProcessElementType type, std::any value);
    ProcessElementType getType();
    void addChild(std::shared_ptr<RlaT_ProcessElement>);
    int getChildsSize();
    RlaT_ProcessElement& getChild(int index);

    std::any& getValue();

private:
    ProcessElementType _type;
    std::vector<std::shared_ptr<RlaT_ProcessElement>> childs;

    std::any _value;
};

} // namespace internal
} // namespace RlaT
