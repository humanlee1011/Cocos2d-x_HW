#ifndef _MENU_SCENE_H
#define _MENU_SCENE_H

#include "HelloWorldScene.h"
#include "MenuScene.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
  // 'scene' is an autorelease object
  auto scene = Scene::create();

  // 'layer' is an autorelease object
  auto layer = MenuScene::create();

  // add layer as a child to scene
  scene->addChild(layer);

  // return the scene
  return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
  if (!Layer::init())
  {
    return false; }
  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  //add background
  auto bg = Sprite::create("MenuBackground.jpg");
  bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
  this->addChild(bg, 0);

  //add title text
  std::string billiard_str = "Billiard";
  auto title_text = Label::createWithTTF(billiard_str.c_str(), "fonts/AmaticSC-Bold.ttf", 130);
  title_text->setPosition(Vec2(visibleSize.width / 4 + origin.x, visibleSize.height / 2 + origin.y));
  this->addChild(title_text, 0);

  // add menu item start game
  std::string start_game_str = "Start Game";
  auto startgame_text = Label::createWithTTF(start_game_str.c_str(), "fonts/AmaticSC-Bold.ttf", 60);
  MenuItem* startgame = MenuItemLabel::create(startgame_text, CC_CALLBACK_1(MenuScene::startMenuCallback, this));
  startgame->setPosition(Vec2(visibleSize.width * 4 / 5 + origin.x, origin.y + 370));
  //add menu item game rules
  std::string game_rule_str = "Game Rule";
  auto gamerule_text = Label::createWithTTF(game_rule_str.c_str(), "fonts/AmaticSC-Bold.ttf", 60);
  MenuItem* gamerule = MenuItemLabel::create(gamerule_text, CC_CALLBACK_1(MenuScene::ruleMenuCallback, this));
  gamerule->setPosition(Vec2(visibleSize.width * 4 / 5 + origin.x, origin.y +270));
  //add menu item exit game
  std::string exit_str = "Exit Game";
  auto exit_text = Label::createWithTTF(exit_str.c_str(), "fonts/AmaticSC-Bold.ttf", 60);
  MenuItem* exitgame = MenuItemLabel::create(exit_text, CC_CALLBACK_1(MenuScene::exitMenuCallback, this));
  exitgame->setPosition(Vec2(visibleSize.width * 4 / 5 + origin.x, origin.y + 170));
  //create menu
  Menu* pMenu = Menu::create(startgame, gamerule, exitgame, NULL);
  pMenu->setPosition(Point::ZERO);
  this->addChild(pMenu, 1);
}

void MenuScene::startMenuCallback(Ref* sender)
{
  auto scene = HelloWorld::createScene();
  Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(255, 255, 255)));
}

void MenuScene::ruleMenuCallback(Ref* sender)
{
	MessageBox("Initially, each player select balls 1-7 or balls 9-15 to shoot and it will continue to shoot until committing a foul, or failing to legally pocket an object ball (whether intentionally or not).\n Once all of a player's or team's group of object balls are pocketed, they may attempt to sink the 8 ball.\nBut be careful, if the 8 ball falls into any pocket other than the one designated or is knocked off the table, the shooting player or team automatically loses.", "Game Rule");
}

void MenuScene::exitMenuCallback(Ref* sender)
{
  Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  exit(0)
#endif
}

#endif