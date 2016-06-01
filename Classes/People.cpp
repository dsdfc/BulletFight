#include "People.h"

bool People::init()
{
	Sprite::init();

	loadAnimActor();
	return true;
}

// 发射子弹
void People::fireBullet()
{
	m_bulletSprite = Gun::create();
	m_bulletSprite->initWithFile("bullets/325.png");
	m_bulletSprite->setColor(Color3B::YELLOW);


	if (getTag() == TAG_ACTOR) {
		m_bulletSprite->setTag(TAG_ACTOR_BULLET);
	}
	else if (getTag() == TAG_ENEMY) {
		m_bulletSprite->setTag(TAG_ENEMY_BULLET);
	}

	Sprite* gun = (Sprite *)(getChildByTag(TAG_GUN));
	log("gun.position.x = %f y = %f", gun->getPosition().x, gun->getPosition().y);
	Point posBegin = getPosition();

	m_firebulletAnim = Sprite::create("firebullet/108.png");
	m_firebulletAnim->setAnchorPoint(Point(0, 0.5));
	auto physicsbody = PhysicsBody::createBox(m_firebulletAnim->getContentSize());

	if (m_faceDir == F_LEFT) {
		m_bulletSprite->setFlippedX(true);
		posBegin = posBegin + Point(-getContentSize().width - gun->getContentSize().width / 2, getContentSize().height / 2 + 10);
		m_bulletSprite->setPosition(Point(posBegin));
		m_firebulletAnim->setPosition(Point(-getContentSize().width-m_bulletSprite->getContentSize().width / 5, getContentSize().height / 2 + 10));
		m_firebulletAnim->setFlippedX(true);
		m_bulletSprite->runAction(MoveBy::create(3.0f, Vec2(-1500, 0)));
	}
	else { // 向右
		m_bulletSprite->setFlippedX(false);
		posBegin = posBegin + Point(getContentSize().width + gun->getContentSize().width / 2, getContentSize().height / 2 + 10);
		m_bulletSprite->setPosition(Point(posBegin));
		m_firebulletAnim->setPosition(Point(getContentSize().width + m_bulletSprite->getContentSize().width / 5, getContentSize().height / 2 + 10));
		m_firebulletAnim->setFlippedX(false);
		m_bulletSprite->runAction(MoveBy::create(3.0f, Vec2(1500, 0)));
	}
	addChild(m_firebulletAnim, LAYER_ACTOR);
	fireBulletAnim();
	getParent()->addChild(m_bulletSprite, LAYER_ACTOR);
	m_bulletSprite->updateCustome();
	SimpleAudioEngine::getInstance()->playEffect(fire_fx_1);
}


bool People::isInRect(Point p, Vec4 rect)
{
	if (p.x >= rect.x && p.x <= rect.z &&
		p.y >= rect.y && p.y <= rect.w) {
		return true;
	}
	return false;
}

// 只有踩在地板上才能进行跳跃
bool People::canMoveUp()
{
	if (isStandingtFloor(getPosition())) {
		return true;
	}
	return false;
}

void People::moveUp()
{
	if (canMoveUp()) {
		this->runAction(MoveBy::create(0.25f, Vec2(0, m_upVerticalSpeed)));
	}
}

void People::moveDown()
{
	log("People::moveDown");
	this->runAction(MoveBy::create(0.25f, Vec2(0, -m_downVerticalSpeed)));
}

void People::moveLeft()
{
	setFlippedX(true);
	m_faceDir = F_LEFT;
	this->runAction(MoveBy::create(0.25f, Vec2(-m_horizontalSpeed, 0)));
}

void People::moveRight()
{
	setFlippedX(false);
	m_faceDir = F_RIGHT;
	this->runAction(MoveBy::create(0.25f, Vec2(m_horizontalSpeed, 0)));
}

void People::updatePeople(float delta)
{
	if (getFaceDir() == F_RIGHT) {
		setFlippedX(false);
		((Sprite *)getChildByName("gun"))->setFlipX(false);
		((Sprite *)getChildByName("gun"))->setPosition(Point(getContentSize().width, getContentSize().height / 2));
	}
	else if (getFaceDir() == F_LEFT){
		setFlippedX(true);
		((Sprite *)getChildByName("gun"))->setFlipX(true);
		((Sprite *)getChildByName("gun"))->setPosition(Point(0, getContentSize().height / 2));
	}
	
	updatePos();
	getStandingtFloor();
}

