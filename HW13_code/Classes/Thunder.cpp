#include "Thunder.h"
#include <algorithm>

USING_NS_CC;

using namespace CocosDenshion;

Scene* Thunder::createScene() {
  // 'scene' is an autorelease object
  auto scene = Scene::create();

  // 'layer' is an autorelease object
  auto layer = Thunder::create();

  // add layer as a child to scene
  scene->addChild(layer);

  // return the scene
  return scene;
}

bool Thunder::init() {
  if (!Layer::init()) {
    return false;
  }

  isMove = false;  // �Ƿ����ɴ�
  visibleSize = Director::getInstance()->getVisibleSize();

  // ��������
  auto bgsprite = Sprite::create("bg.jpg");
  bgsprite->setPosition(visibleSize / 2);
  bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width,
                     visibleSize.height / bgsprite->getContentSize().height);
  this->addChild(bgsprite, 0);

  // �����ɴ�
  player = Sprite::create("player.png");
  player->setAnchorPoint(Vec2(0.5, 0.5));
  player->setPosition(visibleSize.width / 2, player->getContentSize().height);
  player->setName("player");
  this->addChild(player, 1);

  // ��ʾ��ʯ���ӵ�����
  enemysNum = Label::createWithTTF("enemys: 0", "fonts/arial.TTF", 20);
  enemysNum->setColor(Color3B(255, 255, 255));
  enemysNum->setPosition(50, 60);
  this->addChild(enemysNum, 3);
  bulletsNum = Label::createWithTTF("bullets: 0", "fonts/arial.TTF", 20);
  bulletsNum->setColor(Color3B(255, 255, 255));
  bulletsNum->setPosition(50, 30);
  this->addChild(bulletsNum, 3);

  addEnemy(5);        // ��ʼ����ʯ
  preloadMusic();     // Ԥ��������
  playBgm();          // ���ű�������
  explosion();        // ������ը֡����

  // ��Ӽ�����
  addTouchListener();
  addKeyboardListener();
  addCustomListener();

  // ������
  schedule(schedule_selector(Thunder::update), 0.04f, kRepeatForever, 0);

  return true;
}

//Ԥ���������ļ�
void Thunder::preloadMusic() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("music/bgm.mp3");
	audio->preloadEffect("music/explore.wav");
	audio->preloadEffect("music/fire.wav");
}

//���ű�������
void Thunder::playBgm() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("music/bgm.mp3", true);
}

//��ʼ����ʯ
void Thunder::addEnemy(int n) {
  enemys.clear();
  for (unsigned i = 0; i < 3; ++i) {
    char enemyPath[20];
    sprintf(enemyPath, "stone%d.png", 3 - i);
    double width = visibleSize.width / (n + 1.0),
           height = visibleSize.height - (50 * (i + 1));
    for (int j = 0; j < n; ++j) {
      auto enemy = Sprite::create(enemyPath);
      enemy->setAnchorPoint(Vec2(0.5, 0.5));
      enemy->setScale(0.5, 0.5);
      enemy->setPosition(width * (j + 1), height);
      enemys.push_back(enemy);
      addChild(enemy, 1);
    }
  }
}

// ��ʯ�����ƶ��������µ�һ��(�ӷ���)
void Thunder::newEnemy() {
	static int x = 0;
	for (auto enemyIt : enemys) {
		enemyIt->setPosition(Vec2(enemyIt->getPosition().x, enemyIt->getPosition().y - 50));
  }
	x = x++ % 3;
	char enemyPath[20];
	sprintf(enemyPath, "stone%d.png", x);
	double width = visibleSize.width / (5 + 1.0),
		height = visibleSize.height - 50;
	for (int j = 0; j < 5; ++j) {
		auto enemy = Sprite::create(enemyPath);
		enemy->setAnchorPoint(Vec2(0.5, 0.5));
		enemy->setScale(0.5, 0.5);
		enemy->setPosition(width * (j + 0.5), height);
		enemys.push_back(enemy);
		addChild(enemy, 1);
	}
}

// �ƶ��ɴ�
void Thunder::movePlane(char c) {
	int moveDis = 50;
	Vec2 location = player->getPosition();
	if (c == 'A') {
		if (location.x - 50 < 0)
			moveDis = -location.x;
		else
			moveDis = -50;
	}
	else if (c == 'D') {
		if (location.x + 50 > visibleSize.width)
			moveDis = visibleSize.width - location.x;
	}
	if (player->getNumberOfRunningActions() <= 0) {
		auto moveBy = MoveBy::create(1.0f, Vec2(moveDis, 0));
		player->runAction(moveBy);
	}
}

//�����ӵ�
void Thunder::fire() {
  auto bullet = Sprite::create("bullet.png");
  bullet->setAnchorPoint(Vec2(0.5, 0.5));
  bullets.push_back(bullet);
  bullet->setPosition(player->getPosition());
  addChild(bullet, 1);
  SimpleAudioEngine::getInstance()->playEffect("music/fire.wav", false);
  bullet->runAction(MoveBy::create(1.0f, Vec2(0, visibleSize.height)));
}

// �иը����֡
void Thunder::explosion() {
	auto texture = Director::getInstance()->getTextureCache()->addImage("explosion.png");
	explore.reserve(8);
	for (int i = 0; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(180 * i, 0, 180, 150)));
		explore.pushBack(frame);
	}
}

