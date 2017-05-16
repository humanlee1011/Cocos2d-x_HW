#include "HelloWorldScene.h"
#include "Monster.h"

#define database UserDefault::getInstance()
#pragma execution_character_set("utf-8")
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
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

	TMXTiledMap* tmx = TMXTiledMap::create("map.tmx");
	tmx->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	tmx->setAnchorPoint(Vec2(0.5, 0.5));
	tmx->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(tmx, 0);

	//创建一张贴图
	auto texture = Director::getInstance()->getTextureCache()->addImage("$lucia_2.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 113, 113)));
	//使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + visibleSize.width / 2,
							origin.y + visibleSize.height/2));
	addChild(player, 3);

	//hp条
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//使用hp条设置progressBar
	pT = ProgressTimer::create(sp);
	pT->setScaleX(90);
	pT->setAnchorPoint(Vec2(0, 0));
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0, 1));
	pT->setPercentage(100);
	pT->setPosition(Vec2(origin.x+14*pT->getContentSize().width,origin.y + visibleSize.height - 2*pT->getContentSize().height));
	addChild(pT,1);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
	addChild(sp0,0);

	// 静态动画
	idle.reserve(1);
	idle.pushBack(frame0);

	// 攻击动画
	attack.reserve(17);
	for (int i = 0; i < 17; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(113*i,0,113,113)));
		attack.pushBack(frame);
	}

	// 可以仿照攻击动画
	// 死亡动画(帧数：22帧，高：90，宽：79）
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");
    // Todo
	dead.reserve(22);
	for (int i = 0; i < 22; i++) {
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 0, 79, 90)));
		dead.pushBack(frame);
	}


	// 运动动画(帧数：8帧，高：101，宽：68）
	auto texture3 = Director::getInstance()->getTextureCache()->addImage("$lucia_forward.png");
    // Todo
	run.reserve(8);
	for (int i = 0; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i, 0, 68, 101)));
		run.pushBack(frame);
	}

	//创建WASD菜单
	Vector<MenuItem*> MenuItems;
	auto WLabel = Label::createWithTTF("W", "fonts/arial.ttf", 36);
	auto Wfont = MenuItemLabel::create(WLabel, CC_CALLBACK_1(HelloWorld::WFont_callback, this));
	Wfont->setPosition(Vec2(origin.x + 50, origin.y + 50));
	MenuItems.pushBack(Wfont);
	auto ALabel = Label::createWithTTF("A", "fonts/arial.ttf", 36);
	auto Afont = MenuItemLabel::create(ALabel, CC_CALLBACK_1(HelloWorld::AFont_callback, this));
	Afont->setPosition(Vec2(origin.x + 10, origin.y + 10));
	MenuItems.pushBack(Afont);
	auto SLabel = Label::createWithTTF("S", "fonts/arial.ttf", 36);
	auto Sfont = MenuItemLabel::create(SLabel, CC_CALLBACK_1(HelloWorld::SFont_callback, this));
	Sfont->setPosition(Vec2(origin.x + 50, origin.y + 10));
	MenuItems.pushBack(Sfont);
	auto DLabel = Label::createWithTTF("D", "fonts/arial.ttf", 36);
	auto Dfont = MenuItemLabel::create(DLabel, CC_CALLBACK_1(HelloWorld::DFont_callback, this));
	Dfont->setPosition(Vec2(origin.x + 100, origin.y + 10));
	MenuItems.pushBack(Dfont);
	auto WASDMenu = Menu::createWithArray(MenuItems);
	WASDMenu->setPosition(Vec2(origin.x + 10, origin.y + 10));
	this->addChild(WASDMenu, 1);

	createMonsterAndMove();

	//创建XY菜单
	Vector<MenuItem*> xyMenuItems;
	auto XLabel = Label::createWithTTF("X", "fonts/arial.ttf", 36);
	auto Xfont = MenuItemLabel::create(XLabel, CC_CALLBACK_1(HelloWorld::XFont_callback, this));
	Xfont->setPosition(Vec2(origin.x + 50, origin.y + 50));
	xyMenuItems.pushBack(Xfont);
	/*auto YLabel = Label::createWithTTF("Y", "fonts/arial.ttf", 36);
	auto Yfont = MenuItemLabel::create(YLabel, CC_CALLBACK_1(HelloWorld::YFont_callback, this));
	Yfont->setPosition(Vec2(origin.x + 10, origin.y + 10));
	xyMenuItems.pushBack(Yfont);*/
	auto XYMenu = Menu::createWithArray(xyMenuItems);
	XYMenu->setPosition(Vec2(origin.x + visibleSize.width - 80, origin.y + 10));
	this->addChild(XYMenu, 1);

	//从本地数据存取killNum
	if (!database->getBoolForKey("isExist")) 
		database->setBoolForKey("isExist", true);
	else
		killNum = database->getIntegerForKey("killNum");
	

	//initial time
	std::string numStr = StringUtils::toString(killNum);
	num = Label::createWithTTF(numStr, "fonts/arial.ttf", 36);
	num->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 3 / 4+ 100));
	this->addChild(num, 1);
	//调用scheduler
	schedule(schedule_selector(HelloWorld::updateCustom), 5.0f, kRepeatForever, 0);
	//调用默认scheduler
	scheduleUpdate();
	return true;
}

