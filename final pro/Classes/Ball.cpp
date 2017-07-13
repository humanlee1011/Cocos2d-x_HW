#include"Ball.h"
USING_NS_CC;

Factory* Factory::factory = NULL;
Factory::Factory() 
{
	
}
Factory* Factory::getInstance() 
{
	if (factory == NULL) 
	{
		factory = new Factory();
	}
	return factory;
}


Sprite* Factory::createBall(Vec2 p)
{
	auto sp = Sprite::create("1.png");
	sp->setTag(1);
	sp->setScale(0.45);

	auto body = PhysicsBody::createCircle(sp->getContentSize().width / 2, PhysicsMaterial(5.0f, 1.0f, 0.7f));
	//auto body = PhysicsBody::createBox(sp->getContentSize());          
	body->setGravityEnable(false);
	body->setVelocity(Vec2(250, 250));
	body->setLinearDamping(0.3f);
	body->setAngularDamping(0.5f);
	sp->setPhysicsBody(body);
	sp->setPosition(p);

	Balls.pushBack(sp);
	return sp;
}

Sprite* Factory::createBall(Vec2 p, int num)
{
	char str[10];
	sprintf(str, "%d.png", num);
	auto sp = Sprite::create(str);
	//sp->setName("" + num);
	sp->setTag(num);
	sp->setScale(0.45);

	auto body = PhysicsBody::createCircle(sp->getContentSize().width / 2, PhysicsMaterial(5.0f, 1.0f, 0.7f));
	//auto body = PhysicsBody::createBox(sp->getContentSize());          
	body->setGravityEnable(false);
	//body->setVelocity(Vec2(250, 250));
	body->setLinearDamping(0.5f);
	body->setAngularDamping(0.5f);

	body->setCategoryBitmask(1);
	body->setCollisionBitmask(1);
	body->setContactTestBitmask(1);

	sp->setPhysicsBody(body);
	sp->setPosition(p);

	Balls.pushBack(sp);
	return sp;
}

Sprite* Factory::createBall(Sprite* ball)
{
	Balls.pushBack(ball);
	return ball;
}

void Factory::removeBall(Sprite* sp) 
{
	//erase()执行后会返回指向下一个元素的迭代器
	Sequence* seq = Sequence::create(CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sp)), NULL);
	sp->runAction(seq);
}

int Factory::detect(Vec2 p)
{
	int count = 0;
	int index = -1;
	int w = p.x;
	int h = p.y;
	cocos2d:Vector<Sprite*>::iterator it = Balls.begin();
	for (; it != Balls.end();)
	{
		if ((*it)->getBoundingBox().containsPoint(p) || (*it)->getBoundingBox().containsPoint(Vec2(w - 324, h)) || (*it)->getBoundingBox().containsPoint(Vec2(w + 324, h))
			|| (*it)->getBoundingBox().containsPoint(Vec2(w - 324, h - 328)) || (*it)->getBoundingBox().containsPoint(Vec2(w, h - 328)) || (*it)->getBoundingBox().containsPoint(Vec2(w + 324, h - 328)))
		{
			count++;
			if (count == 1) {
				index = (*it)->getTag();
			}
			removeBall(*it);
			it = Balls.erase(it);
		}
		else
		{
			it++;
			//do something 
		}
	}
	return index;
}

int Factory::getSize()
{
	return Balls.size();
}

void Factory::clear()
{
	Balls.clear();
}