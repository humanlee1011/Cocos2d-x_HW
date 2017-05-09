#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "tinyxml2/tinyxml2.h"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
	parseXML("Chinese.xml");
    
    auto label = Label::createWithTTF("15331169", "fonts/simhei.ttf", 24);
	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));
	//创建词典类实例，将xml文件加载到词典中
	auto *chnStrings = Dictionary::createWithContentsOfFile("Chinese.xml");
	//通过xml文件中的key获取value
	const char *str1 = ((String*)chnStrings->objectForKey("name"))->getCString();
	//创建一个标签显示出来
	auto* label1 = Label::createWithTTF(str1, "fonts/font.ttf", 24);
	label1->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label1->getContentSize().height * 2 - 20));
	
    

    // add the label as a child to this layer
    this->addChild(label, 1);
	this->addChild(label1,2);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("background.jpg");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}

void HelloWorld::parseXML(const char *fileName)
{

	std::string filePath = FileUtils::getInstance()->getWritablePath() + fileName;
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	tinyxml2::XMLError errorId = pDoc->LoadFile(filePath.c_str());

	if (errorId != 0) {
		//xml格式错误
		return;
	}

	tinyxml2::XMLElement *rootEle = pDoc->RootElement();

	//获取第一个节点属性
	const tinyxml2::XMLAttribute *attribute = rootEle->FirstAttribute();
	//打印节点属性名和值
	log("attribute_name = %s,attribute_value = %s", attribute->Name(), attribute->Value());

	tinyxml2::XMLElement *dicEle = rootEle->FirstChildElement("dic");
	tinyxml2::XMLElement *keyEle = dicEle->FirstChildElement("key");
	if (keyEle) {
		log("keyEle Text= %s", keyEle->GetText());
	}

	tinyxml2::XMLElement *arrayEle = keyEle->NextSiblingElement();
	tinyxml2::XMLElement *childEle = arrayEle->FirstChildElement();
	while (childEle) {
		log("childEle Text= %s", childEle->GetText());
		childEle = childEle->NextSiblingElement();
	}

	delete pDoc;

}