void Thunder::update(float f) {
  // ʵʱ����ҳ������ʯ���ӵ�����(����ɾ��)
  // Ҫ��������ʾ��ȷ(�ӷ���)
  char str[15];
  sprintf(str, "enemys: %d", enemys.size());
  enemysNum->setString(str);
  sprintf(str, "bullets: %d", bullets.size());
  bulletsNum->setString(str);

  // �ɴ��ƶ�
  if (isMove)
    this->movePlane(movekey);

  static int ct = 0;
  static int dir = 4;
  ++ct;
  if (ct == 120)
    ct = 40, dir = -dir;
  else if (ct == 80) {
    dir = -dir;
    newEnemy();  // ��ʯ�����ƶ��������µ�һ��(�ӷ���)
  }
  else if (ct == 20)
    ct = 40, dir = -dir;

  //��ʯ�����ƶ�
  for (Sprite* s : enemys) {
    if (s != NULL) {
      s->setPosition(s->getPosition() + Vec2(dir, 0));
    }
  }
  // �Ƴ��ɳ���Ļ����ӵ�
  list<Sprite*> toDel;
  for (auto bulletIt : bullets) {
	  if (bulletIt != NULL && bulletIt->getPosition().y > visibleSize.height) {
		  bulletIt->removeFromParentAndCleanup(true);
		  toDel.push_back(bulletIt);
	  }
  }
  for (auto bulletIt : toDel) {
	  bullets.remove(bulletIt);
  }
  // �ַ��Զ����¼�
  EventCustom e("meet");
  _eventDispatcher->dispatchEvent(&e);
}

// �Զ�����ײ�¼�
void Thunder::meet(EventCustom * event) {
	list<Sprite*> toDelE, toDelB;
  // �ж��ӵ��Ƿ������ʯ��ִ�ж�Ӧ����
	for (auto enemyIt : enemys) {
		for (auto bulletIt : bullets) {
			if (enemyIt != NULL && bulletIt != NULL && enemyIt->getPosition().getDistance(bulletIt->getPosition()) < 25) {
				enemyIt->runAction(
					Sequence::create(
						Animate::create(
							Animation::createWithSpriteFrames(explore, 0.05f, 1)
						),
						CallFunc::create([enemyIt, bulletIt] {
					enemyIt->removeFromParentAndCleanup(true);
				}),
						nullptr
					)
				);
				SimpleAudioEngine::getInstance()->playEffect("music/explore.wav");
				toDelE.push_back(enemyIt);
				toDelB.push_back(bulletIt);
			}
			break;
		}
	}
	for (auto delIt : toDelE) {
		enemys.remove(delIt);
	}
	for (auto delIt : toDelB) {
		bullets.remove(delIt);
		this->removeChild(delIt, true);
	}

  // �ж���Ϸ�Ƿ������ִ�ж�Ӧ����
	for (auto enemyIt : enemys) {
		if (enemyIt != NULL && enemyIt->getPosition().y < 0) {
			gameOver();
		}
  }
}

// ����Զ��������
void Thunder::addCustomListener() {
	auto meetListener = EventListenerCustom::create("meet", CC_CALLBACK_1(Thunder::meet, this));
	_eventDispatcher->addEventListenerWithFixedPriority(meetListener, 2);
}

// ��Ӽ����¼�������
void Thunder::addKeyboardListener() {
	auto keyBoardListener = EventListenerKeyboard::create();
	keyBoardListener->onKeyPressed = CC_CALLBACK_2(Thunder::onKeyPressed, this);
	keyBoardListener->onKeyReleased = CC_CALLBACK_2(Thunder::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, player);
}

void Thunder::gameOver() {
	//�Ƴ����м�����
	_eventDispatcher->removeAllEventListeners();
	player->runAction(Animate::create(Animation::createWithSpriteFrames(explore, 0.05f, 1)));
	SimpleAudioEngine::getInstance()->playEffect("music/explore.wav");
	auto gameOverLabel = Sprite::create("gameOver.png");
	gameOverLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(gameOverLabel, 3);
	unschedule(schedule_selector(Thunder::update));
}

void Thunder::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
  switch (code) {
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_CAPITAL_A:
  case EventKeyboard::KeyCode::KEY_A:
    movekey = 'A';
    isMove = true;
    break;
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_CAPITAL_D:
  case EventKeyboard::KeyCode::KEY_D:
    movekey = 'D';
    isMove = true;
    break;
  case EventKeyboard::KeyCode::KEY_SPACE:
    fire();
    break;
  }
}

void Thunder::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
  switch (code) {
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_A:
  case EventKeyboard::KeyCode::KEY_CAPITAL_A:
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_D:
  case EventKeyboard::KeyCode::KEY_CAPITAL_D:
    isMove = false;
    break;
  }
}

// ��Ӵ����¼�������
void Thunder::addTouchListener() {
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Thunder::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Thunder::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Thunder::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithFixedPriority(touchListener, 1);
}

// ����������ڵ�
bool Thunder::onTouchBegan(Touch *touch, Event *event) {
	fire();
	isClick = true;
	return true;
}

void Thunder::onTouchEnded(Touch *touch, Event *event) {
  isClick = false;
}

// ����갴ס�ɴ���ɿ��Ʒɴ��ƶ� (�ӷ���)
void Thunder::onTouchMoved(Touch *touch, Event *event) {
	Vec2 location = touch->getLocation();
	player->setPosition(location.x, player->getPosition().y);
}
