#include <iostream>
#include <RlaT_Script.h>
#include <RlaT_TypedValue.h>

#include <string>

using namespace std;

int main();
void coolMethod(string);
void badMethod(string);

string white = "\e[0;37m";
string cyan = "\e[0;36m";
string red = "\e[0;31m";

int main() {
    
    RlaT::RlaTScript newScript = RlaT::RlaTScript();

    newScript.setOutputMethod(coolMethod);
    newScript.setOutputErrorMethod(badMethod);
    newScript.ping();


    cout << white;
    return 0;
}

void coolMethod(string s) {
    cout << cyan << "Output: " << s << white << std::endl;
}

void badMethod(string s) {
    cout << red << "Error: " << s << white << std::endl;
}

