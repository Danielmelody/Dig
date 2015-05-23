//
// Created by huyiming on 15-5-7.
//

#include "Line.h"

bool Line::init() {
    if(!Node::init()){
        return false;
    }

    return true;
}

void Line::firstLine(){
    for(int t = 0 ; t<RECT_NUM_WIDTH; ++t){
        RectMap* rect = Brick::create();
        this->addChild(rect);
        rect->setPosition((0.5+(float)t)*VISIZE.width/RECT_NUM_WIDTH,0.5*VISIZE.width/RECT_NUM_WIDTH);
        rects.push_back(rect);
    }
}

void Line::up() {
    /*
     * To avoid logic error when player operate too frequently,you must use lastTargetPos
     */
    //this->setPosition(lastTargetPos);
    this->runAction(MoveBy::create(ACTION_INTERVAL,Point(0,VISIZE.width/RECT_NUM_WIDTH)));
    //lastTargetPos += Point(0,VISIZE.width/RECT_NUM_WIDTH);
}

void Line::initWithLastLine(Line *lastLine) {
    RandomProducer* randomProducer = RandomProducer::create();
    this->addChild(randomProducer);
    RectMap* newRect;
    for(int t = 0 ; t<RECT_NUM_WIDTH; ++t){

        int type = MAX(1,MIN(2,randomProducer->getRandom(BRICK-2,FIRE)));

        type = MAX(1,type);

        log("type:%d",type);
        switch (type){
            case BRICK: newRect =  Brick::create();break;
            case STONE: newRect = Stone::create();break;
            case DIAMOND: newRect = Ore::create();((Ore*)newRect)->setOreType(DIAMOND);break;
            case FIRE: newRect = Ore::create();((Ore*)newRect)->setOreType(FIRE);break;
            default: log("rect type error!");
        };

        this->addChild(newRect);
        newRect->setPosition((0.5+(float)t)*VISIZE.width/RECT_NUM_WIDTH,0.5*VISIZE.width/RECT_NUM_WIDTH);
        rects.push_back(newRect);
    }
    int t = 0;
    while(t < RECT_NUM_WIDTH) {
        log("loop1");
        int start = RECT_NUM_WIDTH - 1, end = 0;
        while (t< RECT_NUM_WIDTH&&lastLine->getRectByID(t)->getType() != STONE ) {
            start = MIN(t, start);
            end = MAX(t, end);
            t++;
        }
        if (start <= end && end < RECT_NUM_WIDTH) {
            int ID = MIN(end, MAX(start, randomProducer->getRandom(start, end)));
            convertToBrick(ID);
        }
        t++;
    }
    this->setPosition(lastLine->getPosition()+Point(0,-VISIZE.width/RECT_NUM_WIDTH));
    lastTargetPos = this->getPosition();
}

void Line::convertToBrick(int ID) {
    auto now = rects.begin();
    advance(now,ID);
    RectMap* last = (*now);
    (*now) = Brick::create();
    this->addChild(*now);
    this->removeChild(last);
    (*now)->setPosition(last->getPosition());
}
