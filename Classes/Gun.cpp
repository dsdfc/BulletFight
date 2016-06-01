#include "Gun.h"
#include "People.h"

void Gun::update(float delta)
{
	log("Gun::update");

	Sprite* enemy = (Sprite*)(getParent()->getChildByTag(TAG_ENEMY));
	Sprite* actor = (Sprite *)(getParent()->getChildByTag(TAG_ACTOR));

	m_blood = Sprite::create("blood/72.png");
	m_hit = Sprite::create("pic/hit.png");

	if (getTag() == TAG_ACTOR_BULLET) {        //主角子弹，目标为敌人
		//获取敌人sprite
		if (enemy->getBoundingBox().intersectsRect(getBoundingBox())) {

			if (enemy->getPosition().x < actor->getPosition().x) { //敌人在主角左边
				enemy->runAction(MoveBy::create(0.5f, Vec2(-10, 0)));
				//m_blood->setPosition(Point(enemy->getContentSize().width, enemy->getContentSize().height / 2));
			}
			else {
				enemy->runAction(MoveBy::create(0.5f, Vec2(10, 0)));
				//m_blood->setPosition(Point(0, enemy->getContentSize().height / 2));
			}
			this->setVisible(false);
			//enemy->addChild(m_blood, LAYER_ACTOR);
			//m_hit->setPosition(enemy->getPosition() + Point(0, enemy->getContentSize().height * 1.25));
			//getParent()->addChild(m_hit, LAYER_CONTROL);
		}
		log("enemy.x = %f, enemy.y = %f", enemy->getPosition().x, enemy->getPosition().y);
	}
	else if (getTag() == TAG_ENEMY_BULLET) {   //敌人子弹，目标为主角
		log("actor.x = %f, actor.y = %f", actor->getPosition().x, actor->getPosition().y);
		if (actor->getBoundingBox().intersectsRect(getBoundingBox())) {
			if (enemy->getPosition().x > actor->getPosition().x) {
				actor->runAction(MoveBy::create(0.5f, Vec2(-10, 0)));
				//m_blood->setPosition(Point(actor->getContentSize().width, actor->getContentSize().height / 2));
			}
			else {
				actor->runAction(MoveBy::create(0.5f, Vec2(10, 0)));
				//m_blood->setPosition(Point(0, actor->getContentSize().height / 2));
			}
			this->setVisible(false);
			//actor->addChild(m_blood, LAYER_ACTOR);
			//m_hit->setPosition(actor->getPosition() + Point(0, actor->getContentSize().height * 1.25));
			//getParent()->addChild(m_hit, LAYER_CONTROL);
		}
	}

	//m_hit->runAction(ScaleTo::create(0.8f, 0.01));

	updateDisappear();
	//bloodAnim();
	auto size = Director::getInstance()->getWinSize();

	if (!People::isInRect(getPosition(), Vec4(0, 0, size.width, size.height))){
		this->unschedule(schedule_selector(Gun::update));
	}

}

void Gun::updateDisappear()
{
	log("Gun::updateDisappear parent.x = %f, parent.y = %f", getParent()->getContentSize().width, 
		getParent()->getContentSize().height);

	/*if (!getParent()->getBoundingBox().intersectsRect(getBoundingBox())) {
		removeFromParentAndCleanup(true);
	}*/

}

void Gun::updateCustome()
{
	log("Gun::updateCustome");
	schedule(schedule_selector(Gun::update), 0.08f);
}

void Gun::bloodAnim()
{
	auto animation = Animation::create();

	for (int i = 73; i <= 75; i++) {
		std::string frameName = StringUtils::format("%d.png", i);
		SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		animation->addSpriteFrame(spriteFrame);
	}

	animation->setDelayPerUnit(0.1f); //播放间隔
	animation->setRestoreOriginalFrame(false);

	Animate* action = Animate::create(animation);
	/*auto func = CallFuncN::create([&](Node *node){
		node->setVisible(false);
	});*/
}

void Gun::setAttrByName(std::string gunName)
{
	auto msg = CommonUtil::getInstance()->m_gunMsgs[gunName];

	for (auto x : msg) {
		log("fuck = %f", x);
	}

	log("----------------------------");

	//setBulletNum(msg[0]);
	//setBacklash(msg[1]);
	//setImpact(msg[2]);
	//setShootSpeed(msg[3]);
	//setBulletSpeed(msg[4]);
}


