#ifndef _People_H_
#define _People_H_

#include "CommonUtils.h"
#include "cocos2d.h"
#include "Gun.h"

using namespace cocos2d;

//��ɫ �� ���ǡ�����
class People : public Sprite{
public:

	void fireBullet();  //�����ӵ�
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	bool canMoveUp();
	//virtual bool onContactBegin(const PhysicsContact& contact);
	void updateCustome();
	void onDieActor();
	void updatePos();
	Sprite* getBoxSprite();  //: ��ӻ�ȡ���ӵĺ���

	CREATE_FUNC(People);
	virtual bool init();

	void updatePeople(float delta);
	void updateMap(float delta);  // �������ǵ�λ�������ĵ�ͼ

	static bool isInRect(Point p, Vec4 rect);
	void fireBulletAnim();
	int getStandingtFloor();  // �ж��Ƿ������һ��ذ��� ��������ʱҪ���ض�Ӧ�Ĳ��� ���ϵ���
	void loadAnimActor();
	bool isStandingtFloor(Point pos);

	CC_SYNTHESIZE(ActorFace, m_faceDir, FaceDir);
	CC_SYNTHESIZE(int, m_horizontalSpeed, HorizontalSpeed);
	CC_SYNTHESIZE(int, m_upVerticalSpeed, UpVerticalSpeed);
	CC_SYNTHESIZE(int, m_downVerticalSpeed, DownVerticalSpeed);
	CC_SYNTHESIZE(int, m_HP, HP);

	Gun* m_bulletSprite;
	Sprite* m_firebulletAnim;

	Size winSize = Director::getInstance()->getWinSize();
};

#endif
