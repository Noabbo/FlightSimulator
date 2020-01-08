//
// Created by noa on 21/12/2019.
//

#include "main.h"
bool ClientDone = false;
bool ServerExist = true;
int main(int argc, char* argv[]) {
    try {
        // initialize map of commands
        init();
        // lexer
        vector<string> parameters = lexer(argv[1]);
        //parser
        parser(parameters);
    } catch (char const* e) {
        cout << e << endl;
    }
    return 0;
}
// initialization of map of commands
void init() {
    Command *c = new OpenServerCommand();
    commands_map.emplace(make_pair("openDataServer", c));
    c = new ConnectCommand();
    commands_map.emplace(make_pair("connectControlClient", c));
    c = new DefineVarCommand();
    commands_map.emplace(make_pair("var", c));
    c = new PrintCommand();
    commands_map.emplace(make_pair("Print", c));
    c = new SleepCommand();
    commands_map.emplace(make_pair("Sleep", c));
}
// lexer
vector<string> lexer(string file_name) {
    vector<string> v;
    string line;
    // open file sent in argument
    fstream text;
    text.open(file_name, ios::in | ios::binary);
    if (!text) {
        throw "error - file could not open";
    }
    getline(text, line);
    while (!text.eof()) {
        vector<string> help = helpLexer(line);
        for (string s : help) {
            if (!s.empty()) {
                v.push_back(s);
            }
        }
        getline(text, line);
    }
    text.close();
    return v;
}
// helper of lexer
vector<string> helpLexer(string line) {
    vector<string> v;
    // open server command of sleep or print command
    if ((line.find("openDataServer") != string::npos)
        || (line.find(lowerCase("openDataServer")) != string::npos)
        || (line.find(upperCase("openDataServer")) != string::npos)
        || (line.find("Sleep") != string::npos)
        || (line.find(lowerCase("Sleep")) != string::npos)
        || (line.find(upperCase("Sleep")) != string::npos)
        || (line.find("Print") != string::npos) || (line.find(lowerCase("Print")) != string::npos)
        || (line.find(upperCase("Print")) != string::npos)) {
        // enter name of command to vector
        auto startPos = line.find('(');
        v.push_back(removeSpaces(line.substr(0, startPos)));
        // enter parameter to vector
        auto endPos = line.find_last_of(")");
        string par;
        if (line.at(startPos+1) == '"') {
            par = line.substr(startPos+1, endPos-startPos-1);
        } else {
            par = removeSpaces(line.substr(startPos+1, endPos-startPos-1));
        }
        v.push_back(par);
    } else if ((line.find("connectControlClient") != string::npos)
               || (line.find(lowerCase("connectControlClient")) != string::npos)
               || (line.find(upperCase("connectControlClient")) != string::npos)) {
        // connect client command
        // enter name of command to vector
        auto startPos = line.find("(");
        string name = removeSpaces(line.substr(0, startPos));
        v.push_back(name);
        // enter parameters to vector
        auto endPos = line.find(")");
        string pars = removeSpaces(line.substr(startPos+1, endPos-startPos-1));
        auto commaPos = pars.find(",");
        string ip = removeSpaces(pars.substr(0, commaPos));
        string port = removeSpaces(pars.substr(commaPos+1));
        v.push_back(ip);
        v.push_back(port);
    } else if (line.find("{") != string::npos) {
        auto curlPos = line.find("{");
        // while, if function or define new function
        if (line.find("if") != string::npos) {
            v.emplace_back("if");
            auto pos = line.find("if");
            string condition = removeSpaces(line.substr(pos+2));
            condition.pop_back();
            v.push_back(condition);
        } else if (line.find("while") != string::npos) {
            v.emplace_back("while");
            auto pos = line.find("while");
            string condition = removeSpaces(line.substr(pos+5));
            condition.pop_back();
            v.push_back(condition);
        } else {
            auto startPos = line.find("(");
            string name = removeSpaces(line.substr(0, startPos));
            v.push_back(name);
            auto endPos = line.find(")");
            auto variable = line.substr(startPos + 1, endPos - startPos - 1);
            v.push_back(variable);
        }
        v.emplace_back("{");
    } else if ((line.find("var") != string::npos) || (line.find(lowerCase("var")) != string::npos)
               || (line.find(upperCase("var")) != string::npos)) {
        // define var command
        v.emplace_back("var");
        string name;
        string sim;
        // finding arrow in line
        size_t sign;
        if (line.find("->") != string::npos) {
            sign = line.find("->");
            name = removeSpaces(line.substr(3, sign-3));
            // adding name and arrow to vector
            v.push_back(name);
            v.emplace_back("->");
            v.emplace_back("sim");
            // find path to simulator
            auto pos = line.find("(");
            sim = removeSpaces(line.substr(pos+1));
            // remove (
            sim.pop_back();
            v.push_back(sim);
        } else if (line.find("<-") != string::npos) {
            sign = line.find("<-");
            name = removeSpaces(line.substr(3, sign-3));
            // adding name and arrow to vector
            v.push_back(name);
            v.emplace_back("<-");
            v.emplace_back("sim");
            // find path to simulator
            auto pos = line.find("(");
            sim = removeSpaces(line.substr(pos+1));
            // remove (
            sim.pop_back();
            v.push_back(sim);
        } else {
            // no need to update simulator - "=" is there
            name = removeSpaces(line.substr(3, sign-4));
            // adding name and arrow to vector
            v.push_back(name);
            v.emplace_back("=");
            // add value to vector
            sign = line.find('=');
            string exp = removeSpaces(line.substr(sign+1));
            v.push_back(exp);
        }
    } else if (line == "}") {
        // end of block
        v.emplace_back("}");
    } else {
        // update var, call a function or condition
        if (line.find("<") != string::npos) {
            auto pos = line.find("<");
            auto posEnd = pos;
            auto cond = 1;
            if (line.find("<=") != string::npos) {
                posEnd += 1;
                cond++;
            }
            string name = removeSpaces(line.substr(0, pos));
            v.push_back(name);
            string condition = removeSpaces(line.substr(pos, cond));
            v.push_back(condition);
            string value = removeSpaces(line.substr(posEnd+1));
            v.push_back(value);
        } else if (line.find(">") != string::npos) {
            auto pos = line.find(">");
            auto posEnd = pos;
            auto cond = 1;
            if (line.find(">=") != string::npos) {
                posEnd += 1;
                cond++;
            }
            string name = removeSpaces(line.substr(0, pos));
            v.push_back(name);
            string condition = removeSpaces(line.substr(pos, cond));
            v.push_back(condition);
            string value = removeSpaces(line.substr(posEnd+1));
            v.push_back(value);
        } else if (line.find("=") != string::npos) {
            auto pos = line.find("=");
            auto posEnd = pos;
            auto cond = 1;
            if ((line.find("==") != string::npos) || (line.find("!=") != string::npos)) {
                posEnd += 1;
                cond++;
            }
            string name = removeSpaces(line.substr(0, pos));
            v.push_back(name);
            string condition = removeSpaces(line.substr(pos, cond));
            v.push_back(condition);
            string value = removeSpaces(line.substr(posEnd+1));
            v.push_back(value);
        } else {
            // calling a function
            auto pos = line.find("(");
            string name = removeSpaces(line.substr(0, pos));
            v.push_back(name);
            string exp = removeSpaces(line.substr(pos+1));
            // remove (
            exp.pop_back();
            v.push_back(exp);
        }
    }
    return v;
}
// turns string's letters to all lower cases
string lowerCase(string s) {
    string newString;
    for (char i : s) {
        newString.push_back(::tolower(i));
    }
    return newString;
}
// turns string's letters to all upper cases
string upperCase(string s) {
    string newString;
    for (char i : s) {
        newString.push_back(::tolower(i));
    }
    return newString;
}
// removes excess spaces and tabs in lines extracted from text file
string removeSpaces(string str) {
    string newStr;
    for (char i : str) {
        if ((i != ' ') && (i != '\t')) {
            newStr.push_back(i);
        }
    }
    return newStr;
}
// parser
void parser(vector<string> rawConfig) {
    int i = 0;
    int server_port = 0;
    pair<string, int> client_config;
    while (i < rawConfig.size()) {
        // finding parameters for commands
        if (rawConfig[i] == "openDataServer") {
            server_port = stoi(rawConfig[i + 1]);
            Command *c = commands_map[rawConfig[i]];
            vector<string> param;
            param.push_back(to_string(server_port));
            c->execute(param);
            i += OPEN_SERVER_COMMAND_RET_VALUE;

        } else if (rawConfig[i] == "connectControlClient") {
            string IP = rawConfig[i + 1].substr(1);
            IP.pop_back();
            client_config = make_pair(IP , stoi(rawConfig[i + 2]));
            i += CONNECT_COMMAND_RET_VALUE;

        } else if (rawConfig[i] == "var") {
            // Creation of new variable from preexisting variable
            if (rawConfig[i + 2] == "=") {
                vector<string> command;
                for (int j = 0; j < ALIAS_RET_VALUE; j++) {
                    command.push_back(rawConfig[i + j]);
                }
                game_operation.push_back(command);
                i += ALIAS_RET_VALUE;
            } else {
                //Initialisation of the variables
                Command *c = new DefineVarCommand();
                vector<string> command;
                for (int j = 0; j < NEW_VARIABLE_RET_VALUE; j++) {
                    command.push_back(rawConfig[i + j]);
                }
                //Add to the map the new variable
                c->execute(command);
                i += NEW_VARIABLE_RET_VALUE;
            }
        }
        else if ((rawConfig[i] == "Print") || (rawConfig[i] == "Sleep")) {
            vector<string> command;
            for (int j = 0; j < PRINT_SLEEP_FUNC_RET_VALUE; j++) {
                command.push_back(rawConfig[i + j]);
            }
            game_operation.push_back(command);
            i += PRINT_SLEEP_FUNC_RET_VALUE;
        } else if (rawConfig[i + 2] == "{") {
            // undeclared function
            vector<string> block = findBlock(rawConfig, i);
            block.insert(block.begin(), "FuncCommand");
            // new command to declare
            if ((rawConfig[i] != "if") && (rawConfig[i] != "while")) {
                FuncCommand *c = new FuncCommand(block);
                c->initFunc(block);
            } else {
                game_operation.push_back(block);
            }
            i += block.size()-1;
        } else if (func_map.find(rawConfig[i]) != func_map.end()) {
            // declared function
            // enter variable
            vector<string> func;
            func.insert(func.begin(), "FuncCommand");
            for (int j = 0; j < PRINT_SLEEP_FUNC_RET_VALUE; j++) {
                func.push_back(rawConfig[i + j]);
            }
            game_operation.push_back(func);
            i += PRINT_SLEEP_FUNC_RET_VALUE;
        } else if (game_configuration.find(rawConfig[i]) != game_configuration.end()) {
            // declared variable
            vector<string> var;
            var.insert(var.begin(), "DefineVarCommand");
            for (int j = 0; j < NEW_VALUE_OF_VARIABLE_RET_VALUE; j++) {
                var.push_back(rawConfig[i + j]);
            }
            game_operation.push_back(var);
            i += NEW_VALUE_OF_VARIABLE_RET_VALUE;
        }  else {
            throw "error - no matching command found";
        }
    }
    thread serverth(openServer, server_port);
    thread clientth(connectClient, client_config.first.c_str(), client_config.second, game_configuration, game_operation);
    clientth.join();
    serverth.join();
}

