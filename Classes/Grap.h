

#ifndef MYGAME_GRAP_H
#define MYGAME_GRAP_H


#include "cocos2d.h"
USING_NS_CC;
using namespace std;

#include "Audio.h"
#include "DataDefines.h"


class Grap: public Node {
public:
    virtual bool init();
    //move and dig
    void Dig(int Direction);
    inline void setDigType(int type){digType = type;}
    vector<vector<int>> getBrickEffect(int startLine,int startID,int type);
    inline Sprite* getSprite(){ return _sprite;}
    CREATE_FUNC(Grap);
private:
    Point lastTargetPos;
    Sprite* _sprite;
    int direction;
    int digType;
};


#endif //MYGAME_GRAP_H
