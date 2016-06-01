#include "Enemy.h"

// ����״̬ �� ��������״̬Ҫ����
// ���Ƶз���λ��
void Enemy::updateEnemyPos(float delta)
{

	//AttackState::getInstance()->Execute(this);
	auto isBox = isBoxHere();

	if (!isBox) {
		updateAvoidDrop();
		updatePos();
	}
	else {
		updateCloseToBox();
	}
}


void Enemy::updateEnemy(float delta)
{
	if (getFaceDir() == F_RIGHT) {
		setFlippedX(true);
	}
	else {
		setFlippedX(false);
	}
	updatePeople(delta);
	onDieEnemy();
	updateFireBullet();
}

void Enemy::updateFaceDir()
{
	RelativeDirection rd = getActorRD();
	if (rd == RD_LEFT) {
		setFlippedX(true);
		m_faceDir = F_LEFT;
	}
	else if (rd == RD_RIGHT) {
		setFlippedX(false);
		m_faceDir = F_RIGHT;
	}
}

void Enemy::onDieEnemy()
{
	if (!isInRect(getPosition(), Vec4(0, 0, 1280, 960))) {
		SimpleAudioEngine::getInstance()->playEffect(die_fx_1);
		this->setPosition(Point(1280 / 2, 960 - 20));
		this->getPhysicsBody()->setVelocity(Point(0, 0));
		m_HP--;
	}
}

bool Enemy::init()
{
	if (!Sprite::init()) {
		return false;
	}

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Enemy::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	loadAnimEnemy();
	return true;
}

bool Enemy::onContactBegin(const PhysicsContact& contact)
{
	log("Enemy::onContactBegin");

	auto spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();

	//log("TagA: %d; TagB: %d", spriteA->getTag(), spriteB->getTag());
	//log("tagA.x = %f, tagA.y = %f, TagB.x = %f, TagB.y = %f", spriteA->getPosition().x, spriteA->getPosition().y,
	//	spriteB->getPosition().x, spriteB->getPosition().y);
	if (spriteA->getTag() == TAG_ENEMY_BULLET && spriteB->getTag() == TAG_ACTOR) {
		spriteA->removeFromParent();
		SimpleAudioEngine::getInstance()->playEffect(hit_fx_1);
	}
	else if (spriteB->getTag() == TAG_ENEMY_BULLET && spriteA->getTag() == TAG_ACTOR) {
		spriteB->removeFromParent();
		SimpleAudioEngine::getInstance()->playEffect(hit_fx_1);
	}

	return true;
}

void Enemy::updateCustome()
{
	log("Enemy::updateCustome");

	schedule(schedule_selector(Enemy::updateEnemy), 0.25f);
	schedule(schedule_selector(Enemy::updateEnemyPos), 1.0f); 
}


// ��ȡ���ǵ�ǰ��λ��
Point Enemy::getActorPos()
{
	Point p = getParent()->getChildByTag(TAG_ACTOR)->getPosition();
	return p;
}


// ���Ƶз�����һ��ذ��ƶ�
void Enemy::upFloor()
{
	log("Enemy::upFloor");
	//���Ȼ�ȡ��������ڵ��˵�λ��
	RelativeDirection rd = getActorRD();

	if (canUpSuccess(getPosition())) {  // ������������ƶ��������ƶ�
		log("Enemy::upFloor::moveUp");
		moveUp();
	} 
	//����������һ������Լ������
	else if (isAboveFloor(getPosition())){                     //��������������  // Ҫ�жϵ����Ƿ��ڵذ��ϲ��ܾ��������ƶ�
		if (!isInOneFloor()) { //����ͬһ��Ļ�
			auto floor = getCurUpperFloor(getPosition());
			RelativeDirection rd = getUpperFloorRD(floor);
			if (rd == RD_LEFT) {
				moveLeft();
			}
			else {
				moveRight();
			}
		}
	}
}

// �ж�������з��Ƿ���ͬһ���ذ�
bool Enemy::isInOneFloor()
{
	if (fabs(getActorPos().y - getPosition().y) < 20) {
		return true;
	}

	return false;
}

bool Enemy::isInLastFloor()
{
	return getStandingtFloor() == CommonUtil::getInstance()->getLastFloor();
}

// ���Ƶз�����һ��ذ��ƶ�
void Enemy::downFloor()
{
	log("Enemy::downFloor");
	//���Ȼ�ȡ��������ڵ��˵�λ��
	RelativeDirection rd = getActorRD();

	if (canDownSuccess(getPosition())) {
		moveDown();
	}
	else if (isAboveFloor(getPosition())){
		if (rd == RD_LEFT ) {
			if (!isInOneFloor()) {
				moveLeft();
			}
		}
		else if (rd == RD_RIGHT) {
			if (!isInOneFloor()) {
				moveRight();
			}
		}
	}
}

