#ifndef _Gun_H_
#define _Gun_H_

#include "cocos2d.h"
#include "CommonUtils.h"

using namespace cocos2d;

class Gun : public Sprite {
public:

	CREATE_FUNC(Gun);

	CC_SYNTHESIZE(int, m_bulletNum, BulletNum);  //子弹数量
	CC_SYNTHESIZE(int, m_backlash, Backlash);   //后坐力
	CC_SYNTHESIZE(int, m_impact, Impact);  //冲击力
	CC_SYNTHESIZE(int, m_shootSpeed, ShootSpeed); //射击速度
	CC_SYNTHESIZE(int, m_bulletSpeed, BulletSpeed); //子弹移动速度

	void update(float delta);
	void updateDisappear();
	void updateCustome();
	void bloodAnim();
	void setAttrByName(std::string gunName);

	Sprite* m_blood;
	Sprite* m_hit;
};


#endif