// enters a block to a vector
vector<string> findBlock(vector<string> coms, int pos) {
    vector<string> param;
    bool first = true;
    int count = 1;
    // entering condition and block to vector
    while (count > 0) {
        if (coms[pos] == "{") {
            if (!first) {
                count++;
            }
            first = false;
        }
        if ((coms[pos] == "}") && (count > 0)) {
            count--;
        } else if ((coms[pos] == "}") && (count == 0)) {
            // and of block
            param.emplace_back("}");
            break;
        }
        param.push_back(coms[pos++]);
    }
    // enter end of block
    /*for (int i = 0; i < count; i++) {
        param.emplace_back("}");
    }*/
    return param;
}
// command to create and connect server
string OpenServerCommand::execute(vector<string> parameters) {
    xmlParser();
    return "";
}

//Create the server
void openServer(int port) {
    //Creation of the socket
    char buffer[1024];
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        throw "error - could not create a socket";
    }
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    //Case the bind socket doesn't bind to the IP address
    if (bind(socketfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
        throw "Could not bind the socket to an IP";
    }
    //Socket listen to the port
    if (listen(socketfd, 5) == -1) {
        throw "Error during listening command";
    }
    //Accepting the client
    cout << "Waiting for acceptation " << port << endl;
    int client_socket = accept(socketfd, (struct sockaddr *) &serverAddress, (socklen_t *) &serverAddress);
    if (client_socket == -1) {
        throw "Error- didn't accept client";
    }
    // The server keep listening to the client.
    while (ServerExist) {
        //Wait to listen from the client.
        int res = read(client_socket, buffer, sizeof(buffer));
        // the server sent information
        if (buffer[0] != '\0') {
            mutex_lock.lock();
            vector<Variable>::iterator it;
            int i = 0;
            //Create the string we receive from the buffer
            for (it = orderVars.begin(); it != orderVars.end(); it++) {
                string bufferLine = "";
                while (buffer[i] != ',') {
                    bufferLine += buffer[i];
                    i++;
                }
                //Split the "."
                i++;
                //Update each element of our map with the value receive from the server.
                it->setValue(atof(bufferLine.c_str()));
                string name = it->getName();
                if (game_configuration.find(name) != game_configuration.end())  {
                    game_configuration.at(name).setValue(atof(bufferLine.c_str()));
                }
            }
            mutex_lock.unlock();
        }
        if (ClientDone) {
            break;
        }
    }
    close(socketfd);
}

