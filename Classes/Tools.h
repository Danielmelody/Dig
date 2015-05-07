#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "cocos2d.h"
USING_NS_CC;

/*
计时器
简介：计时。
使用：start方法有两个重载，无参数的仅计时，有参数的传入 计时时间，回调函数 。
*/
class CallbackTimeCounter : public Node
{
public:
	CREATE_FUNC(CallbackTimeCounter);
	virtual bool init();
	virtual void update(float dt);
	void start(float CBtime, std::function<void()> func);//计时加回调
	void start();//仅计时
	float getTime();
	bool isCounting();
private:
	float m_Time;
	float m_CBTime;
	bool m_isCounting;

	std::function<void()> m_func;

};
/******************************************************************/

class RandomProducer : public Node
{
public:
	CREATE_FUNC(RandomProducer);
	static RandomProducer* getInstance();
	virtual bool init();
	int getRandom(int start, int end);
private:
	unsigned int m_randseed;
};


#endif