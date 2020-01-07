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
#define CONNECT_COMMAND_RET_VALUE 3
#define NEW_VARIABLE_RET_VALUE 5
#define PRINT_SLEEP_FUNC_RET_VALUE 2
#define NEW_VALUE_OF_VARIABLE_RET_VALUE 2
#define ALIAS_RET_VALUE 4

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
void connectClient(const char *IP, int port,  unordered_map<string, Variable> game_configuration,
        vector<vector<string>> game_value);
void runExecute( vector<string> parameters, int client_sockect);
bool xmlParser();
void blockParser(vector<string> parameters, bool ifOrWhile, int client_socket);

// class of open server command
class OpenServerCommand : public Command {
public:
    virtual string execute(vector<string> parameters);
    virtual ~OpenServerCommand() {delete this;};
};
// class of connect client command
class ConnectCommand : public Command {
public:
    virtual string execute(vector<string> parameters);
    virtual ~ConnectCommand() {delete this;};

};
// class of define var command
class DefineVarCommand : public Command {
public:
    virtual string execute(vector<string> parameters);
    virtual ~DefineVarCommand() {delete this;};
};
// class of print command
class PrintCommand : public Command {
public:
    virtual string execute(vector<string> parameters);
    virtual ~PrintCommand() {delete this;};

};
// class of sleep command
class SleepCommand : public Command {
public:
    virtual string execute(vector<string> parameters);
    virtual ~SleepCommand() {delete this;};

};
class FuncCommand : public Command {
    vector<string> commands;
public:
    FuncCommand(vector<string> c);
    void initFunc(vector<string> parameters);
    virtual string execute(vector<string> parameters);
    void executeFunc(string name, double var, int client_socket);
    //virtual ~FuncCommand() {delete this;};
};

unordered_map<string, Command*> commands_map;
vector<Variable> orderVars;
unordered_map<string, Variable> xml_configuration;
unordered_map<string, Variable> game_configuration;
vector<vector<string>> game_operation;
unordered_map<string, FuncCommand> func_map;
mutex mutex_lock;
#endif //PART_1_SERVER_H