//Create connection with the server
void connectClient(const char *IP, int port,  unordered_map<string, Variable> game_configuration,
        vector<vector<string>> game_operation) {
    //Creation of the socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        throw "error - could not create a socket";
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
   // address.sin_addr.s_addr = inet_addr(IP);
    address.sin_port = htons(port);
    cout << "Client " << IP <<" " << port << endl;

    if(inet_pton(AF_INET, IP, &address.sin_addr) <= 0) {
        cout << "Client Error" << IP <<" " << port << endl;

        throw("Invalid address - Address not supported");
    }
    //Request a connection with the server on local host, try until the connection is made.
    while (true) {
        int is_connected = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
        cout << "Client is connected "<< is_connected << endl;
        if (is_connected == -1) {
            sleep(1);
        } else {
            int i = 0;
            //Send information to our server
            for (i;  i < game_operation.size(); ++i) {
                runExecute(game_operation[i], client_socket);
            }
            // executions have finished
            if (i == game_operation.size()){
                cout << "Connection Over " << endl;
                break;
            }
        }
        //Close the socket whe the connection is done or do not succeed.
        close(client_socket);
    }
    //Close the socket whe the connection is done or do not succeed.
    close(client_socket);
    ClientDone = true;

}

// run the commands in txt file
void runExecute(vector<string> parameters, int client_socket) {
    string opType = parameters.front();
    Command *c = commands_map[opType];
    // no matchimg command in list - two options
    if (c == nullptr) {
        if (opType == "FuncCommand") {
            parameters.erase(parameters.begin());
            c = new FuncCommand(parameters);
            parameters.insert(parameters.begin(), to_string(client_socket));
            c->execute(parameters);
        } else if (opType == "DefineVarCommand") {
            parameters.erase(parameters.begin());
            c = new DefineVarCommand();
            mutex_lock.lock();
            string updateSimulator = c->execute(parameters);
            mutex_lock.unlock();
            if (!updateSimulator.empty()) {
                 auto rel = write(client_socket, updateSimulator.c_str(), updateSimulator.size() + 1);
            }
        } else if (opType == "{") {
            mutex_lock.lock();
            parameters.erase(parameters.begin());
            parameters.erase(parameters.end());
            Command *c;
            vector<string> operation;
            while (parameters.begin() != parameters.end()) {
                if (game_configuration.find(parameters.front()) != game_configuration.end()) {
                    c = new DefineVarCommand();
                    for (int j = 0; j < 3; j++) {
                        operation.push_back(parameters.front());
                        parameters.erase(parameters.begin());
                    }
                } else {
                    if (parameters.front() == "Sleep") {
                        c = new SleepCommand();
                    } else {
                        c = new PrintCommand();
                    }
                    for (int j = 0; j < 2; j++) {
                        operation.push_back(parameters.front());
                        parameters.erase(parameters.begin());
                    }
                }
               // mutex_lock.lock();
                c->execute(operation);
                mutex_lock.unlock();
                operation.clear();
            }
        }
    } else {
        mutex_lock.lock();
        c->execute(parameters);
        mutex_lock.unlock();
    }
}

