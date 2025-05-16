#include "RlaT_Data.h"

#include <string>
#include "RlaT_Script.h"
#include <sstream>

using namespace std;

namespace RlaT {
namespace internal {
    
    // Inits
    const std::array<std::string, 3> RlaT_Data::datatypeNames = {
        "empy",  // 0
        "error", // 1
        "int"    // 2
    };
    const std::array<std::string, 5> RlaT_Data::operationNames = {
        "set", // 0
        "add", // 1
        "sub", // 2
        "mul", // 3
        "div"  // 4
    };
    const std::array<std::function<RlaT_Data(RlaT_Data, RlaT_Data)>, 5> RlaT_Data::operationMethods = {
        evSet,
        evAdd,
        evSub,
        evMul,
        evDiv
    };


    // Methods
    RlaT_Data::RlaT_Data(DataType type, any defaultValue) {
        this-> type = type;
        this-> value = defaultValue;
    }

    DataType RlaT_Data::getType() {
        return this->type;
    }  
    any RlaT_Data::getAnyValue() {
        return this->value;
    }

    string RlaT_Data::toString() {
        
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


    RlaT_Data RlaT_Data::evaluate(RlaT_Data a, RlaT_Data b, OperatorType operation) {
        // Check if the Datatypes are the same
        if(a.getType() != b.getType()) {
            stringstream ss;
            ss << "Unmatching Datatypes \"" << datatypeNames.at((size_t)a.getType()) << "\" and \"" << datatypeNames.at((size_t)b.getType()) << "\"";
            return RlaT_Data(DataType::ERROR, ss.str());
        }

        return operationMethods.at((size_t)operation)(a, b);
    }

    RlaT_Data RlaT_Data::evSet(RlaT_Data a, RlaT_Data b) {
        // At this point, we know that both have the same Data-Type
        return RlaT_Data(b.getType(), b.getAnyValue());
    }

    RlaT_Data RlaT_Data::evAdd(RlaT_Data a, RlaT_Data b) {
        // At this point, we know that both have the same Data-Type
        switch(a.getType()) {
            case DataType::INTEGER: {
                int aValue = any_cast<int>(a.getAnyValue());
                int bValue = any_cast<int>(b.getAnyValue());

                RlaT_Data result = RlaT_Data(a.getType(), aValue + bValue);
                return result;
            }
        }

        // If the Datatype is not supported, return an error
        return evCreateError(a.getType(), OperatorType::ADD);
    }

    RlaT_Data RlaT_Data::evSub(RlaT_Data a, RlaT_Data b) {
        // At this point, we know that both have the same Data-Type
        switch(a.getType()) {
            case DataType::INTEGER: {
                int aValue = any_cast<int>(a.getAnyValue());
                int bValue = any_cast<int>(b.getAnyValue());

                RlaT_Data result = RlaT_Data(a.getType(), aValue - bValue);
                return result;
            }
        }

        // If the Datatype is not supported, return an error
        return evCreateError(a.getType(), OperatorType::SUB);
    }

    RlaT_Data RlaT_Data::evMul(RlaT_Data a, RlaT_Data b) {
        // At this point, we know that both have the same Data-Type
        switch(a.getType()) {
            case DataType::INTEGER: {
                int aValue = any_cast<int>(a.getAnyValue());
                int bValue = any_cast<int>(b.getAnyValue());

                RlaT_Data result = RlaT_Data(a.getType(), aValue * bValue);
                return result;
            }
        }

        // If the Datatype is not supported, return an error
        return evCreateError(a.getType(), OperatorType::MUL);
    }

    RlaT_Data RlaT_Data::evDiv(RlaT_Data a, RlaT_Data b) {
        // At this point, we know that both have the same Data-Type
        switch(a.getType()) {
            case DataType::INTEGER: {
                int aValue = any_cast<int>(a.getAnyValue());
                int bValue = any_cast<int>(b.getAnyValue());

                RlaT_Data result = RlaT_Data(a.getType(), aValue / bValue);
                return result;
            }
        }

        // If the Datatype is not supported, return an error
        return evCreateError(a.getType(), OperatorType::DIV);
    }

    RlaT_Data RlaT_Data::evCreateError(DataType datatype, OperatorType op) {
        stringstream ss;
        ss << "Can not do the operation \"" << operationNames.at((size_t)op) << "\" on the Data-Type \"" << datatypeNames.at((size_t)datatype) << "\"";
        RlaT_Data error = RlaT_Data(DataType::ERROR, ss.str());
        return error;
    }


} // namespace internal
} // namespace RlaT
