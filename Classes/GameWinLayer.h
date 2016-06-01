#ifndef _GameWinLayer_H_
#define _GameWinLayer_H_

#include "cocos2d.h"
#include "LevelLayer.h"

using namespace cocos2d;

class GameWinLayer : public Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(GameWinLayer);

private:
	Size winSize = Director::getInstance()->getWinSize();
	static bool isLost;
};


#endif
