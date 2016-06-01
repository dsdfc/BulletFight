#ifndef _GameLostLayer_H_
#define _GameLostLayer_H_

#include "cocos2d.h"
#include "LevelLayer.h"

using namespace cocos2d;

class GameLostLayer : public Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameLostLayer);

private:
	Size winSize = Director::getInstance()->getWinSize();
};


#endif
