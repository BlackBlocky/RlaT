cmake_minimum_required(VERSION 3.10)
project(RlaT VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(RlaT_Libary_Sources
    src/RlaT/RlaT_Script.h
    src/RlaT/RlaT_Script.cpp
    src/RlaT/RlaT_Data.h
    src/RlaT/RlaT_Data.cpp
    src/RlaT/RlaT_ProcessTree.h
    src/RlaT/RlaT_ProcessTree.cpp
    src/RlaT/RlaT_ProcessElement.h
    src/RlaT/RlaT_ProcessElement.cpp
)
add_library(rlatLibary STATIC ${RlaT_Libary_Sources})

add_executable(RlaT 
    src/testCode/main.cpp    
)

target_link_libraries(RlaT PRIVATE rlatLibary)
target_include_directories(RlaT PRIVATE src/RlaT)

set_property(TARGET RlaT PROPERTY CXX_STANDARD 17)