//
// Created by noa on 23/12/2019.
//

#ifndef PART_1_SERVER_H
#define PART_1_SERVER_H
#include <algorithm>
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
#include "BooleanType.h"
#include <arpa/inet.h>
#include "tinyxml2.h"

#define OPEN_SERVER_COMMAND_RET_VALUE 2
#define MAX_CONNECTED_CLIENTS 10
#define CONNECT_COMMAND_RET_VALUE 3
#define NUM_VARIABLE 36
using namespace std;
void init();
vector<string> lexer(string file_name);
vector<string> helpLexer(string line);
string lowerCase(string s);
string upperCase(string s);
string removeSpaces(string str);
void parser(vector<string> coms);
vector<string> findBlock(vector<string> coms, int pos);
void openServer(int port);
void connectClient(int port);
void openDataServer();
bool xmlParser();
void Sleep(double number);
void Print(string str);
void blockParser(vector<string> parameters, bool ifOrWhile);
// class of open server command
class OpenServerCommand : public Command {
public:
    virtual int execute(vector<string> parameters);
    virtual ~OpenServerCommand() {delete this;};
};
// class of connect client command
class ConnectCommand : public Command {
public:
    virtual int execute(vector<string> parameters);
    virtual ~ConnectCommand() {delete this;};

};
// class of define var command
class DefineVarCommand : public Command {
public:
    virtual int execute(vector<string> parameters);
    virtual ~DefineVarCommand() {delete this;};
};
// class of print command
class PrintCommand : public Command {
public:
    virtual int execute(vector<string> parameters);
    virtual ~PrintCommand() {delete this;};

};
// class of sleep command
class SleepCommand : public Command {
public:
    virtual int execute(vector<string> parameters);
    virtual ~SleepCommand() {delete this;};

};
class FuncCommand : public Command {
    vector<string> commands;
public:
    FuncCommand(vector<string> c);
    virtual int execute(vector<string> parameters);
    void executeFunc(string name, double var);
    virtual ~FuncCommand() {delete this;};
};

unordered_map<string, Command*> commands;
list<Variable> vars;
unordered_map<string, typename list<Variable>::iterator> vars_map;
unordered_map<string, typename list<Variable>::iterator> sim_map;
unordered_map<string, FuncCommand> func_map;
mutex mutex_lock;
#endif //PART_1_SERVER_H