// translates the xml file
bool xmlParser() {
    tinyxml2::XMLDocument xml_doc;

    tinyxml2::XMLError eResult = xml_doc.LoadFile("generic_small.xml");
    if (eResult != tinyxml2::XML_SUCCESS) {
        return false;
    }

    tinyxml2::XMLNode* root = xml_doc.FirstChildElement("PropertyList")->FirstChildElement("generic")->FirstChildElement("output");
    if (root == nullptr) {
        return false;
    }

    tinyxml2::XMLElement* element = root->FirstChildElement("chunk");
    if (element == nullptr) {
        return false;
    }

    //Enter all our variables into our map.
    while (element != nullptr) {
        string name  = element->FirstChildElement("name")->GetText();
        string type  = element->FirstChildElement("type")->GetText();
        string format  = element->FirstChildElement("format")->GetText();
        string node  = element->FirstChildElement("node")->GetText();
        Variable *variable = new Variable(name, 0, false, node);
        orderVars.push_back(*variable);
        xml_configuration.emplace(make_pair(node, *variable));
        element = element->NextSiblingElement();
    }
}

// command that defines a new or existing variable
string DefineVarCommand::execute(vector<string> parameters) {
    string name;
    // define new variable
    if (parameters[0] == "var") {
        name = parameters[1];
        if (parameters[2] == "=") {
            // creating new local variable
            auto i = new Interpreter(game_configuration);
            Expression* e = i->interpret(parameters[3]);
            double value = e->calculate();
            auto *v = new Variable(name, value, false, "");
            // add new variable to map
            game_configuration.emplace(make_pair(name, *v));
        } else if (parameters[2] == "->") {
            // extracting the sum path
            string path = parameters[4].substr(1);
            path.pop_back();
            // new variable - create new one in list and map
            if (xml_configuration.find(path) == xml_configuration.end()) {
                auto *v = new Variable(name, 0, true, path);
                // add new variable to map
                orderVars.push_back(*v);
                game_configuration.emplace(make_pair(name, *v));
            } else {
                // update boolean type and sim path in variable
                int i = 0;
                while (orderVars[i].getPath() != path) {
                    i++;
                }
                orderVars[i].setName(name);
                orderVars[i].setUpdateSimulator(true);
                Variable v = orderVars[i];
                game_configuration.emplace(make_pair(name, v));
                xml_configuration.erase(xml_configuration.find(path));
            }
        } else {
            // parameters[2] = "<-"
            // update path to simulator in variable
            string path = parameters[4].substr(1);
            path.pop_back();
            // new variable - create new one in list and map
            if (xml_configuration.find(path) == xml_configuration.end()) {
                auto *v = new Variable(name, 0, false, path);
                // add new variable to map
                orderVars.push_back(*v);
                game_configuration.emplace(make_pair(name, *v));
            } else {
                // update sim path in variable
                int i = 0;
                while (orderVars[i].getPath() != path) {
                    i++;
                }
                orderVars[i].setName(name);
                Variable v = orderVars[i];
                game_configuration.emplace(make_pair(name, v));
            }
        }
    } else {
        if (!xml_configuration.empty()) {
            for (auto pair : xml_configuration) {
                Variable v = pair.second;
                game_configuration.emplace(make_pair(v.getName(), v));
            }
            xml_configuration.clear();
        }
        // update existing variable
        // extracting the name and new value of variable
        name = parameters[0];
        string exp = parameters[2];
        // calculating new value of variable
        auto i = new Interpreter(game_configuration);
        Expression* e = i->interpret(exp);
        double value = e->calculate();
        // set new value in map and list
        game_configuration.at(name).setValue(value);
        for (Variable v : orderVars) {
            if (v.getName() == name) {
                v.setValue(value);
                break;
            }
        }
        // simulator needs to be updated
        if (game_configuration.at(name).getUpdateSimulator()) {
            string updateSim = "set";
            updateSim += game_configuration.at(name).getPath();
            updateSim += " ";
            updateSim += to_string(game_configuration.at(name).getValue());
            updateSim += "\r\n";
            return updateSim;
        }
    }
    return "";

}

