#pragma once

#include <any>
#include <string>
#include <array>
#include <functional>

namespace RlaT {
namespace internal {
    
enum class DataType {
    EMPY = 0,       // value = bool (unused)
    ERROR = 1,      // value = string
    INTEGER = 2,    // value = int32
    FUNCTION = 3,   // value = bool (unused) (Just for Interpreting purposes, not actually a Datatype)
    OPFRAGMENT = 4  // value = int32 (unsued) (Just for generating AST models in ProcessTree)
};
enum class OperatorType {
    NONE = 0,     // used for generting AST, no other purpose
    SET = 1,
    ADD = 2,
    SUB = 3,
    MUL = 4,
    DIV = 5
};

inline std::string toString(OperatorType op) {
    switch (op) {
        case OperatorType::NONE: return "NONE";
        case OperatorType::SET:  return "SET";
        case OperatorType::ADD:  return "ADD";
        case OperatorType::SUB:  return "SUB";
        case OperatorType::MUL:  return "MUL";
        case OperatorType::DIV:  return "DIV";
        default:                 return "UNKNOWN";
    }
}

class RlaT_Data {
public:
    RlaT_Data(DataType type, std::any defaultValue);
    DataType getType();
    std::any getAnyValue();
    std::string toString();

    static RlaT_Data evaluate(RlaT_Data a, RlaT_Data b, OperatorType operation);


private:
    static const std::array<std::string, 3> datatypeNames;

    static const std::array<std::string, 5> operationNames;
    static const std::array<std::function<RlaT_Data(RlaT_Data, RlaT_Data)>, 5> operationMethods;


    DataType type;
    std::any value;
    

    static RlaT_Data evSet(RlaT_Data a, RlaT_Data b);
    static RlaT_Data evAdd(RlaT_Data a, RlaT_Data b);
    static RlaT_Data evSub(RlaT_Data a, RlaT_Data b);
    static RlaT_Data evMul(RlaT_Data a, RlaT_Data b);
    static RlaT_Data evDiv(RlaT_Data a, RlaT_Data b);

    static RlaT_Data evCreateError(DataType datatype, OperatorType op);
};

} // namespace internal
} // namespace Rlat
