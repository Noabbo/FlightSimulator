//
// Created by noa on 21/12/2019.
//

#include "server.h"
int main(int argc, char* argv[]) {
    // lexer
    vector<string> parameters = lexer(text);
    return 0;
}

vector<string> lexer(string file_name) {
    vector<string> v;
    string line;
    string openDataServer = "openDataServer";
    string connectControlClient = "connectControlClient";
    string var = "var";
    string Print = "Print";
    string Sleep = "Sleep";
    // open file sent in argument
    fstream text;
    text.open(file_name, ios::in | ios::binary);
    if (!text) {
        throw "error - file could not open";
    }
    getline(file, line);
    while (!line.empty()) {
        if (line.find("openDataServer")) {

        }
        getline(file, line);
    }
    text.close();
    return v;
}

int OpenServerCommand::execute(string parameters) {
    int port = stoi(parameters);
    thread serverth(openServer, port);
    serverth.join();
    return OPEN_SERVER_COMMAND_RET_VALUE;
}

void openServer(int port) {
    mutex_lock.lock();
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        throw "error - could not create a socket";
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    mutex_lock.unlock();
}

int DefineVarCommand::execute(string parameters) {

}

int SetVarCommand::execute(string parameters) {
    // extracting the name and new value of variable
    size_t pos = parameters.find("=");
    string name = parameters.substr(0, pos);
    string exp = parameters.substr(pos + 1);
    // calculating new value of variable
    Interpreter i = new Interpreter(vars_map);
    Expression* e = i.interpret(exp);
    double value = e->calculate();
    // set new value in map
    vars_map.at(name).setValue(value);
    // simulator needs to be updated
    if (vars_map.find(name).getUpdateSim()) {
        string updateSimulator = "set" + vars_map.find(name).getSim() + " " + to_string(vars_map.find(name).getValue());
        // send setting of variable to simulator
        // Fanny - please dp that
    }
}
