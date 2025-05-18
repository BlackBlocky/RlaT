#pragma once
#include <string>
#include <functional>

namespace RlaT {

class RlaTScript {
public:
    void setOutputMethod(std::function<void(std::string)>);
    void setOutputErrorMethod(std::function<void(std::string)>);

    void outputString(std::string);
    void outputErrorString(std::string);

    void ping();
    

private:
    std::function<void(std::string)> outputFunction;
    std::function<void(std::string)> outputErrorFunction;
};

}


