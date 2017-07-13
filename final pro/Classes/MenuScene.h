#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_
#include "cocos2d.h"

class MenuScene:public cocos2d::Layer {
public:
  static cocos2d::Scene* createScene();
  virtual bool init();

  //callbacks start, rule and exit
  void startMenuCallback(cocos2d::Ref* sender);
  void ruleMenuCallback(cocos2d::Ref* sender);
  void exitMenuCallback(cocos2d::Ref* sender);
  CREATE_FUNC(MenuScene);
};



#endif
