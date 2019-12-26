//
// Created by noa on 23/12/2019.
//

#ifndef PART_1_SERVER_H
#define PART_1_SERVER_H
#include <thread>
#include <string>
#include <unordered_map>
#include <list>
#include <iostream>
#include <fstream>
#include <mutex>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include "Command.h"
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
#define OPEN_SERVER_COMMAND_RET_VALUE 2
using namespace std;
unordered_map<string, Command*> commands;
list<Variable> vars;
unordered_map<string, typename list<Variable>::iterator> vars_map;
unordered_map<string, typename list<Variable>::iterator> sim_map;
unordered_map<string, FuncCommand> func_map;
mutex mutex_lock;
vector<string> lexer(string file_name);
void openServer(int port);
// class of open server command
class OpenServerCommand : public Command {
public:
    virtual int execute(string parameters);

};
// class of define var command
class DefineVarCommand : public Command {
public:
    virtual int execute(string parameters);
};
// class of set var command
class SetVarCommand : public Command {
    virtual int execute(string parameters);
};

class DefineFuncCommand : public Command {
    virtual int execute(string parameters);
};

class FuncCommand : public Command {
    virtual int execute(string parameters);
};
#endif //PART_1_SERVER_H
