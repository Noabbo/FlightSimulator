//
// Created by noa on 19/11/2019.
//

#ifndef UNTITLED_EXPRESSION_H
#define UNTITLED_EXPRESSION_H

using namespace std;
/**
 * Expression Interface
 */
class Expression {

public:
    virtual double calculate() = 0;
    virtual ~Expression() {};
};

#endif //UNTITLED_EXPRESSION_H
