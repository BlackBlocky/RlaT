#pragma once
#include <string>
#include <functional>

namespace RlaT {

class RlaT_Script {
public:
    void setOutputMethod(std::function<void(std::string)>);
    void setOutputErrorMethod(std::function<void(std::string)>);

    void ping();
    

private:
    void outputString(std::string);
    void outputErrorString(std::string);

    std::function<void(std::string)> outputFunction;
    std::function<void(std::string)> outputErrorFunction;
};

}


