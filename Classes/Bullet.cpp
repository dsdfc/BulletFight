#include "Bullet.h"
#include "CommonUtils.h"
#include "People.h"

void Bullet::update(float delta)
{
	log("Gun::update");
	
	Sprite* enemy = (Sprite*)(getParent()->getChildByTag(TAG_ENEMY));
	Sprite* actor = (Sprite *)(getParent()->getChildByTag(TAG_ACTOR));


	if (getTag() == TAG_ACTOR_BULLET) {        //�����ӵ���Ŀ��Ϊ����
		//��ȡ����sprite
		if (enemy->getBoundingBox().intersectsRect(getBoundingBox())) {

			if (enemy->getPosition().x < actor->getPosition().x) { //�������������
				enemy->runAction(MoveBy::create(0.5f, Vec2(-10, 0)));
			}
			else {
				enemy->runAction(MoveBy::create(0.5f, Vec2(10, 0)));
			}
			this->setVisible(false);
		}
		log("enemy.x = %f, enemy.y = %f", enemy->getPosition().x, enemy->getPosition().y);
	}
	else if (getTag() == TAG_ENEMY_BULLET) {   //�����ӵ���Ŀ��Ϊ����
		log("actor.x = %f, actor.y = %f", actor->getPosition().x, actor->getPosition().y);
		if (actor->getBoundingBox().intersectsRect(getBoundingBox())) {
			if (enemy->getPosition().x > actor->getPosition().x) {
				actor->runAction(MoveBy::create(0.5f, Vec2(-10, 0)));
			}
			else {
				actor->runAction(MoveBy::create(0.5f, Vec2(10, 0)));
			}
			this->setVisible(false);
		}
	}


	updateDisappear();

	auto size = CommonUtil::getInstance()->m_MapSize;

	if (!People::isInRect(getPosition(), Vec4(0, 0, size.width, size.height))){
		this->unschedule(schedule_selector(Bullet::update));
	}
}

void Bullet::updateDisappear()
{
	log("Gun::updateDisappear parent.x = %f, parent.y = %f", getParent()->getContentSize().width,
		getParent()->getContentSize().height);
}

void Bullet::updateCustome()
{
	log("Gun::updateCustome");
	schedule(schedule_selector(Bullet::update), 0.08f);
}

