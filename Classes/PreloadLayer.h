#ifndef _PreloadLayer_H_
#define _PreloadLayer_H_

#include "cocos2d.h"
#include "CommonUtils.h"

using namespace cocos2d;

class PreloadLayer : public Layer
{
private:
	//������
	cocos2d::ProgressTimer* m_progress;
	//���������µĴ���
	float m_progressInterval;
	//��Ϸ��Ҫ���ص���Դ����
	int m_sourceCount;
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene(int level);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	//������Ϸ�������������ʱ�������
	void load(float _delta);
	// implement the "static create()" method manually
	CREATE_FUNC(PreloadLayer);
};

#endif
