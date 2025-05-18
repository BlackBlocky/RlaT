#include "RlaT_ProcessElement.h"

#include <memory>

namespace RlaT {
namespace internal {

ProcessElement::ProcessElement(ProcessElementType type, std::any value) {
    this->_type = type;
    this->_value = value;
    this->childs = std::vector<std::shared_ptr<ProcessElement>>();
}

ProcessElementType ProcessElement::getType() {
    return this->_type;
}

void ProcessElement::addChild(std::shared_ptr<ProcessElement> child) {
    childs.push_back(std::move(child));
}

int ProcessElement::getChildsSize() {
    return childs.size();
}

ProcessElement& ProcessElement::getChild(int index) {
    return *childs.at(index);
}

std::any& ProcessElement::getValue() {
    return _value;
} 


} // namespace internal
} // namespace RlaT
