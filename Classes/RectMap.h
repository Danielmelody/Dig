

#ifndef MYGAME_RECTMAP_H
#define MYGAME_RECTMAP_H

#include <cocos2d.h>
#include "DataDefines.h"
USING_NS_CC;


class RectMap: public Node {
public:
    virtual bool init();
    //Different Rect reflect differently when being digged
    virtual void beDigged();
    //make RectMap empty
    virtual  void clear();
    inline Sprite* getSprite(){ return _sprite;}
    //get type information
    inline int getType(){ return type;}

protected:
    Sprite* _sprite;
    int hp;
    int type;
    bool empty;
};

class Brick:public RectMap{
public:
    bool init()override;
    void beDigged()override;
    CREATE_FUNC(Brick);
};

class Stone:public RectMap{
public:
    bool init()override;
    void beDigged()override;
    CREATE_FUNC(Stone);
};


#endif //MYGAME_RECTMAP_H
