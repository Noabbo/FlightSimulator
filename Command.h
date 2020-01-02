//
// Created by noa on 21/12/2019.
//

#ifndef PART_1_COMMAND_H
#define PART_1_COMMAND_H
#include <string>
#include <vector>
using namespace std;
class Command {
public:
    virtual int execute(vector<string> parameters) = 0;
    virtual ~Command() {};
};


#endif //PART_1_COMMAND_H
