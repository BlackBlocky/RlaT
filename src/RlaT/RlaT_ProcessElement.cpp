#include "RlaT_ProcessElement.h"

namespace RlaT {
namespace internal {
    
const std::map<std::string, ProcessElementType> RlaT_ProcessElement::processKeywordToType = {
    {"print", ProcessElementType::PRINT}
};

RlaT_ProcessElement::RlaT_ProcessElement(ProcessElementType type) {
    this->_type = type;
    this->childs = std::vector<RlaT_ProcessElement*>();
}

const ProcessElementType RlaT_ProcessElement::getType() {
    return this->_type;
}

void RlaT_ProcessElement::addChild(RlaT_ProcessElement* child) {
    childs.push_back(child);
}

const int RlaT_ProcessElement::getChildsSize() {
    return childs.size();
}

const RlaT_ProcessElement& RlaT_ProcessElement::getChild(int index) {
    return *childs.at(index);
}


} // namespace internal
} // namespace RlaT
