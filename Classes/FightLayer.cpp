#include "FightLayer.h"
#include "Box.h"
#include "LevelLayer.h"
#include "GameWinLayer.h"
#include "GameLostLayer.h"
#include "LevelUtils.h"

Scene* FightLayer::createScene()
{
	auto scene = Scene::createWithPhysics();  
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, -50.0f));
	auto layer = FightLayer::create();
	layer->setPhyWorld(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

cocos2d::Scene* FightLayer::createSceneWithLevel(int selectLevel)
{
	log("FightLayer::createSceneWithLevel = %d", selectLevel);
	log("CommonUtil::getInstance()->m_currentLevel = %d", CommonUtil::getInstance()->m_currentLevel);
	return createScene();
}

bool FightLayer::init()
{
	log("PlayerLayer::init");

	if (!Layer::init()) {
		return false;
	}
	// 创建物理边界
	//auto boxBody = PhysicsBody::createEdgeBox(winSize, PHYSICSBODY_MATERIAL_DEFAULT, 5.0f);
	//auto edgeNode = Node::create();
	//edgeNode->setPosition(winSize / 2);
	//edgeNode->setPhysicsBody(boxBody);
	//this->addChild(edgeNode, LAYER_CONTROL);

	initVar(); 

	CommonUtil::getInstance()->chooseLevel(CommonUtil::getInstance()->m_currentLevel);
	SimpleAudioEngine::getInstance()->playBackgroundMusic(bg_music_1, true);
	loadMap();
	//bgMap->runAction(MoveBy::create(0.05f, Vec2(0, -240)));

	loadPeople();
	loadDirection();
	loadFireBtn();
	loadHP();

	//第二个参数是中断时间间隔, 第三个是执行次数，第四个是延迟调用时间
	//this->schedule(schedule_selector(PlayerLayer::update), 0.1, kRepeatForever, 0);

	//物理引擎碰撞检测回调
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(FightLayer::onContactBox, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	auto sched = Director::getInstance()->getScheduler();
	sched->schedule(schedule_selector(FightLayer::updateCustome), this, 0.25f, false);
	sched->schedule(schedule_selector(FightLayer::winOrLost), this, 1.0f, false);
	
	m_box = Box::create();
	m_box->setPos();
	m_bgMap->addChild(m_box, LAYER_ACTOR);

	return true;
}

//成员变量初始化
void FightLayer::initVar()
{
	m_isEnemyMoveLeft = false;
	m_isPress = false;
	m_isPressDirLeft = false;
	m_isPressDirRight = false;
	buttonCount = 0;
}

void FightLayer::updateCustome(float delta)
{
	
	if (m_isPress) {
		log("btn is being press");
		m_actor->fireBullet();
	}
	else {
		log("btn is not being press");
	}
	if (m_isPressDirLeft) {
		m_actor->moveLeft();
	}
	if (m_isPressDirRight) {
		m_actor->moveRight();
	}
}


void FightLayer::winOrLost(float delta)
{
	auto actorHP = StringUtils::format("%d", m_actor->getHP());
	actorHP_label->setString("Actor Lives : " + actorHP);

	auto enemyHP = StringUtils::format("%d", m_enemy->getHP());
	enemyHP_label->setString("Enemy Lives : " + enemyHP);

	if (m_actor->getHP() == 0)
	{
		// lost
		CommonUtil::getInstance()->m_currentWinner = WINNER_ENEMY;
		auto transitions = TransitionFadeTR::create(0.4f, GameLostLayer::createScene());
		Director::getInstance()->replaceScene(transitions);

	}
	
	if (m_enemy->getHP() == 0)
	{
		// win
		CommonUtil::getInstance()->m_currentWinner = WINNER_ACTOR;
		auto transitions = TransitionFadeTR::create(0.4f, GameWinLayer::createScene());
		CommonUtil::getInstance()->m_currentLevel++;
		LevelUtils::writeLevelToFile(CommonUtil::getInstance()->m_currentLevel);
		Director::getInstance()->replaceScene(transitions);

	}
}

void FightLayer::onExit()
{
	log("onExit");
	this->unschedule(schedule_selector(FightLayer::update));
	//CC_SAFE_DELETE(commonUtil::getInstance()->_instance);
}

void FightLayer::loadMap() {

	CommonUtil::getInstance()->m_isInUpStage = true;
	std::string mapFile = StringUtils::format("maps/map%d.png", CommonUtil::getInstance()->m_currentLevel);
	log("FightLayer::loadMap::%s", mapFile.c_str());

	m_bgMap = Sprite::create(mapFile.c_str());
	m_bgMap->setPosition(Point(0, 0));
	m_bgMap->setAnchorPoint(Point(0, 0)); // 锚点设为了（0，0）
	this->addChild(m_bgMap, LAYER_BG);

	CommonUtil::getInstance()->chooseLevel(CommonUtil::getInstance()->m_currentLevel);
	// 测试地板区域
	for (auto x : CommonUtil::getInstance()->groundRect) {
		//x = x + Vec4(0,0,0,30);
		// todo：把物理矩形 的高度设为一半
		// 密度 弹性 摩擦力
		auto body = PhysicsBody::createEdgeBox(Size(x.z - x.x, x.w - x.y), PhysicsMaterial(0.0f, 0.0f, 0.0f));

		//body->setDynamic(false);  设置其不受力的作用
		auto edgeNode = Node::create();
		edgeNode->setAnchorPoint(Point(0.5, 0.5));
		edgeNode->setPosition(Point(x.x + (x.z - x.x) / 2, x.y + (x.w - x.y) / 2));
		edgeNode->setPhysicsBody(body);
		m_bgMap->addChild(edgeNode, 20);
	}

	m_bgMap->runAction(MoveBy::create(0.05f, Vec2(0, -240)));

	log("bgMap.x = %f, bgMap.y = %f", m_bgMap->getContentSize().width, m_bgMap->getContentSize().height);
	//loadBack();
	//addTouchListener();
}


void FightLayer::loadPeople() 
{
	int curLevel = CommonUtil::getInstance()->m_currentLevel;
	int maxRowDist = CommonUtil::getInstance()->m_rowDistData[curLevel].x;
	int minRowDist = CommonUtil::getInstance()->m_rowDistData[curLevel].y;

	// 敌人
	m_enemy = Enemy::create();
	m_enemy->initWithFile("people/y1.png");
	m_enemy->setTag(TAG_ENEMY);
	m_enemy->setAnchorPoint(Point(0.5, 0));
	m_enemy->setPosition(Point(300, 540));
	m_bgMap->addChild(m_enemy, LAYER_ACTOR);
	auto enemyBody = PhysicsBody::createBox(m_enemy->getContentSize(), PhysicsMaterial(10.0f, 0.0f, 0.0f));
	enemyBody->setContactTestBitmask(MASK_ENEMY);
	enemyBody->setCategoryBitmask(0x10);
	enemyBody->setCollisionBitmask(0x01);
	enemyBody->setRotationEnable(false);
	m_enemy->setPhysicsBody(enemyBody);
	m_enemy->setFaceDir(F_RIGHT);
	m_enemy->setHorizontalSpeed(40);
	m_enemy->setUpVerticalSpeed(maxRowDist);
	m_enemy->setDownVerticalSpeed(minRowDist);
	m_enemy->setHP(MAX_HP);
	m_enemy->updateCustome();
	auto gun = Sprite::create("guns/AMP.png");
	gun->setFlipX(false);
	gun->setPosition(Point(m_enemy->getContentSize().width, m_enemy->getContentSize().height / 2));
	gun->setName("gun");
	gun->setTag(TAG_GUN);
	m_enemy->addChild(gun, 10);

	m_actor = People::create();
	m_actor->initWithFile("people/x1.png");
	m_actor->setPosition(Point(900, 540));
	m_actor->setAnchorPoint(Point(0.5, 0));
	m_bgMap->addChild(m_actor, LAYER_ACTOR);
	auto actorBody = PhysicsBody::createBox(m_actor->getContentSize(), PhysicsMaterial(10.0f, 0.0f, 0.0f));
	actorBody->setRotationEnable(false);
	actorBody->setContactTestBitmask(MASK_ACTOR);
	actorBody->setCategoryBitmask(0x01);
	actorBody->setCategoryBitmask(0x10);
	m_actor->setPhysicsBody(actorBody);
	m_actor->setTag(TAG_ACTOR);
	m_actor->setFaceDir(F_LEFT);
	m_actor->setHorizontalSpeed(40);
	m_actor->setUpVerticalSpeed(maxRowDist);
	m_actor->setDownVerticalSpeed(minRowDist);
	m_actor->setHP(MAX_HP);
	m_actor->updateCustome();
	auto gun1 = Sprite::create("guns/AMP.png");
	gun1->setFlipX(true);
	gun1->setPosition(Point(0, m_actor->getContentSize().height / 2));
	gun1->setName("gun");
	gun1->setTag(TAG_GUN);
	m_actor->addChild(gun1, 10);
}

void FightLayer::loadDirection()
{
	auto itemUp = MenuItemImage::create("pic/3.png", "pic/11.png",
		[&](Ref* pSender){
		m_actor->moveUp();
	});
	itemUp->setPosition(Point(150, 160));

	auto itemDown = MenuItemImage::create("pic/3.png", "pic/11.png",
		[&](Ref* pSender){
		
		m_actor->moveDown();
	});
	itemDown->setRotation(180);
	itemDown->setPosition(Point(150, 42));

	auto menu = Menu::create(itemUp, itemDown, nullptr);
	menu->setPosition(Point::ZERO);
	addChild(menu, LAYER_CONTROL);

	auto itemLeft = Sprite::create("pic/3.png");
	itemLeft->setRotation(270);
	itemLeft->setPosition(Point(87, 102));
	addChild(itemLeft, LAYER_CONTROL);
	auto itemLeftListener = EventListenerTouchOneByOne::create();
	itemLeftListener->setSwallowTouches(true);
	itemLeftListener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
		auto target = (Sprite*)event->getCurrentTarget();
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode)) {
			m_isPressDirLeft = true;
			SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("11.png");
			target->setSpriteFrame(frame);
			return true;
		}
		return false;
	};
	itemLeftListener->onTouchEnded = [&](Touch* touch, Event* event) {
		auto target = (Sprite *)event->getCurrentTarget();
		SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("3.png");
		target->setSpriteFrame(frame);
		m_isPressDirLeft = false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(itemLeftListener, itemLeft);

	auto itemRight = Sprite::create("pic/3.png");
	itemRight->setRotation(90);
	itemRight->setPosition(Point(207, 102));
	addChild(itemRight, LAYER_CONTROL);
	auto itemRightListener = EventListenerTouchOneByOne::create();
	itemRightListener->setSwallowTouches(true);
	itemRightListener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
		auto target = (Sprite*)event->getCurrentTarget();
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode)) {
			m_isPressDirRight = true;
			SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("11.png");
			target->setSpriteFrame(frame);
			return true;
		}
		return false;
	};
	itemRightListener->onTouchEnded = [&](Touch* touch, Event* event) {
		auto target = (Sprite *)event->getCurrentTarget();
		SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("3.png");
		target->setSpriteFrame(frame);
		m_isPressDirRight = false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(itemRightListener, itemRight);
}

