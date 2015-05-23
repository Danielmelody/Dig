

#ifndef MYGAME_RECTMAP_H
#define MYGAME_RECTMAP_H

#include <cocos2d.h>
#include "DataDefines.h"
#include "Audio.h"

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

class Ore:public Brick{
public:
    bool init()override;
    void beDigged()override;
    void clear()override;
    inline Sprite* getExtra(){ return _extra;}
    void setOreType(int type);
    CREATE_FUNC(Ore);
private:
    Sprite* _extra;
};

#endif //MYGAME_RECTMAP_H
