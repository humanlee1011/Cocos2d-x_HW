#include "HelloWorldScene.h"
#include "Ball.h"
#include "MenuScene.h"
#include <string>
#pragma execution_character_set("utf-8")
USING_NS_CC;
Scene* HelloWorld::createScene()
{
	auto scene = Scene::createWithPhysics();                                  
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);   

	auto layer = HelloWorld::create();

	scene->addChild(layer);

	return scene;


}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
	this->hasEntered = false;
	this->isPunished = false;
	this->hasHit = false;
	this->hitCount = 1;
	this->p1Count = this->p2Count = 0;

	schedule(schedule_selector(HelloWorld::update), 0.01f, kRepeatForever, 0.0f);

	auto body = PhysicsBody::createEdgeBox(Size(visibleSize.width - 59, visibleSize.height - 144),
	PHYSICSBODY_MATERIAL_DEFAULT, 5.0f);                    
	auto edgeNode = Node::create();                                        
	edgeNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));              
	edgeNode->setPhysicsBody(body);  
	edgeNode->setName("edge");
	edgeNode->setTag(100);
	this->addChild(edgeNode);

	auto bg = Sprite::create("table.png");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	bg->setScale(1.3);
	this->addChild(bg, 0);

	char str[10];
	//sprintf(str, "%d", Factory::getInstance()->getSize());
	size = Label::createWithTTF("Welcome, Please touch the white ball and hold on to play", "fonts/arial.ttf", 24);
	size->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 25));
	this->addChild(size, 3);

	setTouchEnabled(true);                                             
	//设置为单点触摸  
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	putDownWhiteBall(Vec2(164, 240));

	start(20);


	return true;
}

void HelloWorld::putDownWhiteBall(Vec2 p) {
	whiteBall = Sprite::create("0.png");
	whiteBall->setName("0");
	whiteBall->setTag(0);
	whiteBall->setScale(0.45);

	auto ball = PhysicsBody::createCircle(whiteBall->getContentSize().width / 2, PhysicsMaterial(5.0f, 1.0f, 0.7f));
	ball->setGravityEnable(false);
	ball->setLinearDamping(0.5f);
	ball->setAngularDamping(0.5f);
	ball->setCategoryBitmask(1);
	ball->setCollisionBitmask(1);
	ball->setContactTestBitmask(1);

	whiteBall->setPhysicsBody(ball);
	whiteBall->setPosition(p);
	this->addChild(whiteBall);
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)                      
{
	if (whiteBall == NULL) {
		putDownWhiteBall(touch->getLocation());
		size->setString("");
		return false;
	}
	if (whiteBall->getBoundingBox().containsPoint(touch->getLocation())) {
		if (this->cue == nullptr) {
			return false;
		}
		hitBall = -1;
		hasHit = false;
		count = 1;
		schedule(schedule_selector(HelloWorld::updateCue), 0.5f, kRepeatForever, 0.0f);
		return true;
	}
	if (this->cue!=nullptr)
	{
		this->cue->removeFromParent();
		this->cue = nullptr;
	}
	Vec2 location = touch->getLocation();
	CCPoint whitelocation = whiteBall->getPosition();
	float nx = whitelocation.x - location.x;
	float ny = whitelocation.y - location.y;
	direction = Vec2(nx, ny).getNormalized();
	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("sprite_sheet.plist");
	this->cue = Sprite::createWithSpriteFrameName("cue.png");
	this->cue->setPosition(whitelocation+direction*(cue->getContentSize().width/2+50));
	direction = -direction;
	cue->setRotation(-direction.getAngle() * 180 / 3.1415926);
	this->addChild(cue, 1);
	if (hitCount == 0) {
		isP1 = !isP1;
		if (isPunished) {
			hitCount = 2;
		}
		else {
			hitCount = 1;
		}
	}
	if (isP1) {
		string str = "";
		if (hasEntered) {
			if (p1Balls == 1) {
				str = "(1--7)";
			}
			else {
				str = "(9--15)";
			}
		}
		size->setString("Now is P1 round." + str);
	}
	else {
		string str = "";
		if (hasEntered) {
			if (p1Balls == 1) {
				str = "(9--15)";
			}
			else {
				str = "(1--7)";
			}
		}
		size->setString("Now is P2 round." + str);
	}
	return false;
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event) {
	unschedule(schedule_selector(HelloWorld::updateCue));
	auto radius = cue->getContentSize().width * 0.25;
	auto offset = direction * (radius * 0.1 * count + 50 - whiteBall->getContentSize().width / 2);
	auto move = MoveBy::create(0.5f, offset);
	auto crash = CCCallFunc::create([this]() {
		this->force();
	});
	auto removeCue = CCCallFunc::create([this]() {
		this->cue->removeFromParent();
		this->cue = nullptr;
	});
	auto seq = Sequence::create(move, crash, removeCue, nullptr);
	cue->runAction(seq);
	hitCount--;
}



void HelloWorld::updateCue(float dt) {
	if (count == 10) {
		return;
	}
	auto size = cue->getContentSize();
	auto radius = size.width * 0.25;
	auto x = -direction.x * radius;
	auto y = -direction.y * radius;
	auto move = MoveBy::create(0.5f, Vec2(x * 0.1, y * 0.1));
	cue->runAction(move);
	count += 1;
}


