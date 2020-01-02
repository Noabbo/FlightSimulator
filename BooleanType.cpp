//
// Created by noa on 01/01/2020.
//

#include "BooleanType.h"
// constructor
BooleanType::BooleanType(Expression *l, Expression *r, string o) : BinaryOperator(l, r) {
    this->opt = o;
}

// getters
Expression* BooleanType::getLeft() {
    return this->left;
}
Expression* BooleanType::getRight() {
    return this->right;
}

string BooleanType::getOpt() {
    return this->opt;
}
// calculate
double BooleanType::calculate() {
    if (this->opt.compare("==") == 0) {
        // condition returned true
        if (this->operator==(0)) {
            return 1;
        }
    } else if (this->opt.compare("!=") == 0) {
        // condition returned true
        if (this->operator!=(0)) {
            return 1;
        }
    } else if (this->opt.compare("<=") == 0) {
        // condition returned true
        if (this->operator<=(0)) {
            return 1;
        }
    } else if (this->opt.compare(">=") == 0) {
        // condition returned true
        if (this->operator>=(0)) {
            return 1;
        }
    } else if (this->opt.compare("<") == 0) {
        // condition returned true
        if (this->operator<(0)) {
            return 1;
        }
    } else if (this->opt.compare(">") == 0) {
        // condition returned true
        if (this->operator>(0)) {
            return 1;
        }
    }
    // condition returned false
    return 0;
}
// defining operators
bool BooleanType::operator==(const double) const {
    return this->left->calculate() == this->right->calculate();
}

bool BooleanType::operator!=(const double) const {
    return this->left->calculate() != this->right->calculate();
}

bool BooleanType::operator<=(const double) const {
    return this->left->calculate() <= this->right->calculate();
}

bool BooleanType::operator>=(const double) const {
    return this->left->calculate() >= this->right->calculate();
}

bool BooleanType::operator<(const double) const {
    return this->left->calculate() < this->right->calculate();
}

bool BooleanType::operator>(const double) const {
    return this->left->calculate() > this->right->calculate();
}
// destructor
BooleanType::~BooleanType() {
    delete(this->left);
    delete(this->right);
}
