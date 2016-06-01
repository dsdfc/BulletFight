#ifndef _LevelScene_H_
#define _LevelScene_H_

#include "cocos2d.h"
#include "StartLayer.h"
#include "FightLayer.h"

using namespace cocos2d;

class LevelLayer : public Layer
{
private:
	//屏幕的宽度 高度
	int m_screenWidth;
	int m_screenHeight;
	//当前成功的关卡数
	int m_successLevel = 1;
	//读取中文配置信息
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	void addTouchListener();

	void loadResource();

	// implement the "static create()" method manually
	CREATE_FUNC(LevelLayer);
};

#endif
