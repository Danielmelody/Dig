//
// Created by huyiming on 15-5-7.
//

#include "Grap.h"

bool Grap::init() {
    if (!Node::init()) {
        return false;
    }
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
    _sprite->setPosition(lastTargetPos);
    _sprite->runAction(MoveBy::create(ACTION_INTERVAL,Point(Direction*VISIZE.width/RECT_NUM_WIDTH,0)));
    lastTargetPos += Point(Direction*VISIZE.width/RECT_NUM_WIDTH,0);
    log("horizen");
}