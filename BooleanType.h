//
// Created by noa on 01/01/2020.
//

#ifndef PART_1_BOOLEANTYPE_H
#define PART_1_BOOLEANTYPE_H


#include <string>
#include "BinaryOperator.h"

class BooleanType : public BinaryOperator {
    string opt;
public:
    BooleanType(Expression *l, Expression *r, string o);
    Expression* getLeft();
    Expression* getRight();
    string getOpt();
    double calculate();
    bool operator==(const double) const;
    bool operator!=(const double) const;
    bool operator<=(const double) const;
    bool operator>=(const double) const;
    bool operator<(const double) const;
    bool operator>(const double) const;
    virtual ~BooleanType();
};


#endif //PART_1_BOOLEANTYPE_H
