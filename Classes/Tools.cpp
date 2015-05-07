#include "Tools.h"

bool CallbackTimeCounter::init()
{
	m_isCounting = false;
	this->scheduleUpdate();
	return true;
}

void CallbackTimeCounter::update(float dt)
{
	if (m_isCounting == false){
		return;
	}
	m_Time += dt;

	if (m_Time > m_CBTime){
		m_func();
		m_isCounting = false;
	}
}

void CallbackTimeCounter::start(float CBTime, std::function<void()> func)
{
	m_CBTime = CBTime;
	m_Time = 0;
	m_func = func;
	m_isCounting = true;
}

void CallbackTimeCounter::start()
{
	m_Time = 0;
	m_isCounting = true;
}

float CallbackTimeCounter::getTime()
{
	return m_Time;
}

bool CallbackTimeCounter::isCounting()
{
	return m_isCounting;
}

/****************************************************/
static RandomProducer* RandPro;

RandomProducer* RandomProducer::getInstance()
{
	if (RandPro == nullptr)
		RandPro = RandomProducer::create();
	return RandPro;
}

bool RandomProducer::init()
{
	timeval seed;
	gettimeofday(&seed, NULL);
	m_randseed = seed.tv_sec * 1000 + seed.tv_usec / 1000;
	srand(m_randseed);
	return true;
}

int RandomProducer::getRandom(int start, int end)
{
	if(start == end){
		return start;
	}
	return rand() % (end - start) + start + 1;
}