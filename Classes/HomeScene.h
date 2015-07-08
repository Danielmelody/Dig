//
// Created by huyiming on 15-5-11.
//

#ifndef MYGAME_HOMESCENE_H
#define MYGAME_HOMESCENE_H


#include "cocos2d.h"
#include "DataDefines.h"
#include "DigScene.h"
USING_NS_CC;



class HomeScene: public Scene {
public:
    bool init()override;
    void onEnter()override;
    CREATE_FUNC(HomeScene);
    void replaceScene(Ref *pRect);

private:
    bool isOut;
    Sprite *c1,*c2,*c3,*dig_logo;
};



#endif //MYGAME_HOMESCENE_H
