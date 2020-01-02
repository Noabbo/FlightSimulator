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
    c = new IfCommand();
    commands.emplace(make_pair("if", c));
    c = new WhileCommand();
    commands.emplace(make_pair("while", c));
}
// lexer - returns vector of strings extracted from text file
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
    } else if ((line.find("==") != string::npos) || (line.find("!=") != string::npos)
               || (line.find("<=") != string::npos) || (line.find(">=") != string::npos)
               || (line.find("<") != string::npos) || (line.find(">") != string::npos)) {
        // condition of while or if
        if (line.find("==") != string::npos) {
            auto opt = line.find("==");
            v.push_back(line.substr(0, opt));
            v.emplace_back("==");
            v.push_back(line.substr(opt + 2));
        }
        if (line.find("!=") != string::npos) {
            auto opt = line.find("!=");
            v.push_back(line.substr(0, opt));
            v.emplace_back("!=");
            v.push_back(line.substr(opt + 2));
        }
        if (line.find("<=") != string::npos) {
            auto opt = line.find("<=");
            v.push_back(line.substr(0, opt));
            v.emplace_back("<=");
            v.push_back(line.substr(opt + 2));
        }
        if (line.find(">=") != string::npos) {
            auto opt = line.find(">=");
            v.push_back(line.substr(0, opt));
            v.emplace_back(">=");
            v.push_back(line.substr(opt + 2));
        }
        if (line.find('<') != string::npos) {
            auto opt = line.find('<');
            v.push_back(line.substr(0, opt));
            v.emplace_back("<");
            v.push_back(line.substr(opt + 1));
        }
        if (line.find('>') != string::npos) {
            auto opt = line.find('>');
            v.push_back(line.substr(0, opt));
            v.emplace_back(">");
            v.push_back(line.substr(opt + 1));
        }
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
    for (int i = 0; i < s.size(); i++) {
        newString.push_back(::tolower(s[i]));
    }
    return newString;
}
// turns string's letters to all upper cases
string upperCase(string s) {
    string newString;
    for (int i = 0; i < s.size(); i++) {
        newString.push_back(::tolower(s[i]));
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
// parser - executes commands according to vector of string from lexer
void parser(vector<string> coms) {
    int i = 0;
    while (i < coms.size()) {
        Command *c = commands.find(coms[i])->second;
        if (c != nullptr) {

        } else {
            // command not in map - either funcCommand or variable

        }
    }
}
// command to open server that connects to the simulator
int OpenServerCommand::execute(vector<string> parameters) {
    Interpreter *i = new Interpreter(vars_map, vars);
    Expression *e = i->interpret(parameters[0]);
    double port = e->calculate();
    thread serverth(openServer, port);
    serverth.join();
    return OPEN_SERVER_COMMAND_RET_VALUE;
}
// function of server thread - blocking call
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
// command to open client in simulator
int ConnectCommand::execute(vector<string> parameters) {
    return 0;
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
// if command
int IfCommand::execute(vector<string> parameters) {
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
        // remove condition and "if" from parameters
        parameters.erase(parameters.begin());
        parameters.erase(parameters.begin());
        // run the commands in block until '}'
        int i = 0;
        while (parameters[i].compare("}") != 0) {

        }
    }
    return parameters.size();
}
// while command
int WhileCommand::execute(vector<string> parameters) {
    return parameters.size();
}
// func command - bonus!
int FuncCommand::execute(vector<string> parameters) {
    // search for function

    return parameters.size();
}