// ����������ʱ���ƶ�
void Enemy::upFloorForBox()
{
	log("Enemy::upFloorForBox");
	RelativeDirection rd = getBoxRD();

	if (canUpSuccess(getPosition())) {  // ������������ƶ��������ƶ�
		log("Enemy::upFloor::moveUp");
		moveUp();
	}
	else if (isAboveFloor(getPosition())){                     //��������������  // Ҫ�жϵ����Ƿ��ڵذ��ϲ��ܾ��������ƶ�
		/*	if (rd == RD_LEFT) {
				log("Enemy::upFloor::moveleft");
				moveLeft();
				}
				else if (rd == RD_RIGHT) {
				log("Enemy::upFloor::moveright");
				moveRight();
				}*/
		auto floor = getCurUpperFloor(getPosition());
		RelativeDirection rd = getUpperFloorRD(floor);
		if (rd == RD_LEFT) {
			moveLeft();
		}
		else {
			moveRight();
		}
	}
}

void Enemy::downFloorForBox()
{
	log("Enemy::downFloorForBox");
	RelativeDirection rd = getBoxRD();

	if (canDownSuccess(getPosition())) {
		moveDown();
	}
	else if (isAboveFloor(getPosition())){
		if (rd == RD_LEFT) {
			moveLeft();
		}
		else if (rd == RD_RIGHT) {
			moveRight();
		}
	}
}


// ��ȡ��������ڵ��˵����λ��
RelativeDirection Enemy::getBoxRD()
{
	RelativeDirection rd = RD_NONE;

	// ���ǵ�x���� ���� �з���x����
	if (getBoxSprite()->getPosition().x > getPosition().x) {
		rd = RD_RIGHT;
	}
	else if (getBoxSprite()->getPosition().x < getPosition().x) {
		rd = RD_LEFT;
	}

	return rd;
}

bool Enemy::isInGround(Sprite* sprite)
{
	for (auto x : CommonUtil::getInstance()->groundRect) {
		if (isInRect(sprite->getPosition(), x + Vec4(0, 0, 0, 15))) {
			log("isInRect");
			return true;
		}
	}

	return false;
}

// �Զ������ӵ�
void Enemy::updateFireBullet()
{
	log("Enemy::updateFireBullet");
	Point actorPos = getActorPos();
	Point currentPos = getPosition();

	Point dist = actorPos - currentPos;

	if (fabs(dist.y) < 12) {
		fireBullet();
	}
}

// �������
void Enemy::updateAvoidDrop()
{
	log("Enemy::updateAvoidDrop");

	auto currentPosX = this->getPosition().x;
	auto winX = Director::getInstance()->getWinSize().width;

	auto curMinX = getCurMinX();
	auto curMaxX = getCurMaxX();

	if (isInLastFloor()) {
		log("Enemy::updateAvoidDrop::isInLastFloor");
	}

	//���
	if (currentPosX < winX / 2) {
		if (currentPosX - getContentSize().width * 2 <= curMinX) {
			moveRight();
		}
	}
	else { //�ұ�
		if (currentPosX + getContentSize().width * 2 >= curMaxX) {
			moveRight();
		}
	}
}

//��ȡ��ǰ��������������
float Enemy::getCurMinX()
{
	float minX = CommonUtil::getInstance()->groundRect[0].x;
	for (auto x : CommonUtil::getInstance()->groundRect) {
		if (x.x < minX) {
			minX = x.x;
		}
	}

	return minX;
}

//��ȡ��ǰ��������������
float Enemy::getCurMaxX()
{
	float maxX = CommonUtil::getInstance()->groundRect[0].z;
	for (auto x : CommonUtil::getInstance()->groundRect) {
		if (x.z > maxX) {
			maxX = x.z;
		}
	}

	return maxX;
}

// ���µ��˵�λ��
void Enemy::updatePos()
{
	log("Enemy::updatePos");
	Point actorPos = getActorPos();
	Point enemyPos = getPosition();

	updateFaceDir();

	/*log("actorPos.y = %f", actorPos.y);
	log("enemyPos.y = %f", enemyPos.y);*/

	if (fabs(actorPos.y - enemyPos.y) < 20) {

	}
	
	// ���˵������� ���� ���ǵ�������
	if (fabs(actorPos.y - enemyPos.y) > 20 && enemyPos.y > actorPos.y) {
		log("enemy go down");
		downFloor();
	}
	// ���ǵ������� ���� ���˵�������
	else if (fabs(actorPos.y - enemyPos.y) > 20 && enemyPos.y < actorPos.y) {
		log("enemy go up");
		upFloor();
	}
}

// ��ȡ��ǰ��������ڵз��ķ���
RelativeDirection Enemy::getActorRD()
{
	RelativeDirection rd = RD_NONE;

	// ���ǵ�x���� ���� �з���x����
	if (getActorPos().x > getPosition().x) {
		rd = RD_RIGHT;
	}
	else if (getActorPos().x < getPosition().x) {
		rd = RD_LEFT;
	}

	return rd;
}

// �жϵ�ǰ�з��ܷ�ɹ�������Ծ
bool Enemy::canUpSuccess(Point pos)
{
	Point upBegin = Point(pos.x, pos.y + 10);
	Point upEnd = Point(pos.x, upBegin.y + getUpVerticalSpeed());

	for (auto x : CommonUtil::getInstance()->groundRect) {
		if (CommonUtil::intersect(upBegin, upEnd,
			Point(x.x, x.y), Point(x.z, x.w))) {
			return true;
		}
	}

	return false;
}

