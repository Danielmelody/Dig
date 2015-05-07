//
// Created by huyiming on 15-5-7.
//

#include "RectMap.h"



bool RectMap::init() {
    if(!Node::init()) {
        return false;
    }
    return true;
}

void RectMap::beDigged(){
    if(!empty)hp--;
}

void RectMap::clear(){
    if(_sprite){
        this->removeChild(_sprite);
        hp = 0;
        empty = true;
    }
}



bool Brick::init() {
    if(!RectMap::init()){
        return false;
    }
    type = BRICK;
    hp = 1;
    _sprite = Sprite::create("Brick.jpg");
    _sprite->setScale(VISIZE.width/RECT_NUM_WIDTH/_sprite->getContentSize().width);
    this->addChild(_sprite);
    return true;
}

void Brick::beDigged() {
    RectMap::beDigged();
    if(hp == 0&&!empty){
        _sprite->setVisible(false);
        auto explo = ParticleExplosion::create();
        explo->setTexture(Director::getInstance()->getTextureCache()->addImage("Brick.jpg"));
        explo->setLife(0.15);
        explo->setLifeVar(0.15);
        explo->setTotalParticles(60);
        explo->setStartSize(VISIZE.width/RECT_NUM_WIDTH/5);
        explo->setStartSizeVar(VISIZE.width/RECT_NUM_WIDTH/2/5/2);
        explo->setSpeed(VISIZE.width/RECT_NUM_WIDTH*3);
        explo->setSpeedVar(explo->getSpeed());
        explo->setEndColor(Color4F(1,1,1,1));
        this->addChild(explo);
        explo->setGlobalZOrder(5);
        explo->setPosition(_sprite->getPosition());
        empty = true;
    }
}

bool Stone::init() {
    if(!RectMap::init()){
        return false;
    }
    type = STONE;
    hp = 3;
    _sprite = Sprite::create("Stone.jpg");
    _sprite->setScale(VISIZE.width/RECT_NUM_WIDTH/_sprite->getContentSize().width);
    this->addChild(_sprite);

    return true;
}

void Stone::beDigged() {
    RectMap::beDigged();

    NotificationCenter::getInstance()->postNotification("Crash!");
}



