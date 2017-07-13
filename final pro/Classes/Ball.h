#pragma once
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;


class Factory:public cocos2d::Ref {
public:
	//获取单例工厂
	static Factory* getInstance();
	//生成一个球，并存储到容器中管理
	Sprite* createBall(Vec2 p);
	Sprite* createBall(Vec2 p, int num);
	Sprite* createBall(Sprite* ball);
	//移除球
	void removeBall(Sprite*);
	//检测是否进洞
	int detect(Vec2 p);

	int getSize();

	void clear();
private:
	Factory();
	Vector<Sprite*> Balls;
	static Factory* factory;
};