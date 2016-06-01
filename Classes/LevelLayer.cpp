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

	//背景图片精灵
	auto bgSprite = Sprite::create("pic/levelSelect.png");
	bgSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bgSprite);

	//返回图片
	auto back = Sprite::create("pic/back.png");
	back->setPosition(40, 40);
	back->setScale(0.5f);
	back->setTag(100);
	this->addChild(back);

	m_successLevel = LevelUtils::readLevelFromFile();

	//显示8个关卡
	std::string imagepath = "";
	for (int i = 0; i < 8; i++) {
		//i小于读到关卡数，说明当前关卡已经通关
		if (i < m_successLevel) {
			imagepath = "pic/level.png";
			//加入卡通关卡数字
			std::string str = StringUtils::format("%d", i + 1);
			auto num = Label::createWithSystemFont(str, "Arial", 40, Size(70, 70), TextHAlignment::CENTER);
			// 一排放4个关卡图标，所以i % 4
			float x = 100 + i % 4 * 300;
			float y = m_screenHeight - 180 - i / 4 * 150;
			num->setPosition(x, y);
			this->addChild(num, 2);
		}
		else {  //加锁的关卡图片
			imagepath = "pic/lock.png";
		}

		auto level = Sprite::create(imagepath);
		// 设置关卡精灵
		level->setTag(i + 1);
		// 设置每一个关卡图片一定的距离
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
	// 添加触摸函数
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event* event) {
		Vec2 touchLocation = touch->getLocation();
		//openGL转换成相对节点的坐标
		Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);

		//循环判断单击的是哪一个图片关卡图标
		for (int i = 0; i < this->getChildrenCount(); i++) {
			//获取但却场景的每一个子节点
			auto tempSprite = this->getChildren().at(i);
			if (tempSprite->getBoundingBox().containsPoint(nodeLocation)) {
				//判断玩家点击的是返回 还是关卡图标
				if (tempSprite->getTag() == 100) {
					//切换到开始界面
					auto transition = TransitionSplitRows::create(2.0f, StartLayer::createScene());
					Director::getInstance()->replaceScene(transition);
				}

				//如果玩家点击的是已经开放的关卡
				else if (tempSprite->getTag() > 0 &&
					tempSprite->getTag() < (m_successLevel + 1)) {
					// 删除当前的触摸事件监听
					Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
					//进行游戏主场景
					auto transtion = TransitionSplitRows::create(2.0f,
						PreloadLayer::createScene(tempSprite->getTag()));
					log("tempSprite Tag = %d", tempSprite->getTag());

					Director::getInstance()->replaceScene(transtion);
				}
				// 如果玩家点击的是尚未开发的关卡图标 提示信息弹出
				else if (tempSprite->getTag() != -1) {
					std::string msg = StringUtils::format("level %d is not opening", tempSprite->getTag());
					MessageBox(msg.c_str(), "提示");
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
