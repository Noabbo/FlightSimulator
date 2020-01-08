//
// Created by noa on 19/11/2019.
//

#include <string>
#include "Variable.h"

// constructor
Variable::Variable(string n, double v, bool b, string s) {
    this->name.insert(0, n);
    this->value = v;
    this->path = s;
    this->updateSimulator = b;
}

// calculate
double Variable::calculate() {
    return this->value;
}
// getters
bool Variable::getUpdateSimulator() {
    return this->updateSimulator;
}

string Variable::getPath() {
    return this->path;
}

double Variable::getValue() {
    return this->value;
}

string Variable::getName() {
    return this->name;
}
// setters
void Variable::setName(string n) {
    this->name = n;
}
void Variable::setValue(double v) {
    this->value = v;
}

void Variable::setPath(string s) {
    this->path = s;
}

void Variable::setUpdateSimulator(bool b) {
    this->updateSimulator = b;
}

//operators
Variable& Variable::operator+=(const double v) const {
    Variable *newVar = new Variable(this->name, this->value + v, this->updateSimulator,  this->path);
    return *newVar;
}
Variable& Variable::operator-=(const double v) const {
    Variable *newVar = new Variable(this->name, this->value - v, this->updateSimulator, this->path);
    return *newVar;
}
Variable& Variable::operator++() const {
    Variable *newVar = new Variable(this->name, this->value + 1, this->updateSimulator, this->path);
    return *newVar;
}
Variable& Variable::operator--() const {
    Variable *newVar = new Variable(this->name, this->value - 1,this->updateSimulator, this->path);
    return *newVar;
}
Variable& Variable::operator++(const int) const {
    Variable *newVar = new Variable(this->name, this->value + 1,this->updateSimulator, this->path);
    return *newVar;
}
Variable& Variable::operator--(const int) const {
    Variable *newVar = new Variable(this->name, this->value - 1,this->updateSimulator, this->path);
    return *newVar;
}

// destructor
Variable:: ~Variable() {}
