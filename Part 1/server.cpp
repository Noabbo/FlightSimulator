//
// Created by noa on 21/12/2019.
//

#include "server.h"
#define OPEN_SERVER_COMMAND_RET_VALUE 2
unordered_map<string, Command*> commands;
list<Variable> vars;
unordered_map<string, typename list<Variable>::iterator> vars_map;
unordered_map<string, typename list<Variable>::iterator> sim_map;
mutex mutex_lock;
int main(int argc, char* argv[]) {
    // open file sent in argument
    fstream text;
    text.open(argv[1], ios::in | ios::binary);
    if (!text) {
        throw "error - file could not open";
    }
    // lexer
    vector<string> parameters = lexer(text);
    return 0;
}

vector<string> lexer(fstream file) {
    vector<string> v;
    string line;
    getline(file, line);
    while (!line.empty()) {

        getline(file, line);
    }
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