void FightLayer::loadFireBtn()
{
	auto fireSprite = Sprite::create("pic/fire.png");
	fireSprite->setPosition(Point(1086, 86));
	fireSprite->setTag(200);
	addChild(fireSprite, 15);

	auto fireSpriteListener = EventListenerTouchOneByOne::create();
	fireSpriteListener->setSwallowTouches(true);
	fireSpriteListener->onTouchBegan = [&](Touch* touch, Event* event)->bool {

		auto target = (Sprite *)event->getCurrentTarget();
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		// 点击范围判断检测
		if (rect.containsPoint(locationInNode)) {
			m_isPress = true;
			log("sprite began.. x = %f, y = %f", locationInNode.x, locationInNode.y);
			target->setOpacity(180);
			return true;
		}
		return false;

	};
	fireSpriteListener->onTouchEnded = [&](Touch* touch, Event* event) {
		auto target = (Sprite *)event->getCurrentTarget();
		target->setOpacity(255);
		m_isPress = false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(fireSpriteListener, fireSprite);
}


void FightLayer::loadBack()
{
	//返回图片
	auto back = Sprite::create("pic/back.png");
	back->setPosition(40, winSize.height - 40);
	back->setScale(0.5f);
	back->setTag(100);
	this->addChild(back);
}



void FightLayer::loadHP()
{
	enemyHP_label = Label::create("Enemy Lives : 5", "font/Marker Felt", 30);
	enemyHP_label->setPosition(Point(0 + enemyHP_label->getContentSize().width / 2, winSize.height - enemyHP_label->getContentSize().height / 2));
	enemyHP_label->setColor(Color3B::BLUE);
	this->addChild(enemyHP_label, LAYER_CONTROL);


	actorHP_label = Label::create("Actor Lives : 5", "font/Marker Felt", 30);
	actorHP_label->setPosition(Point(0 + enemyHP_label->getContentSize().width * 2, winSize.height - enemyHP_label->getContentSize().height / 2));
	actorHP_label->setColor(Color3B::BLUE);
	this->addChild(actorHP_label, LAYER_CONTROL);
}

bool FightLayer::onContactBox(const PhysicsContact& contact)
{
	log("FightLayer::onContactBegin");
	auto spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();

	///*int tag = spriteA->getTag();
	//log("TagA: %d; TagB: %d", spriteA->getTag(), spriteB->getTag());
	//log("tagA.x = %f, tagA.y = %f, TagB.x = %f, TagB.y = %f", spriteA->getPosition().x, spriteA->getPosition().y,
	//spriteB->getPosition().x, spriteB->getPosition().y);*/
	//if (spriteA->getTag() == BOX_TAG && spriteA->isVisible() == true) {
	//	if (spriteB->getTag() == ACTOR_TAG) {
	//		/*	spriteB->getChildByName("gun")->setVisible(false);*/
	//		spriteB->initWithFile("gun1/e1.png");
	//		/*spriteA->setPosition(Point(-1000, -1000));*/
	//	}
	//}
	//else if (spriteB->getTag() == BOX_TAG && spriteA->isVisible() == true) {
	//	if (spriteA->getTag() == ACTOR_TAG) {
	//		//spriteA->getChildByName("gun")->setVisible(false);
	//		spriteA->initWithFile("gun1/e1.png");
	//		//spriteB->setPosition(Point(-1000, -1000));
	//	}
	//}

	return true;
}

void FightLayer::addTouchListener()
{
	log("FightLayer::addTouchListener");
	// 添加触摸函数
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch *touch, Event* event) {
		Vec2 touchLocation = touch->getLocation();
		//openGL转换成相对节点的坐标
		Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);

		//循环判断单击的是哪一个图片关卡图标
		for (int i = 0; i < this->getChildrenCount(); i++) {
			log("this->getChildrenCount = %d", this->getChildrenCount());
			//获取但却场景的每一个子节点
			auto tempSprite = this->getChildren().at(i);
			if (tempSprite->getBoundingBox().containsPoint(nodeLocation)) {
				//判断玩家点击的是返回 还是关卡图标
				if (tempSprite->getTag() == 100) {
					//切换到开始界面
					auto transition = TransitionSplitRows::create(2.0f, LevelLayer::createScene());
					Director::getInstance()->replaceScene(transition);
				}
			}
			}
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