void HelloWorld::WFont_callback(Ref* pSender) {
	int moveY = 0;
	auto location = player->getPosition();
	if (location.y + 50 <= origin.y + visibleSize.height) {
		moveY = 50;
	}
	move(0, moveY);
	
}

void HelloWorld::AFont_callback(Ref* pSender) {
	int moveX = 0;
	auto location = player->getPosition();
	if (location.x - 30 >= origin.x) {
		moveX = -30;
	}
	if (lastCid != 'A') {
		player->setFlipX(true);
		lastCid = 'A';
	}
	move(moveX, 0);
}

void HelloWorld::SFont_callback(Ref* pSender) {
	int moveY = 0;
	auto location = player->getPosition();
	if (location.y - 30 >= origin.y) {
		moveY = -30;
	}
	move(0, moveY);
}

void HelloWorld::DFont_callback(Ref* pSender) {
	int moveX = 0;
	auto location = player->getPosition();
	if (location.x + 30 <= origin.x + visibleSize.width) {
		moveX = 30;
	}
	if (lastCid != 'D') {
		player->setFlipX(false);
		lastCid = 'D';
	}
	move(moveX, 0);	
}


void HelloWorld::XFont_callback(Ref* pSender) {
	Animation* animation = Animation::createWithSpriteFrames(attack, 0.1f);
	Animate* animate = Animate::create(animation);
	Animation* idleAnimation = Animation::createWithSpriteFrames(idle, 0.1f);
	Animate* originalState = Animate::create(idleAnimation);
	auto seq = Sequence::create(animate, originalState, nullptr);
	if (player->getNumberOfRunningActions() == 0) {
		player->runAction(seq);
		if (attackMonster()) {
			int percentage = pT->getPercentage();
			if (percentage <= 90)
				pT->setPercentage(percentage + 10);
			else if (percentage <= 100)
				pT->setPercentage(100);
			killNum++;
			database->setIntegerForKey("killNum", killNum);
			std::string numStr = StringUtils::toString(killNum);
			num->setString(numStr);
		}
	}
}

void HelloWorld::deadAnimation(Ref* pSender) {
	Animation* animation = Animation::createWithSpriteFrames(dead, 0.1f);
	Animate* animate = Animate::create(animation);
	Animation* idleAnimation = Animation::createWithSpriteFrames(idle, 0.1f);
	Animate* originalState = Animate::create(idleAnimation);
	auto seq = Sequence::create(animate, originalState, nullptr);
	player->runAction(seq);
	int percentage = pT->getPercentage();
	if (percentage >= 10)
		pT->setPercentage(percentage - 10);
	else if (percentage >= 0)
		pT->setPercentage(0);
	
}

void HelloWorld::move(int moveX, int moveY) {
	Animation* runAnimation = Animation::createWithSpriteFrames(run, 0.1f);
	Animate* animate = Animate::create(runAnimation);
	Animation* idleAnimation = Animation::createWithSpriteFrames(idle, 0.1f);
	Animate* originalState = Animate::create(idleAnimation);
	auto moveBy = MoveBy::create(0.5, Vec2(moveX, moveY));
	auto runSpawn = Spawn::createWithTwoActions(animate, moveBy);
	auto seq = Sequence::create(runSpawn, originalState, nullptr);
	if (player->getNumberOfRunningActions() == 0)
		player->runAction(seq);
}

void HelloWorld::updateCustom(float dt) {
	createMonsterAndMove();
//	if (dtime > 0) {
//		dtime--;
//		std::string timeStr = StringUtils::toString(dtime);
//		time->setString(timeStr);
//	}
//	else {
//		//Close the cocos2d-x game scene and quit the application
//		Director::getInstance()->end();
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//		exit(0);
//#endif
//	}
}

void HelloWorld::update(float dt) {
	hitByMonster(dt);
}

void HelloWorld::hitByMonster(float dt) {
	auto fac = Factory::getInstance();
	Sprite* collision = fac->collider(player->getBoundingBox());
	if (collision != NULL) {
		fac->removeMonster(collision);
		deadAnimation(this);
	}
}

bool HelloWorld::attackMonster() {
	Rect playerRect = player->getBoundingBox();
	Rect attackRect = Rect(playerRect.getMinX() - 40, playerRect.getMinY(),
		playerRect.getMaxX() - playerRect.getMinX() + 80, playerRect.getMaxY() - playerRect.getMinY());
	auto fac = Factory::getInstance();
	Sprite* collision = fac->collider(attackRect);
	if (collision != NULL) {
		fac->removeMonster(collision);
		return true;
	}
	else 
		return false;
}

void HelloWorld::createMonsterAndMove() {
	auto fac = Factory::getInstance();
	Sprite* m = fac->createMonster();
	m->setPosition(random(origin.x, visibleSize.width), random(origin.y, visibleSize.height));
	this->addChild(m, 3);
	fac->moveMonster(player->getPosition(), 1);
}