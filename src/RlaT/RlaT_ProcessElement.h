#pragma once

#include <vector>
#include <map>
#include <string>

namespace RlaT {
namespace internal {

enum class ProcessElementType {
    LITERAL = 0,
    VARIABLE = 1,
    OPERATION = 2,
    PRINT = 3,
    ERROR = 4
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
