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
    if(!empty){
        hp--;
        NotificationCenter::getInstance()->postNotification("changeEnergy",this);
    }
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
        explo->setLife(ACTION_INTERVAL);
        explo->setLifeVar(ACTION_INTERVAL);
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
}

bool Ore::init() {
    if(!Brick::init()){
        return false;
    }
    return true;
}

void Ore::clear() {
    RectMap::clear();
    _extra->setOpacity(0);
    type = BRICK;
}

void Ore::beDigged() {
    Brick::beDigged();
    this->removeChild(_extra);
    log("OreDig");
}

void Ore::setOreType(int type) {
    this->type = type;
    switch (type){
        case DIAMOND:
            _extra = Sprite::create("ex-rock.png");
            _extra->setScale(VISIZE.width/RECT_NUM_WIDTH/_extra->getContentSize().width);
            _extra->setPosition(_sprite->getPosition());
            this->addChild(_extra);
        }
}
