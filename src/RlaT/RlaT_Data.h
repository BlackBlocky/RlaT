#pragma once

#include <any>
#include <string>
#include <array>
#include <functional>

namespace RlaT {
namespace internal {
    
class RlaT_Data {
public:
    static const int EMPY = 0;      // boolean
    static const int ERROR = 1;     // string
    static const int INTERGER = 2;  // int
    
    static const int OP_SET = 0;
    static const int OP_ADD = 1;
    static const int OP_SUB = 2;
    static const int OP_MUL = 3;
    static const int OP_DIV = 4;


    RlaT_Data(int type, std::any defaultValue);
    int getType();
    std::any getAnyValue();
    std::string toString();

    static RlaT_Data evaluate(RlaT_Data a, RlaT_Data b, int operation);


private:
    static const std::array<std::string, 3> datatypeNames;

    static const std::array<std::string, 5> operationNames;
    static const std::array<std::function<RlaT_Data(RlaT_Data, RlaT_Data)>, 5> operationMethods;


    int type;
    std::any value;
    

    static RlaT_Data evSet(RlaT_Data a, RlaT_Data b);
    static RlaT_Data evAdd(RlaT_Data a, RlaT_Data b);
    static RlaT_Data evSub(RlaT_Data a, RlaT_Data b);
    static RlaT_Data evMul(RlaT_Data a, RlaT_Data b);
    static RlaT_Data evDiv(RlaT_Data a, RlaT_Data b);

    static RlaT_Data evCreateError(int datatype, int op);
};

} // namespace internal
} // namespace Rlat