void HelloWorld::force()                                
{
	hitBall = -1;
	if (whiteBall!=nullptr)
	{
		CCPoint location = whiteBall->getPosition();
		auto p = this->cue->getPosition();
		auto body = whiteBall->getPhysicsBody();
		body->setVelocity(Vec2(location.x - p.x, location.y - p.y).getNormalized() * 100 * count);
	}
}

void HelloWorld::update(float dt) 
{
	auto fac = Factory::getInstance();
	int index = fac->detect(Vec2(360, 404));
	if (index != -1) {
		int num = index;
		if (!hasEntered) {
			hasEntered = true;
			int num1;
			int num2;
			if (num < 8) {
				num1 = 1;
				num2 = 9;
			}
			else if (num > 8) {
				num1 = 9;
				num2 = 1;
			}
			else {

			}
			if (isP1) {
				p1Balls = num1;
			}
			else {
				p1Balls = num2;
			}
		}
		if (num == 8) {
			if (isP1) {
				if (p1Count < 7) {
					size->setString("P1 has lost.Congratulations!P2");
				}
				else {
					size->setString("P2 has lost.Congratulations!P1");
				}
			}
			else {
				if (p2Count < 7) {
					size->setString("P2 has lost.Congratulations!P1");
				}
				else {
					size->setString("P1 has lost.Congratulations!P2");
				}
			}
			MessageBox("The game is over, the game will return to menu in 3 seconds.", "Game Over");
			Sleep(3000);
			auto scene = MenuScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(255, 255, 255)));
		}
		if (isP1) {
			if ((p1Balls == 1 && num > 8) || (p1Balls == 9 && num < 8)) {
				p2Count++;
			}
			else {
				p1Count++;
				hitCount++;
			}
		}
		else {
			if ((p1Balls == 1 && num < 8) || (p1Balls == 9 && num > 8)) {
				p1Count++;
			}
			else {
				p2Count++;
				hitCount++;
			}
		}
	}
	whiteBallListener(Vec2(360, 404));

	/*CCPoint location = whiteBall->getPosition();
	char str[100];
	sprintf(str, "%lf %lf", abs(location.x), abs(location.y));
	size->setString(str);*/
}

void HelloWorld::start(int gap)
{
	this->isP1 = true;
	this->hitBall = -1;
	int arr[16] = { 0 };
	auto fac = Factory::getInstance();
	int num = 1;
	for (int i = 0; i < 5; i++)
	{
		int y = 240 + i * gap / 2;
		for (int j = 0; j <= i; j++)
		{
			if (num == 5)
			{
				this->addChild(fac->createBall(Vec2(500 + i * gap, y - j * gap), 8));
				arr[8] = 1;
				num++;
			}
			else
			{
				while (1)
				{
					int x = cocos2d::random(1, 15);
					if (arr[x] != 1 && x != 8)
					{
						this->addChild(fac->createBall(Vec2(500 + i * gap, y - j * gap), x));
						arr[x] = 1;
						num++;
						break;
					}
				}
			}
		}
	}

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

}

bool HelloWorld::onContactBegin(PhysicsContact & contact) {
	if (hasHit || !hasEntered) {
		return true;
	}
	auto a = contact.getShapeA()->getBody();
	auto b = contact.getShapeB()->getBody();
	auto sp1 = (Sprite*)a->getNode();
	auto sp2 = (Sprite*)b->getNode();
	if (sp1->getTag() != 0 && sp2->getTag() != 0) {
		return true;
	}
	if (sp2->getTag() == 100 || sp1->getTag() == 100) {
		return true;
	}
	hitBall = sp2->getTag();
	if (hitBall == 0) {
		hitBall = sp1->getTag();
	}
	hasHit = true;
	int index = hitBall;
	//size->setString("" + index);
	if (isP1) {
		if ((p1Balls == 1 && index >= 8) || (p1Balls == 9 && index <= 8)) {
			if (index == 8 && p1Count == 7) {
				return true;
			}
			isPunished = true;
			hitCount = 0;
			size->setString("Wrong hit!You are punished!Next is P2.");
			return true;
		}
	}
	else {
		if ((p1Balls == 1 && index <= 8) || (p1Balls == 9 && index >= 8)) {
			if (index == 8 && p2Count == 7) {
				return true;
			}
			isPunished = true;
			hitCount = 0;
			size->setString("Wrong hit!You are punished!Next is P1.");
			return true;
		}
	}
	if (hitCount == 0 && isPunished) {
		isPunished = false;
	}
}

void HelloWorld::whiteBallListener(Vec2 p)
{
	int w = p.x;
	int h = p.y;
	if (whiteBall != NULL)
	{
		if ((whiteBall)->getBoundingBox().containsPoint(p) || (whiteBall)->getBoundingBox().containsPoint(Vec2(w - 324, h)) || (whiteBall)->getBoundingBox().containsPoint(Vec2(w + 324, h))
			|| (whiteBall)->getBoundingBox().containsPoint(Vec2(w - 324, h - 328)) || (whiteBall)->getBoundingBox().containsPoint(Vec2(w, h - 328)) || (whiteBall)->getBoundingBox().containsPoint(Vec2(w + 324, h - 328)))
		{
			whiteBall->removeFromParent();
			whiteBall = NULL;
			size->setString("Click anywhere to put down the white ball.");
			if (isPunished) {
				isPunished = false;
				hitCount = 0;
			}
		}
	}
}