string ConnectCommand::execute(vector<string> parameters) {
    return "";
}

// constructor for func command
FuncCommand::FuncCommand(vector<string> c) {
    this->commands = c;
}

// execute func command - bonus!
string FuncCommand::execute(vector<string> parameters) {
    //Give the socket of the client
    int client_socket = stoi(parameters[0]);
    bool isDeclaredFunc = true;
    // find out what type of function to handle
    for (string s : parameters) {
        if (s == "{") {
            isDeclaredFunc = false;
            break;
        }
    }
    // Run declared function
    if (isDeclaredFunc) {
        auto f = func_map.at(parameters[1]);
        Interpreter *i = new Interpreter(game_configuration);
        Expression *e = i->interpret(parameters[2]);
        double var = e->calculate();
        f.executeFunc(var, client_socket);
        // run function
        return "";
    } else {
        // The function as a if condition
        if (parameters[1] == "if") {
            Interpreter *i = new Interpreter(game_configuration);
            string cond = parameters[2];
            vector<string> condition = helpLexer(cond);
            // left part of the condition
            Expression *l = i->interpret(condition[0]);
            // right part of the condition
            Expression *r = i->interpret(condition[2]);
            BooleanType *boo = new BooleanType(l, r, condition[1]);
            // condition is true
            if (boo->calculate() == 1) {
                blockParser(parameters, true, client_socket);
            }
            // The function as a while condition
        } else if (parameters[1] == "while") {
            bool run = true;
            while (run) {
                Interpreter *i = new Interpreter(game_configuration);
                string cond = parameters[2];
                vector<string> condition = helpLexer(cond);
                // left part of the condition
                Expression *l = i->interpret(condition[0]);
                // right part of the condition
                Expression *r = i->interpret(condition[2]);
                BooleanType *boo = new BooleanType(l, r, condition[1]);
                // condition is true
                if (boo->calculate() == 1) {
                    blockParser(parameters, true, client_socket);
                } else {
                    // condition is false
                    run = false;
                }
            }
        }
    }
    return "";
}

