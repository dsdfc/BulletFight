#ifndef _People_H_
#define _People_H_

#include "CommonUtils.h"
#include "cocos2d.h"
#include "Gun.h"

using namespace cocos2d;

//角色 ： 主角、敌人
class People : public Sprite{
public:

	void fireBullet();  //发射子弹
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	bool canMoveUp();
	//virtual bool onContactBegin(const PhysicsContact& contact);
	void updateCustome();
	void onDieActor();
	void updatePos();
	Sprite* getBoxSprite();  //: 添加获取箱子的函数

	CREATE_FUNC(People);
	virtual bool init();

	void updatePeople(float delta);
	void updateMap(float delta);  // 根据主角的位置来更改地图

	static bool isInRect(Point p, Vec4 rect);
	void fireBulletAnim();
	int getStandingtFloor();  // 判断是否在最后一层地板上 加载数据时要加载对应的层数 从上到下
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
