//
// Created by huyiming on 15-7-5.
//

#include "UserData.h"

UserData* UserData::getInstance(){
    static UserData userData;
    return userData;
}

void UserData::saveData(int score){
    string writeblePath = FileUtils::getInstance()->getWritablePath();

}