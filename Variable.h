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
    string path;
    bool updateSimulator;
public:
    Variable(string n, double v,bool b, string s);
    double calculate();
    string getPath();
    double getValue();
    string getName();
    bool getUpdateSimulator();
    void setValue(double v);
    void setUpdateSimulator(bool b);
    void setPath(string s);
    Variable& operator+=(const double v) const;
    Variable& operator-=(const double v) const;
    Variable& operator++() const;
    Variable& operator--() const;
    Variable& operator++(const int v) const;
    Variable& operator--(const int v) const;
    virtual ~Variable();
};


#endif //EX1_VARIABLE_H
