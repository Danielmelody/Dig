
#ifndef MYGAME_CICLEACTION_H
#define MYGAME_CICLEACTION_H


#include "cocos2d.h"
USING_NS_CC;
#define RIGHT 1//Ë³Ê±Õë
#define LEFT -1//ÄæÊ±Õë
#define PI acos(-1)
class CircleAlltime :public Node{
public:
    bool init();
    inline void boundNode(Node* node){
        _node = node;
        node->addChild(this);
    }
    inline void setCircleCenter(Point center){ _center = center; }
    inline void setCircleCenterClobal(Point center){ _center = _node->convertToNodeSpace(center); }
    inline void setClockwise(int clockwise){ _clockwise = clockwise; }
    inline void setTime(float time){ _timeACircle = time; }
    void updatePos(float dt);
    static CircleAlltime* create(Node* node,float time, Point center, int clockwise){
        CircleAlltime *pRet = new CircleAlltime();
        pRet->boundNode(node);
        pRet->setTime(time);
        pRet->setCircleCenter(center);
        pRet->setClockwise(clockwise);
        if (pRet && pRet->init()){
            pRet->autorelease();
            return pRet;
        }
        else{
            delete pRet;
            pRet = NULL;
            return NULL;
        }
    }
private:
    float totalTime;
    float _timeACircle;
    int _clockwise;
    Point _center;
    Node* _node;
    float dis;
    float alpha;
    double unitAngle;
};


#endif //MYGAME_CICLEACTION_H
