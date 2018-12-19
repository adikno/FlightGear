//
// Created by michal on 12/17/18.
//
#include <string>
#include <string.h>
#include "Expression.h"
#include "ShuntingYard.h"

using namespace std;

class BooleanExpression: public Expression {
    Expression *first;
    Expression *second;
    string oper;
public:
    BooleanExpression(vector<string> &x) {
        if (x.at(x.size() - 1) == "{") {
            x.erase(x.begin() + x.size() - 1);
        }
        if (x.size() != 3) {
            throw "Illegal condition";
        }
        if (x.at(1) != ">" && x.at(1) != "<" && x.at(1) != ">=" && x.at(1) != "<=" && x.at(1) != "==" && x.at(1) != "!=") {
            throw "Illegal condition";
        }
        char data[x.at(0).size()];
        strcpy(data, x.at(0).data());
        ShuntingYard *shuntingYard = new ShuntingYard();
        queue<string> queue1 = shuntingYard->shuntingYard(data);
        first = shuntingYard->postfixEvaluate(queue1);
        data[x.at(2).size()];
        strcpy(data, x.at(2).data());
        queue1 = shuntingYard->shuntingYard(data);
        second = shuntingYard->postfixEvaluate(queue1);
        oper = x.at(1);
    }

    double calculate() {
        if (oper == "<") {
            return first->calculate() < second->calculate();
        }
        if (oper == ">") {
            return first->calculate() > second->calculate();
        }
        if (oper == "<=") {
            return first->calculate() <= second->calculate();
        }
        if (oper == ">=") {
            return first->calculate() >= second->calculate();
        }
        if (oper == "==") {
            return first->calculate() == second->calculate();
        }
        if (oper == "!=") {
            return first->calculate() != second->calculate();
        }
    }
};