void People::updateMap(float delta)
{
	Point actorPos = getPosition();
	auto &isInUp = CommonUtil::getInstance()->m_isInUpStage;
	auto mapSize = CommonUtil::getInstance()->m_MapSize;

	if (isInUp) {
		if (actorPos.y <= mapSize.height - winSize.height * 2 / 3) {
			getParent()->runAction(MoveBy::create(1.0f, Vec2(0, 240)));
			isInUp = false;
		}
	}
	else {
		if (actorPos.y >= winSize.height * 2 / 3) {
			getParent()->runAction(MoveBy::create(1.0f, Vec2(0, -240)));
			isInUp = true;
		}
	}
}

void People::updatePos()
{
	Point p = this->getPosition();
	for (auto x : CommonUtil::getInstance()->groundRect) {
		if (isInRect(p, x)) {
			log("in is in rect");
			if (fabs(p.y - x.y) <= 10) {
				this->runAction(MoveBy::create(0.1f, Point(0.0f, x.w - x.y + 20)));
			}
		}
	}

	log("people.x = %f, people.y = %f", this->getPosition().x, this->getPosition().y);

	onDieActor();
}


void People::updateCustome()
{
	log("People::updateCustome");
	schedule(schedule_selector(People::updatePeople), 0.25f);
	schedule(schedule_selector(People::updateMap), 0.25f);
}

void People::onDieActor()
{
	if (!isInRect(getPosition(), Vec4(0, 0, 1280, 960))) {
		SimpleAudioEngine::getInstance()->playEffect(die_fx_1);
		this->setPosition(Point(1280 / 2, 960 - 20));
		this->getPhysicsBody()->setVelocity(Point(0, 0));
		m_HP--;
	}
}

Sprite* People::getBoxSprite()
{
	Sprite* box = (Sprite *)(getParent()->getChildByTag(TAG_BOX));

	return box;
}

void People::loadAnimActor()
{
	auto animation = Animation::create();

	for (int i = 1; i <= 4; i++) {
		std::string frameName = StringUtils::format("y%d.png", i);
		SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		animation->addSpriteFrame(spriteFrame);
	}

	animation->setDelayPerUnit(0.18f); //播放间隔
	animation->setRestoreOriginalFrame(true);

	Animate* action = Animate::create(animation);
	runAction(RepeatForever::create(action));
}

bool People::isStandingtFloor(Point pos)
{
	for (auto x : CommonUtil::getInstance()->groundRect) {
		if (isInRect(pos, x + Vec4(0, 0, 0, 20))) {
			return true;
		}
	}

	return false;
}

void People::fireBulletAnim()
{
	auto animation = Animation::create();

	for (int i = 108; i <= 109; i++) {
		std::string frameName = StringUtils::format("%d.png", i);
		SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		animation->addSpriteFrame(spriteFrame);
	}

	animation->setDelayPerUnit(0.1f); //播放间隔
	animation->setRestoreOriginalFrame(false);

	Animate* action = Animate::create(animation);
	auto func = CallFuncN::create([&](Node *node){
		node->setVisible(false);
	});
	m_firebulletAnim->runAction(Sequence::create(action, func, NULL));
}

int People::getStandingtFloor()
{
	log("People::isInLastFloor");
	std::vector<Vec4> rects = CommonUtil::getInstance()->groundRect;
	auto floorIndexs = CommonUtil::getInstance()->groundFloorIndex;

	Vec4 standGround = rects[0]; //角色当前站立的地板
	int index = 0;        //地板对应的索引

	for (int i = 0; i < rects.size(); i++) {
		if (isInRect(getPosition(), rects[i] + Vec4(0, 0, 0, 20))) {
			standGround = rects[i];
			index = i;
		}
	}
	// 根据index获取层数

	//获取地板对应的序号
	log("standGround x = %f, y = %f, z = %f, w = %f", standGround.x, standGround.y, 
		standGround.z, standGround.w);
	return floorIndexs[index];
}
