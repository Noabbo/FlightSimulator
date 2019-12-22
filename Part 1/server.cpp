//
// Created by noa on 21/12/2019.
//
#include <thread>
#include <string>
#include <unordered_map>
#include <list>
#include "Command.h"
#include "OpenServerCommand.h"
#include "Expression.h"
#include "Value.h"
#include "Variable.h"
#include "UnaryOperator.h"
#include "UPlus.h"
#include "UMinus.h"
#include "BinaryOperator.h"
#include "Plus.h"
#include "Minus.h"
#include "Mul.h"
#include "Div.h"
#include "Interpreter.h"

list<thread> threads;
unordered_map<string, Command*> commands;
list<Variable> vars;
unordered_map<string, typename list<Variable>::iterator> vars_map;
unordered_map<string, typename list<Variable>::iterator> sim_map;
int main(int argc, char* argv[]) {

}
