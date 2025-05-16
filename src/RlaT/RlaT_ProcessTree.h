#pragma once

#include <string>
#include <memory>
#include <vector>
#include <array>

#include "RlaT_ProcessElement.h"

namespace RlaT {
class RlaT_Script;

namespace internal {

class RlaT_ProcessTree {
public:
    RlaT_ProcessTree(const std::string* tokens, const size_t tokenLength, RlaT_Script* rootScript);

private:
    RlaT_Script* rootScript;

    std::unique_ptr<RlaT_ProcessElement> _mainElement;

    std::unique_ptr<RlaT_ProcessElement> createElementFromIndex(const std::string* tokens, const int index);
    std::vector<RlaT_ProcessElement> getAllElements(const std::string* tokens, const size_t tokenLength, const int depth);
};

} // namespace internal
} // namespace Rla
