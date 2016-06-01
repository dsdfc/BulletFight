#include "Box.h"
#include "Gun.h"

Point Box::getRandPosition()
{
	auto ground = CommonUtil::getInstance()->groundRect;
	for (auto x : ground) {
		log("(x.x = %f, x.w = %f), (x.z = %f, x.w = %f)", x.x, x.w, x.z, x.w);
	}

	int order = random(0, int(ground.size() - 1));
	cocos2d::Vec2 randPoint = Point(random(ground[order].x, ground[order].z), ground[order].w);
	return randPoint;
}

bool Box::init()
{
	if (!Sprite::init()) {
		return false;
	}

	//物理引擎碰撞检测回调
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Box::onContactBox, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	return true;
}

void Box::updateCustome()
{
	//todo: 实现每隔30秒显示一次，每隔10秒消失的功能

	schedule([&](float delta) {
		setVisible(false);
		setPosition(Point(2000, 2000));
		}, 15.0f, kRepeatForever, 0, "boxdisappear");

		schedule([&](float delta){
		setVisible(true);
		setPosition(getRandPosition());
		}, 20.0f, kRepeatForever, 0, "boxstart");
}

void Box::setPos()
{
	initWithFile("pic/782.png");
	Point pos = getRandPosition();
	setAnchorPoint(Point(0.5, 0));
	setPosition(Point(pos));


	auto boxBody = PhysicsBody::createBox(getContentSize(), PhysicsMaterial(5.0f, 0.5f, 0.5f));
	boxBody->setRotationEnable(false);
	boxBody->setContactTestBitmask(MASK_BOX);
	boxBody->setDynamic(false);  //使其不受力的影响

	this->setPhysicsBody(boxBody);
	setTag(TAG_BOX);

	updateCustome();
}


bool Box::onContactBox(const PhysicsContact& contact)
{

	log("FightLayer::onContactBegin");
	auto spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();

	/*int tag = spriteA->getTag();
	log("TagA: %d; TagB: %d", spriteA->getTag(), spriteB->getTag());
	log("tagA.x = %f, tagA.y = %f, TagB.x = %f, TagB.y = %f", spriteA->getPosition().x, spriteA->getPosition().y,
	spriteB->getPosition().x, spriteB->getPosition().y);*/

	auto rd = rand() % 5;
	std::string gunFileName = CommonUtil::getInstance()->m_gunsNames[rd] + ".png";
	log("gunFileName = %s", gunFileName.c_str());

	if (spriteA->getTag() == TAG_BOX && spriteA->isVisible() == true) {
			SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(gunFileName);
			((Gun *)(spriteB->getChildByName("gun")))->setSpriteFrame(frame);
			((Gun *)(spriteB->getChildByName("gun")))->
				setAttrByName(CommonUtil::getInstance()->m_gunsNames[rd]);
			spriteA->setPosition(Point(2000, 2000));
	}
	else if (spriteB->getTag() == TAG_BOX && spriteA->isVisible() == true) {
			SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(gunFileName);
			((Gun *)(spriteA->getChildByName("gun")))->setSpriteFrame(frame);
			((Gun *)(spriteB->getChildByName("gun")))->
				setAttrByName(CommonUtil::getInstance()->m_gunsNames[rd]);
			spriteB->setPosition(Point(2000, 2000));
	}

	return true;
}