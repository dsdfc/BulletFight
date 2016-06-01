#ifndef _PreloadLayer_H_
#define _PreloadLayer_H_

#include "cocos2d.h"
#include "CommonUtils.h"

using namespace cocos2d;

class PreloadLayer : public Layer
{
private:
	//进度条
	cocos2d::ProgressTimer* m_progress;
	//进度条更新的次数
	float m_progressInterval;
	//游戏需要加载的资源总数
	int m_sourceCount;
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene(int level);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	//更新游戏进度条，计算何时加载完成
	void load(float _delta);
	// implement the "static create()" method manually
	CREATE_FUNC(PreloadLayer);
};

#endif
