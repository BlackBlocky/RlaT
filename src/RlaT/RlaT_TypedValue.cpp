#include "RlaT_TypedValue.h"

#include <string>
#include "RlaT_Script.h"
#include <sstream>

using namespace std;

namespace RlaT {
namespace internal {
    
    // Inits
    const std::array<std::string, 3> TypedValue::datatypeNames = {
        "empy",  // 0
        "error", // 1
        "int"    // 2
    };
    const std::array<std::string, 5> TypedValue::operationNames = {
        "set", // 0
        "add", // 1
        "sub", // 2
        "mul", // 3
        "div"  // 4
    };
    const std::array<std::function<TypedValue(TypedValue, TypedValue)>, 5> TypedValue::operationMethods = {
        evSet,
        evAdd,
        evSub,
        evMul,
        evDiv
    };


    // Methods
    TypedValue::TypedValue(DataType type, any defaultValue) {
        this-> type = type;
        this-> value = defaultValue;
    }

    DataType TypedValue::getType() {
        return this->type;
    }  
    any TypedValue::getAnyValue() {
        return this->value;
    }

    string TypedValue::toString() {
        
        switch(type) {
            case DataType::EMPY: {
                return "Empy";
            }
            case DataType::ERROR: {
                return any_cast<string>(this->value);
            }
            case DataType::INTEGER: {
                return to_string(any_cast<int>(this->value));
            }
        }

        // Default return value
        return "";
    }


    TypedValue TypedValue::evaluate(TypedValue a, TypedValue b, OperatorType operation) {
        // Check if the Datatypes are the same
        if(a.getType() != b.getType()) {
            stringstream ss;
            ss << "Unmatching Datatypes \"" << datatypeNames.at((size_t)a.getType()) << "\" and \"" << datatypeNames.at((size_t)b.getType()) << "\"";
            return TypedValue(DataType::ERROR, ss.str());
        }

        return operationMethods.at((size_t)operation)(a, b);
    }

    TypedValue TypedValue::evSet(TypedValue a, TypedValue b) {
        // At this point, we know that both have the same Data-Type
        return TypedValue(b.getType(), b.getAnyValue());
    }

    TypedValue TypedValue::evAdd(TypedValue a, TypedValue b) {
        // At this point, we know that both have the same Data-Type
        switch(a.getType()) {
            case DataType::INTEGER: {
                int aValue = any_cast<int>(a.getAnyValue());
                int bValue = any_cast<int>(b.getAnyValue());

                TypedValue result = TypedValue(a.getType(), aValue + bValue);
                return result;
            }
        }

        // If the Datatype is not supported, return an error
        return evCreateError(a.getType(), OperatorType::ADD);
    }

    TypedValue TypedValue::evSub(TypedValue a, TypedValue b) {
        // At this point, we know that both have the same Data-Type
        switch(a.getType()) {
            case DataType::INTEGER: {
                int aValue = any_cast<int>(a.getAnyValue());
                int bValue = any_cast<int>(b.getAnyValue());

                TypedValue result = TypedValue(a.getType(), aValue - bValue);
                return result;
            }
        }

        // If the Datatype is not supported, return an error
        return evCreateError(a.getType(), OperatorType::SUB);
    }

    TypedValue TypedValue::evMul(TypedValue a, TypedValue b) {
        // At this point, we know that both have the same Data-Type
        switch(a.getType()) {
            case DataType::INTEGER: {
                int aValue = any_cast<int>(a.getAnyValue());
                int bValue = any_cast<int>(b.getAnyValue());

                TypedValue result = TypedValue(a.getType(), aValue * bValue);
                return result;
            }
        }

        // If the Datatype is not supported, return an error
        return evCreateError(a.getType(), OperatorType::MUL);
    }

    TypedValue TypedValue::evDiv(TypedValue a, TypedValue b) {
        // At this point, we know that both have the same Data-Type
        switch(a.getType()) {
            case DataType::INTEGER: {
                int aValue = any_cast<int>(a.getAnyValue());
                int bValue = any_cast<int>(b.getAnyValue());

                TypedValue result = TypedValue(a.getType(), aValue / bValue);
                return result;
            }
        }

        // If the Datatype is not supported, return an error
        return evCreateError(a.getType(), OperatorType::DIV);
    }

    TypedValue TypedValue::evCreateError(DataType datatype, OperatorType op) {
        stringstream ss;
        ss << "Can not do the operation \"" << operationNames.at((size_t)op) << "\" on the Data-Type \"" << datatypeNames.at((size_t)datatype) << "\"";
        TypedValue error = TypedValue(DataType::ERROR, ss.str());
        return error;
    }


} // namespace internal
} // namespace RlaT
