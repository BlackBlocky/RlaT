#include "RlaT_ProcessElement.h"

#include <memory>

namespace RlaT {
namespace internal {

RlaT_ProcessElement::RlaT_ProcessElement(ProcessElementType type, std::any value) {
    this->_type = type;
    this->_value = value;
    this->childs = std::vector<std::shared_ptr<RlaT_ProcessElement>>();
}

ProcessElementType RlaT_ProcessElement::getType() {
    return this->_type;
}

void RlaT_ProcessElement::addChild(std::shared_ptr<RlaT_ProcessElement> child) {
    childs.push_back(std::move(child));
}

int RlaT_ProcessElement::getChildsSize() {
    return childs.size();
}

RlaT_ProcessElement& RlaT_ProcessElement::getChild(int index) {
    return *childs.at(index);
}

std::any& RlaT_ProcessElement::getValue() {
    return _value;
} 


} // namespace internal
} // namespace RlaT
