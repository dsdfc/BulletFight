#ifndef _Enemy_H_
#define _Enemy_H_

#include "People.h"


class Enemy : public People {
public:
	CREATE_FUNC(Enemy);
	void updateEnemy(float delta);
	void updateEnemyPos(float delta);
	virtual bool init();
	void updateCustome();
	virtual bool onContactBegin(const PhysicsContact& contact);

	
	Point getActorPos();
	bool isInGround(Sprite* sprite);
	void upFloor();
	void downFloor();
	RelativeDirection getActorRD();

	void updateFireBullet();
	void updateAvoidDrop();
	void updatePos();
	void updateCloseToBox();
	void updateFaceDir();
	void onDieEnemy();

	float getCurMinX();
	float getCurMaxX();

	bool canUpSuccess(Point pos);  
	bool canDownSuccess(Point pos);
	bool isAboveFloor(Point pos); 
	bool isBoxHere();

	void upFloorForBox();
	void downFloorForBox();
	RelativeDirection getBoxRD();
	bool isInOneFloor();
	bool isInLastFloor();

	Vec4 getCurUpperFloor(Point pos);
	RelativeDirection getUpperFloorRD(Vec4 floor);

	std::vector<float> getDropPointXInLast();

	void loadAnimEnemy();
};

#endif