//�жϵ�ǰ�з��ܷ�ɹ���������
bool Enemy::canDownSuccess(Point pos)
{
	log("Enemy::canDownSuccess");

	for (auto x : CommonUtil::getInstance()->groundRect){
		if (CommonUtil::intersect(pos, Point(pos.x, 0), Point(x.x, x.y), Point(x.z, x.w))) {
			return true;
		}
	}
	//Point afterDown = pos - Point(0, getVerticalSpeed());

	//log("afterDown.x = %f, afterDown.y = %f", afterDown.x, afterDown.y);
	//// .y = 500
	//for (auto x : CommonUtil::getInstance()->groundRect) {
	//	if (isInRect(afterDown, x + Vec4(0, 0, 0, 30))) {
	//		return true;
	//	}
	//}
	
	return false;
}

bool Enemy::isAboveFloor(Point pos) // �жϵ�ǰλ���Ƿ��ڵذ���
{
	for (auto x : CommonUtil::getInstance()->groundRect) {
		if (isInRect(pos, x + Vec4(0, 0, 0, 20))) {
			return true;
		}
	}

	return false;
}

//���µ���Ϊ��ȡ���Ӷ����µ�λ��
void Enemy::updateCloseToBox() // ��������ʱ��������
{
	log("Enemy::updateCloseToBox");
	
	log("isBoxHere");
	auto boxPos = getBoxSprite()->getPosition();
	auto curPos = getPosition();

	log("boxpos.x = %f, boxpos.y = %f", boxPos.x, boxPos.y);
	log("curPos.x = %f, curpos.y = %f", curPos.x, curPos.y);

	auto dropPoints = getDropPointXInLast();

	if (isInLastFloor()) {
		if (getFaceDir() == F_RIGHT) {
			if (getPosition().x + 50 >= dropPoints[0]) {
				this->moveUp();
			}
		}
		else if (getFaceDir() == F_LEFT) {
			if (getPosition().x - 50 <= dropPoints[1]) {
				this->moveUp();
			}
		}
	}

	if (fabs(boxPos.y - curPos.y) < 20) {
		RelativeDirection rd = getBoxRD();
		if (rd == RD_LEFT) {
			moveLeft();
		}
		else if (rd == RD_RIGHT) {
			moveRight();
		}
	}
	else if (fabs(boxPos.y - curPos.y) > 20 && boxPos.y > curPos.y) {
		upFloorForBox();
	}
	else if (fabs(boxPos.y - curPos.y) > 20 && boxPos.y < curPos.y) {
		log("go up");
		downFloorForBox();
	}
}

bool Enemy::isBoxHere() // �ж������Ƿ��������Ļ
{
	auto size = Director::getInstance()->getWinSize();
	auto boxPos = getBoxSprite()->getPosition();
	
	log("box.x = %f, box.y = %f", boxPos.x, boxPos.y);

	if (isInRect(boxPos, Vec4(0, 0, size.width, size.height))) {
		return true;
	}

	return false;
}

//��ȡ��ǰ��������������һ��ذ�
Vec4 Enemy::getCurUpperFloor(Point pos)
{
	auto upperFloor = CommonUtil::getInstance()->groundRect[0];
	for (auto x : CommonUtil::getInstance()->groundRect) {
		if (x.y - pos.y > 0 && x.y < upperFloor.y) {
			upperFloor = x;
		}
	}

	return upperFloor;
}

RelativeDirection Enemy::getUpperFloorRD(Vec4 floor)
{
	RelativeDirection rd = RD_NONE;

	if (floor.x > getPosition().x) {
		rd = RD_RIGHT;
	}
	else {
		rd = RD_LEFT;
	}

	return rd;
}

// ��ȡ���һ��ĵ����
std::vector<float> Enemy::getDropPointXInLast()
{
	std::vector<Vec4> lastFloors;
	CommonUtil* instance = CommonUtil::getInstance();
	std::vector<int> indexVec;

	for (int i = 0; i < instance->groundFloorIndex.size(); i++) {
		if (instance->groundFloorIndex[i] == instance->getLastFloor()) {
			indexVec.push_back(i);
		}
	}

	for (auto x : indexVec) {
		lastFloors.push_back(instance->groundRect[x]);
	}

	std::vector<float> dropPoint;
	dropPoint.push_back(lastFloors[0].z);
	dropPoint.push_back(lastFloors[1].x);

	for (auto x : dropPoint) {
		log("drop = %f", x);
	}

	return dropPoint;
}

void Enemy::loadAnimEnemy()
{
	auto animation = Animation::create();

	for (int i = 1; i <= 4; i++) {
		std::string frameName = StringUtils::format("x%d.png", i);
		SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		animation->addSpriteFrame(spriteFrame);
	}

	animation->setDelayPerUnit(0.18f); //���ż��
	animation->setRestoreOriginalFrame(true);

	Animate* action = Animate::create(animation);
	runAction(RepeatForever::create(action));
}