//
// Created by michal on 12/16/18.
//

#include <cstring>
#include "Command.h"
#include "ShuntingYard.h"

class PrintCommand: public Command{

    int doCommand(vector<string> &x){
        for (int i = 0; i < x.size(); i++) {
            if (x.at(i)[0] == '\"') {
                cout << x.at(i).substr(1, x.at(i).length() - 2);
                continue;
            } else if (x.at(i) == "+") {
                continue;
            }
            char data[x.at(i).size()];
            strcpy(data, x.at(i).data());
            ShuntingYard *shuntingYard = new ShuntingYard();
            queue<string> queue1 = shuntingYard->shuntingYard(data);
            Expression *exp = shuntingYard->postfixEvaluate(queue1);
            cout << exp->calculate();
        }
        cout << endl;
    }


};