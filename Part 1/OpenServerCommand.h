//
// Created by noa on 20/12/2019.
//

#ifndef PART_1_OPENSERVERCOMMAND_H
#define PART_1_OPENSERVERCOMMAND_H

#include <string>
#include "Command.h"
class OpenServerCommand : public Command {
public:
    virtual int execute(string parameters);
    virtual ~OpenServerCommand();
};


#endif //PART_1_OPENSERVERCOMMAND_H
