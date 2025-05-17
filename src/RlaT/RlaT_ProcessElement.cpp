#include "RlaT_ProcessElement.h"

namespace RlaT {
namespace internal {

RlaT_ProcessElement::RlaT_ProcessElement(ProcessElementType type, std::any value) {
    this->_type = type;
    this->_value = value;
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

const std::any& RlaT_ProcessElement::getValue() {
    return _value;
} 


} // namespace internal
} // namespace RlaT
