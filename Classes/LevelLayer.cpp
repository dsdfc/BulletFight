#include "LevelLayer.h"
#include "LevelUtils.h"
#include "PreloadLayer.h"

Scene* LevelLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = LevelLayer::create();
	scene->addChild(layer);

	return scene;
}

bool LevelLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	loadResource();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_screenWidth = visibleSize.width;
	m_screenHeight = visibleSize.height;

	//����ͼƬ����
	auto bgSprite = Sprite::create("pic/levelSelect.png");
	bgSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bgSprite);

	//����ͼƬ
	auto back = Sprite::create("pic/back.png");
	back->setPosition(40, 40);
	back->setScale(0.5f);
	back->setTag(100);
	this->addChild(back);

	m_successLevel = LevelUtils::readLevelFromFile();

	//��ʾ8���ؿ�
	std::string imagepath = "";
	for (int i = 0; i < 8; i++) {
		//iС�ڶ����ؿ�����˵����ǰ�ؿ��Ѿ�ͨ��
		if (i < m_successLevel) {
			imagepath = "pic/level.png";
			//���뿨ͨ�ؿ�����
			std::string str = StringUtils::format("%d", i + 1);
			auto num = Label::createWithSystemFont(str, "Arial", 40, Size(70, 70), TextHAlignment::CENTER);
			// һ�ŷ�4���ؿ�ͼ�꣬����i % 4
			float x = 100 + i % 4 * 300;
			float y = m_screenHeight - 180 - i / 4 * 150;
			num->setPosition(x, y);
			this->addChild(num, 2);
		}
		else {  //�����Ĺؿ�ͼƬ
			imagepath = "pic/lock.png";
		}

		auto level = Sprite::create(imagepath);
		// ���ùؿ�����
		level->setTag(i + 1);
		// ����ÿһ���ؿ�ͼƬһ���ľ���
		float x = 100 + i % 4 * 300;
		float y = m_screenHeight - 180 - i / 4 * 150;
		level->setPosition(x, y);
		level->setScale(0.5f);
		this->addChild(level, 1);
	}

	addTouchListener();

	return true;
}

void LevelLayer::addTouchListener()
{
	// ��Ӵ�������
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event* event) {
		Vec2 touchLocation = touch->getLocation();
		//openGLת������Խڵ������
		Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);

		//ѭ���жϵ���������һ��ͼƬ�ؿ�ͼ��
		for (int i = 0; i < this->getChildrenCount(); i++) {
			//��ȡ��ȴ������ÿһ���ӽڵ�
			auto tempSprite = this->getChildren().at(i);
			if (tempSprite->getBoundingBox().containsPoint(nodeLocation)) {
				//�ж���ҵ�����Ƿ��� ���ǹؿ�ͼ��
				if (tempSprite->getTag() == 100) {
					//�л�����ʼ����
					auto transition = TransitionSplitRows::create(2.0f, StartLayer::createScene());
					Director::getInstance()->replaceScene(transition);
				}

				//�����ҵ�������Ѿ����ŵĹؿ�
				else if (tempSprite->getTag() > 0 &&
					tempSprite->getTag() < (m_successLevel + 1)) {
					// ɾ����ǰ�Ĵ����¼�����
					Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
					//������Ϸ������
					auto transtion = TransitionSplitRows::create(2.0f,
						PreloadLayer::createScene(tempSprite->getTag()));
					log("tempSprite Tag = %d", tempSprite->getTag());

					Director::getInstance()->replaceScene(transtion);
				}
				// �����ҵ��������δ�����Ĺؿ�ͼ�� ��ʾ��Ϣ����
				else if (tempSprite->getTag() != -1) {
					std::string msg = StringUtils::format("level %d is not opening", tempSprite->getTag());
					MessageBox(msg.c_str(), "��ʾ");
				}
			}
		}
		return true;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void LevelLayer::loadResource()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("guns/guns.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("people/people.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("firebullet/firebullet.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("blood/blood.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("pic/direction.plist");


	SimpleAudioEngine::getInstance()->preloadEffect(fire_fx_1);
	SimpleAudioEngine::getInstance()->preloadEffect(hit_fx_1);
	SimpleAudioEngine::getInstance()->preloadEffect(die_fx_1);
}
