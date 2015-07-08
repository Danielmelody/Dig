//
// Created by huyiming on 15-5-7.
//

#include "Grap.h"




bool Grap::init() {
    if (!Node::init()) {
        return false;
    }
    digType = BRICK;
    _sprite = Sprite::create("machine_basic.png");
    this->addChild(_sprite);
    _sprite->setScale(VISIZE.width/RECT_NUM_WIDTH/_sprite->getContentSize().width);
    log("grap init done");
    return true;
}

void Grap::Dig(int Direction) {
    /*
     * To avoid logic error when player operate too frequently,you must use lastTargetPos
     */
    //_sprite->setPosition(lastTargetPos);
    _sprite->runAction(MoveBy::create(ACTION_INTERVAL,Point(Direction*VISIZE.width/RECT_NUM_WIDTH,0)));
    SimpleAudioEngine::getInstance()->playEffect(DIG_AUDIO);
    //lastTargetPos += Point(Direction*VISIZE.width/RECT_NUM_WIDTH,0);
    log("horizen");
    log("xxx");
}



vector<vector<int>> Grap::getBrickEffect(int startLine, int startID,int type) {
    vector<vector<int>> points;
    switch (type)
    {
        case BRICK:
            points = {{startLine,startID}};
            break;
        case FIRE:
            points = {{startLine-1,startID-1},{startLine-1,startID},{startLine-1,startID+1},
                    {startLine,startID-1},{startLine,startID},{startLine,startID+1},
                      {startLine+1,startID-1},{startLine+1,startID},{startLine+1,startID+1}
            };
            break;
        default:
            points = {{startLine,startID}};
            break;

    }
    //digType = BRICK;
    return points;
}
