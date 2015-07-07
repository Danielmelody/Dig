//
// Created by huyiming on 15-7-5.
//

#ifndef MYGAME_USERDATA_H
#define MYGAME_USERDATA_H

#include <vector>
#include <iostream>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

class UserData {
public:
    static UserData* getInstance();
    void saveData(int score);
    vector<int> getData();
    ~UserData(){};
private:
    UserData(){};
};


#endif //MYGAME_USERDATA_H
