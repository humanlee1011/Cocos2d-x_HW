#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;
class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event);

	bool onContactBegin(PhysicsContact & contact);

	void force();

	void update(float dt);

	void start(int gap);

	void updateCue(float dt);

	void whiteBallListener(Vec2 p);

	void putDownWhiteBall(Vec2 p);

	void setColor();
private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Label* size;
	cocos2d::Vec2 direction;
	int count;
	int hitBall;
	Sprite* whiteBall;
	Sprite* cue;
	bool isP1;
	int p1Balls;
	int p1Count;
	int p2Count;
	bool hasEntered;
	bool isPunished;
	int hitCount;
	bool hasHit;
};

#endif // __HELLOWORLD_SCENE_H__
