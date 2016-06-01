#ifndef _Gun_H_
#define _Gun_H_

#include "cocos2d.h"
#include "CommonUtils.h"

using namespace cocos2d;

class Gun : public Sprite {
public:

	CREATE_FUNC(Gun);

	CC_SYNTHESIZE(int, m_bulletNum, BulletNum);  //�ӵ�����
	CC_SYNTHESIZE(int, m_backlash, Backlash);   //������
	CC_SYNTHESIZE(int, m_impact, Impact);  //�����
	CC_SYNTHESIZE(int, m_shootSpeed, ShootSpeed); //����ٶ�
	CC_SYNTHESIZE(int, m_bulletSpeed, BulletSpeed); //�ӵ��ƶ��ٶ�

	void update(float delta);
	void updateDisappear();
	void updateCustome();
	void bloodAnim();
	void setAttrByName(std::string gunName);

	Sprite* m_blood;
	Sprite* m_hit;
};


#endif
