

#ifndef MYGAME_GRAP_H
#define MYGAME_GRAP_H


#include "cocos2d.h"
USING_NS_CC;

#include "DataDefines.h"


class Grap: public Node {
public:
    virtual bool init();
    //move and dig
    void Dig(int Direction);
    inline Sprite* getSprite(){ return _sprite;}
    CREATE_FUNC(Grap);
private:
    Point lastTargetPos;
    Sprite* _sprite;
};


#endif //MYGAME_GRAP_H
