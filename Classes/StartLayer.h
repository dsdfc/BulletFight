#ifndef _MainLayer_H_
#define _MainLayer_H_

#include "cocos2d.h"
using namespace cocos2d;

class StartLayer : public Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(StartLayer);

private:
	Size winSize = Director::getInstance()->getWinSize();
};

#endif