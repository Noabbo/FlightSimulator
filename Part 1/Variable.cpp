//
// Created by noa on 19/11/2019.
//

#include <string>
#include "Variable.h"

// constructor
Variable::Variable(string n, double v, bool b, string s) {
    this->name.insert(0, n);
    this->value = v;
    this->updateSim = b;
    this->sim = s;
}

// calculate
double Variable::calculate() {
    return this->value;
}

bool Variable::getUpdateSim() {
    return this->updateSim;
}

string Variable::getSim() {
    return this->sim;
}

double Variable::getValue() {
    return this->value;
}

void Variable::setValue(double v) {
    this->value = v;
}

//operators
Variable& Variable::operator+=(const double v) const {
    Variable *newVar = new Variable(this->name, this->value + v, this->updateSim, this->sim);
    return *newVar;
}
Variable& Variable::operator-=(const double v) const {
    Variable *newVar = new Variable(this->name, this->value - v, this->updateSim, this->sim);
    return *newVar;
}
Variable& Variable::operator++() const {
    Variable *newVar = new Variable(this->name, this->value + 1, this->updateSim, this->sim);
    return *newVar;
}
Variable& Variable::operator--() const {
    Variable *newVar = new Variable(this->name, this->value - 1, this->updateSim, this->sim);
    return *newVar;
}
Variable& Variable::operator++(const int) const {
    Variable *newVar = new Variable(this->name, this->value + 1, this->updateSim, this->sim);
    return *newVar;
}
Variable& Variable::operator--(const int) const {
    Variable *newVar = new Variable(this->name, this->value - 1, this->updateSim, this->sim);
    return *newVar;
}

// destructor
Variable:: ~Variable() {}