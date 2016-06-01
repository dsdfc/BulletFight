#include "StartLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "FightLayer.h"
#include "LevelLayer.h"

using namespace cocos2d::ui;

Scene* StartLayer::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = StartLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool StartLayer::init()
{
	if (!Layer::init()) {
		return false;
	}

	auto spriteBg = Sprite::create("pic/background.jpg");
	spriteBg->setPosition(Point(winSize.width / 2, winSize.height / 2));
	this->addChild(spriteBg);


	auto bullet_label = Label::create("Bullets Fight", "font/Marker Felt", 72);
	bullet_label->setColor(Color3B::RED);
	bullet_label->setPosition(Point(winSize.width * 0.5, winSize.height * 0.7));
	this->addChild(bullet_label, 10);
	auto itemStartGame = MenuItemFont::create("Start Game", [&](Ref* pSender)
	{
		log("item Font CallBack");
		Director::getInstance()->replaceScene(LevelLayer::createScene());
	});
	itemStartGame->setFontName("font/Marker Felt");
	itemStartGame->setScale(2.0f);
	itemStartGame->setPosition(winSize.width / 2, winSize.height / 2);

	/*
	auto itemSetting = MenuItemFont::create("Setting", [&](Ref* pSender)
	{
		log("setting callBack");
		Director::getInstance()->replaceScene(GameWinLayer::createScene());
	});
	itemSetting->setFontName("font/Marker Felt");
	itemSetting->setScale(2.0f);
	itemSetting->setPosition(Point(winSize.width * 0.5, winSize.height * 0.25));
	*/

	auto menuSetting = Menu::create(itemStartGame, NULL);
	menuSetting->setPosition(Point::ZERO);
	this->addChild(menuSetting, 10);
	

	return true;
}

