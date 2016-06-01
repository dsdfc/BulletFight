#ifndef _Bullet_H_
#define _Bullet_H_

#include "cocos2d.h"
using namespace cocos2d;

class Bullet : public Sprite {

public:
	void fireBullet();

	CC_SYNTHESIZE(int, m_bulletNum, BulletNum);  //�ӵ�����
	CC_SYNTHESIZE(int, m_blowback, Blowback);   //������
	CC_SYNTHESIZE(int, m_impact, Impact);  //������
	CC_SYNTHESIZE(int, m_shootSpeed, ShootSpeed); //�����ٶ�
	CC_SYNTHESIZE(int, m_bulletSpeed, BulletSpeed); //�ӵ��ƶ��ٶ�

	void update(float delta);
	void updateDisappear();
	void updateCustome();

};

#endif