// initializing new function
void FuncCommand::initFunc(vector<string> parameters) {
    // declare function
    string name = parameters[1];
    vector<string> block;
    // create block of function
    for (int j = 2; j < parameters.size(); j++) {
        block.push_back(parameters[j]);
    }
    // create new func
    auto *f = new FuncCommand(block);
    // add new func to map
    func_map.emplace(make_pair(name, *f));
}

// execute a declared function
void FuncCommand::executeFunc(double var, int client_socket) {
    // find name of variable
    auto posVar = this->commands[0].find(" ");
    string variable = removeSpaces(this->commands[0].substr(posVar));
    Variable *v = new Variable(variable, var, false, "");
    // add local variable to map
    game_configuration.emplace(make_pair(variable, *v));
    // execute function
    blockParser(this->commands, false, client_socket);
    // remove local variable from map
    game_configuration.erase(game_configuration.find(variable));
}

// calls parser for a block
void blockParser(vector<string> parameters, bool ifOrWhile, int client_socket) {
    // remove variable from parameters
    parameters.erase(parameters.begin());
    if (ifOrWhile) {
        // remove condition and "if" or "while" from parameters
        parameters.erase(parameters.begin());
    }
    // run the commands in block until '}'
    int pos = 0;
    while (pos < parameters.size()) {
        // skip opening bracket
        if (parameters[pos] != "{") {
            // execute commands in block
            runExecute(parameters, client_socket);
            pos++;
        } else {
            parameters.erase(parameters.begin());
        }
    }
}
// sleep command
string SleepCommand::execute(vector<string> parameters) {
    Interpreter *i = new Interpreter(game_configuration);
    Expression *e = i->interpret(parameters[1]);
    double time = e->calculate();
    sleep(time);
    return "";
}

// print command
string PrintCommand::execute(vector<string> parameters) {
    // string to print
    if (parameters[1].at(0) == '"') {
        string str = parameters[1].substr(1);
        str.pop_back();
        cout << str << endl;
    } else {
        // expression to calculate and print result
        auto *i = new Interpreter(game_configuration);
        Expression *e = i->interpret(parameters[1]);
        double num = e->calculate();
        cout << num << endl;
    }
    return "";
}