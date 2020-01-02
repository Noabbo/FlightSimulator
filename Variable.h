//
// Created by noa on 19/11/2019.
//

#ifndef EX1_VARIABLE_H
#define EX1_VARIABLE_H

#include <string>
#include <iostream>
#include "Expression.h"
class Variable : public Expression {
    string name;
    double value;
    bool updateSim;
    string sim;
public:
    Variable(string n, double v, bool b, string s);
    double calculate();
    bool getUpdateSim();
    string getSim();
    double getValue();
    void setValue(double v);
    void setUpdateSim(bool b);
    void setSim(string s);
    Variable& operator+=(const double v) const;
    Variable& operator-=(const double v) const;
    Variable& operator++() const;
    Variable& operator--() const;
    Variable& operator++(const int v) const;
    Variable& operator--(const int v) const;
    virtual ~Variable();
};


#endif //EX1_VARIABLE_H
