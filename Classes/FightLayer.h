#ifndef _PlayerLayer_H_
#define _PlayerLayer_H_

#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "CommonUtils.h"
#include "People.h"
#include "Enemy.h"
#include "Box.h"


using namespace cocos2d::ui;
using namespace CocosDenshion;
using namespace cocos2d;

class FightLayer : public Layer {
public:
  	People* m_actor; //����
	Enemy* m_enemy; // ����
	Sprite* m_bulletSprite; // �ӵ�

	bool m_isPress;
	bool m_isPressDirLeft;
	bool m_isPressDirRight;

	static Scene* createScene();
	static Scene* createSceneWithLevel(int selectLevel);
	virtual bool init();

	void initVar();

	CREATE_FUNC(FightLayer);

	void updateCustome(float delta);
	void winOrLost(float delta);
	virtual void onExit();
	void setLoadingAnimate();
	void initFightPhysics();    //��ʼ�����ǣ����ˣ��ӵ�
	void setPhyWorld(PhysicsWorld* world) { m_phyWorld = world; }
	PhysicsWorld* getPhyWorld() { return m_phyWorld; }

	void loadMap();  // ����ͼ�еľ�������
	void loadPeople();
	void loadDirection();
	void loadFireBtn();
	void loadBack();
	void loadHP();
	virtual bool onContactBox(const PhysicsContact& contact);
	void addTouchListener();
	void updateStart(float delta);
private:
	Size winSize = Director::getInstance()->getWinSize();
	PhysicsWorld* m_phyWorld;  // ��������
	Sprite* m_bgMap;
	bool m_isEnemyMoveLeft;
	Box* m_box;
	int buttonCount;

	Label* enemyHP_label;
	Label* actorHP_label;
};

#endif