#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;
class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	void WFont_callback(Ref* pSender);
	void AFont_callback(Ref* pSender);
	void SFont_callback(Ref* pSender);
	void DFont_callback(Ref* pSender);
	void XFont_callback(Ref* pSender);
	void YFont_callback(Ref* pSender);
	void move(int moveX, int moveY);
	// implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	void updateCustom(float dt);
private:
	cocos2d::Sprite* player;
	cocos2d::Vector<SpriteFrame*> attack;
	cocos2d::Vector<SpriteFrame*> dead;
	cocos2d::Vector<SpriteFrame*> run;
	cocos2d::Vector<SpriteFrame*> idle;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Label* time;
	int dtime;
	cocos2d::ProgressTimer* pT;
};

#endif // __HELLOWORLD_SCENE_H__
