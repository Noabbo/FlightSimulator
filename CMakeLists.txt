cmake_minimum_required(VERSION 3.15)
project(Part_1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(Command.cpp Command.h BinaryOperator.h Expression.h Variable.cpp Variable.h Value.cpp Value.h Plus.cpp
        Plus.h Minus.cpp Minus.h Mul.cpp Mul.h Div.cpp Div.h UnaryOperator.h UPlus.cpp UPlus.h UMinus.cpp UMinus.h
        UnaryOperator.cpp BinaryOperator.cpp Interpreter.cpp Interpreter.h main.cpp main.h BooleanType.cpp
        BooleanType.h tinyxml2.cpp tinyxml2.h)
