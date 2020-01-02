//
// Created by noa on 21/12/2019.
//

#include "server.h"
int main(int argc, char* argv[]) {
    // initialize map of commands
    init();
    // lexer
    vector<string> parameters = lexer(argv[1]);
    // parser
    parser(parameters);
    return 0;
}
// initialization of map of commands
void init() {
    Command *c = new OpenServerCommand();
    commands.emplace(make_pair("openDataServer", c));
    c = new ConnectCommand();
    commands.emplace(make_pair("connectControlClient", c));
    c = new DefineVarCommand();
    commands.emplace(make_pair("var", c));
    c = new PrintCommand();
    commands.emplace(make_pair("Print", c));
    c = new SleepCommand();
    commands.emplace(make_pair("Sleep", c));
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
            v.push_back(s);
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
    } else if (line.find("{") != string::npos) {
        auto curlPos = line.find("{");
        // while, if function or define new function
        if (line.find("if") != string::npos) {
            v.emplace_back("if");
            string condition = removeSpaces(line.substr(2, curlPos-2));
            v.push_back(condition);
        } else if (line.find("while") != string::npos) {
            v.emplace_back("while");
            string condition = removeSpaces(line.substr(5, curlPos-5));
            v.push_back(condition);
        } else {
            auto startPos = line.find("(");
            string name = removeSpaces(line.substr(0, startPos));
            v.push_back(name);
            auto endPos = line.find(")");
            string variable = removeSpaces(line.substr(startPos+1, endPos-startPos-1));
            v.push_back(variable);
        }
        v.emplace_back("{");
    } else {
        // update var
        if (line.find("=") != string::npos) {
            auto pos = line.find("=");
            string name = removeSpaces(line.substr(0, pos));
            v.push_back(name);
            string value = removeSpaces(line.substr(pos+1));
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
void parser(vector<string> coms) {
    int i = 0;
    vector<string> param;
    while (i < coms.size()) {
        // get command
        Command *c = commands[coms[i]];
        // finding parameters for commands
        if (coms[i] == "openDataServer") {
            param.push_back(coms[i+OPEN_SERVER_COMMAND_RET_VALUE]);
            i += c->execute(param);
        } else if (coms[i] == "connectControlClient") {
            param.push_back(coms[i+CONNECT_COMMAND_RET_VALUE]);
            i += c->execute(param);
        } else if (coms[i] == "var") {
            // undeclared variable
            if (coms[i+2] == "=") {
                for (int j = 0; j < 4; j++) {
                    param.push_back(coms[i+j]);
                }
            } else {
                for (int j = 0; j < 5; j++) {
                    param.push_back(coms[i+j]);
                }
            }
            i += c->execute(param);
        } else if ((coms[i] == "Print") || (coms[i] == "Sleep")) {
            param.push_back(coms[i+1]);
            i += c->execute(param);
        } else if (func_map.find(coms[i]) != func_map.end()) {
            // declared function
            // enter variable
            param.push_back(coms[++i]);
            i += c->execute(param);
        } else if (vars_map.find(coms[i]) != vars_map.end()) {
            // declared variable
            for (int j = 0; j < 3; j++) {
                param.push_back(coms[i+j]);
            }
            i += c->execute(param);
        } else if (coms[i+2] == "{") {
            // undeclared function
            param = findBlock(coms, i);
            i += c->execute(param);
        } else {
            throw "error - no matching command found";
        }
    }
}
// enters a block to a vector
vector<string> findBlock(vector<string> coms, int pos) {
    vector<string> param;
    // entering condition and block to vector
    while (coms[pos] != "}") {
        param.push_back(coms[++pos]);
    }
    // enter end of block
    param.emplace_back("}");
    return param;
}
// command to create and connect server
int OpenServerCommand::execute(vector<string> parameters) {
    int port = stoi(parameters[0]);
    thread serverth(openServer, port);
    serverth.join();
    return OPEN_SERVER_COMMAND_RET_VALUE;
}

//Create the server
void openServer(int port) {
    //Creation of the socket
    char buffer[255];
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
    int client_socket = accept(socketfd, (struct sockaddr *) &serverAddress, (socklen_t *) &serverAddress);
    if (client_socket == -1) {
        throw "Error- didn't accept client";
    }
    // The server keep listening to the client.
    while (true) {
        //Wait to listen from the client.
        recv(socketfd, buffer, sizeof(buffer), 0);
        if (buffer[0] == '\0') {
            cout << "waiting" << endl;
        }
            //The client succeed to connect.
        else {
            cout << "Connection server succeeded" << endl;
        }
    }
    close(socketfd);
}

//Create connection with the server
void connectClient(int port) {
    //Creation of the socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        throw "error - could not create a socket";
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(port);
    //Request a connection with the server on local host, try until the connection is made.
    while (true) {
        int is_connected = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
        if (is_connected == -1) {
            cout << "error - could not connected to host server" << endl;
            sleep(1);
        } else {
            cout << "Connection succeeded" << endl;
            break;
        }
    }
    //Close the socket whe the connection is done.
    close(client_socket);
}

void openDataServer() {
    xmlParser();
}
bool xmlParser() {
    tinyxml2::XMLDocument xml_doc;

    tinyxml2::XMLError eResult = xml_doc.LoadFile("/home/fanny/Documents/Advance conding/FlightSimulator-master..Noa upgrates.28.01/FlightSimulator-master/Part 1/generic_small.xml");
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
    typename list<Variable>::iterator>;
    while (eResult != EOF) {
        string name  = element->FirstChildElement("name")->GetText();
        string type  = element->FirstChildElement("type")->GetText();
        string format  = element->FirstChildElement("format")->GetText();
        string node  = element->FirstChildElement("node")->GetText();
        Variable *variable = new Variable(name, 0, false, node);
        vars.push_back(*variable);
        vars_map.emplace(make_pair(name, prev(vars.end())));
        sim_map.emplace(make_pair(node, prev(vars.end())));

        cout << name << type << format << node << endl;
        element = element->NextSiblingElement();
    }

}
// command that defines a new or existing variable
int DefineVarCommand::execute(vector<string> parameters) {
    string name;
    // define new variable
    if (parameters[0] == "var") {
        name = parameters[1];
        if (parameters[2] == "=") {
            // creating new local variable
            auto i = new Interpreter(vars_map, vars);
            Expression* e = i->interpret(parameters[3]);
            double value = e->calculate();
            auto *v = new Variable(name, value, false, "");
            // add new variable to list and map
            vars.push_back(*v);
            auto it = prev(vars.end());
            vars_map.emplace(make_pair(name, it));
        } else if (parameters[2] == "->") {
            // extracting the sum path
            string sim = parameters[4].substr(1);
            sim.pop_back();
            // new variable - create new one in list and map
            if (vars_map.find(name) == vars_map.end()) {
                auto *v = new Variable(name, 0, true, sim);
                // add new variable to list and maps
                vars.push_back(*v);
                auto it = prev(vars.end());
                vars_map.emplace(make_pair(name, it));
                sim_map.emplace(make_pair(sim, it));
            } else {
                // update boolean type and sim path in variable
                vars_map.at(name)->setUpdateSim(true);
                vars_map.at(name)->setSim(sim);
            }
        } else {
            // parameters[2] = "<-"
            // update path to simulator in variable
            string sim = parameters[4].substr(1);
            sim.pop_back();
            // new variable - create new one in list and map
            if (vars_map.find(name) == vars_map.end()) {
                auto *v = new Variable(name, 0, false, sim);
                // add new variable to list and map
                vars.push_back(*v);
                auto it = prev(vars.end());
                vars_map.emplace(make_pair(name, it));
                sim_map.emplace(make_pair(sim, it));
            } else {
                // update sim path in variable
                vars_map.at(name)->setSim(sim);
            }
        }
    } else {
        // update existing variable
        // extracting the name and new value of variable
        name = parameters[0];
        string exp = parameters[1];
        // calculating new value of variable
        auto i = new Interpreter(vars_map, vars);
        Expression* e = i->interpret(exp);
        double value = e->calculate();
        // set new value in map
        vars_map.at(name)->setValue(value);
        // simulator needs to be updated
        if (vars_map.at(name)->getUpdateSim()) {
            string updateSimulator = "set";
            updateSimulator += vars_map.at(name)->getSim();
            updateSimulator += " ";
            updateSimulator += to_string(vars_map.at(name)->getValue());
            updateSimulator += "\r\n";
            // send setting of variable to simulator
            // Fanny - please do that
        }
    }
    return parameters.size() + 1;
}

// constructor for func command
FuncCommand::FuncCommand(vector<string> c) {
    this->commands = c;
}
// func command - bonus!
int FuncCommand::execute(vector<string> parameters) {
    if (parameters[1] != "{") {
        auto f = func_map.at(parameters[0]);
        Interpreter *i = new Interpreter(vars_map, vars);
        Expression *e = i->interpret(parameters[1]);
        double var = e->calculate();
        f.executeFunc(parameters[0], var);
        // run function
        return 1;
    } else {
        // if or while
        if ((parameters[0] == "if") || (parameters[0] == "while")) {
            Interpreter *i = new Interpreter(vars_map, vars);
            string cond = parameters[1];
            vector<string> condition = helpLexer(cond);
            // left part of the condition
            Expression *l = i->interpret(condition[0]);
            // right part of the condition
            Expression *r = i->interpret(condition[2]);
            BooleanType *boo = new BooleanType(l, r, condition[1]);
            // condition is true
            if (boo->calculate() == 1) {
                blockParser(parameters, true);
            } else {
                // condition is false
                throw "condition is false";
            }
        } else {
            // declare function
            string name = parameters[0];
            vector<string> block;
            // create block of function
            for (int j = 1; j < parameters.size(); j++) {
                block.push_back(parameters[j]);
            }
            // create new func
            auto *f = new FuncCommand(block);
            // add new func to map
            func_map.emplace(make_pair(name, *f));
        }
    }
    return parameters.size() + 2;
}

void FuncCommand::executeFunc(string name, double var) {
    // change every declared variable in func to its value
    for (string s : this->commands) {
        while (s.find(name) != string::npos) {
            auto pos = s.find(name);
            s.replace(pos, name.size(), to_string(var));
        }
    }
    // execute function
    blockParser(this->commands, false);
}

// calls parser for a block
void blockParser(vector<string> parameters, bool ifOrWhile) {
    // remove variable from parameters
    parameters.erase(parameters.begin());
    if (ifOrWhile) {
        // remove condition and "if" or "while" from parameters
        parameters.erase(parameters.begin());
    }
    // run the commands in block until '}'
    int pos = 0;
    while (parameters[pos] != "}") {
        // skip opening bracket
        if (parameters[pos] != "{") {
            // execute commands in block
            parser(parameters);
        }
        pos++;
    }
}
// sleep command
int SleepCommand::execute(vector<string> parameters) {
    Interpreter *i = new Interpreter(vars_map, vars);
    Expression *e = i->interpret(parameters[0]);
    double num = e->calculate();
    Sleep(num);
    return 1;
}
// sleep
void Sleep(double number) {
    sleep(number);
}
// print command
int PrintCommand::execute(vector<string> parameters) {
    Print(parameters[0]);
    return 1;
}
// print
void Print(string str) {
    // string to print
    if (str.at(0) == '"') {
        cout << str << endl;
    } else {
        // expression to calculate and print result
        auto *i = new Interpreter(vars_map, vars);
        Expression *e = i->interpret(str);
        double num = e->calculate();
        cout << num << endl;
    }
}