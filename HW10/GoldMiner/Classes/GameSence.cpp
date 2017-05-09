#include "GameSence.h"

USING_NS_CC;

Scene* GameSence::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameSence::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameSence::init()
{

	if (!Layer::init())
	{
		return false;
	}

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameSence::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background = Sprite::create("level-background-0.jpg");
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(background, 0);

	mouse = Sprite::createWithSpriteFrameName("gem-mouse-0.png");
	Animate* mouseAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
	mouse->runAction(RepeatForever::create(mouseAnimation));
	mouse->setPosition(Vec2(visibleSize.width / 2 + origin.x, origin.y + visibleSize.height / 2));
	
	mouseLayer = LayerColor::create(Color4B(0, 0, 0, 0));
	mouseLayer->addChild(mouse);
	this->addChild(mouseLayer, 1);

	std::string shoot_str = "Shoot";
	auto shoot_text = LabelTTF::create(shoot_str.c_str(), "fonts/arial.ttf", 36);
	shoot_item = MenuItemLabel::create(shoot_text, CC_CALLBACK_1(GameSence::shootCallBack, this));
	Menu* pMenu = Menu::create(shoot_item, NULL);
	pMenu->setPosition(Vec2(visibleSize.width + origin.x - 100, visibleSize.height + origin.y - 150));
	this->addChild(pMenu, 2);

	stone = Sprite::create("stone.png");
	stone->setPosition(Vec2(origin.x + visibleSize.width / 2 + 100, origin.y + visibleSize.height / 2 + 150));
	stoneLayer = LayerColor::create(Color4B(0,0,0,0));
	stoneLayer->addChild(stone);
	this->addChild(stoneLayer, 1);

	return true;
}

bool GameSence::onTouchBegan(Touch *touch, Event *unused_event) {

	auto location = touch->getLocation();
	
	auto cheese = Sprite::create("cheese.png");
	cheese->setPosition(location);
	mouseLayer->addChild(cheese);
	auto fadeout = FadeOut::create(3.0f);
	cheese->runAction(fadeout);

	CCPoint point = mouse->convertToNodeSpace(location);
	point.x -= cheese->getContentSize().width / 2;
	point.y -= cheese->getContentSize().height / 2;
	auto moveBy = MoveBy::create(1, point);

	mouse->runAction(moveBy);
	return true;
}

void GameSence::shootCallBack(Ref* pSender) {
	//font become larger and then smaller
	auto scale1 = ScaleTo::create(0.5f, 2.0f);
	shoot_item->runAction(scale1);
	auto scale2 = ScaleTo::create(0.5f, 1.0f);
	shoot_item->runAction(scale2);
	//shoot stone to mouse position
	auto mouse_location = mouse->getPosition();
	
	auto shooting_stone = Sprite::create("stone.png");
	shooting_stone->setPosition(stone->getPosition());
	stoneLayer->addChild(shooting_stone);
	//move to mouse location
	CCPoint relative_mouse_location = stone->convertToNodeSpace(mouse_location);
	relative_mouse_location.x -= mouse->getContentSize().width / 2;
	relative_mouse_location.y -= mouse->getContentSize().height / 2;
	auto moveBy = MoveBy::create(0.5, relative_mouse_location);
	auto callbackMoveBy = CallFunc::create([]() {
	});
	//then fade out in 2sec
	auto fadeOut = FadeOut::create(0.5f);
	auto callbackFadeOut = CallFunc::create([]() {
	});
	auto seq = Sequence::create(moveBy, callbackMoveBy, fadeOut, callbackFadeOut, nullptr);
	shooting_stone->runAction(seq);
	//mouse leave a diamond and leave to random position
	auto diamond = Sprite::create("diamond.png");
	diamond->setPosition(mouse_location);
	mouseLayer->addChild(diamond);

	auto moveTo = MoveTo::create(0.5f, Vec2(rand() % 950, rand() % 450));
	mouse->runAction(moveTo);
}