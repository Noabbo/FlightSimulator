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
using namespace std;
vector<string> lexer(fstream file);
void openServer(int port);
class OpenServerCommand : public Command {
public:
    virtual int execute(string parameters);

};

#endif //PART_1_SERVER_H
