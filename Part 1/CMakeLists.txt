cmake_minimum_required(VERSION 3.15)
project(Part_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Part_1 Command.cpp Command.h BinaryOperator.h Expression.h Variable.cpp Variable.h Value.cpp Value.h Plus.cpp Plus.h Minus.cpp Minus.h Mul.cpp Mul.h Div.cpp Div.h UnaryOperator.h UPlus.cpp UPlus.h UMinus.cpp UMinus.h UnaryOperator.cpp BinaryOperator.cpp Interpreter.cpp Interpreter.h Interpreter.cpp Interpreter.h server.cpp server.h)