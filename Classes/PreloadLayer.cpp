#include "PreloadLayer.h"
#include "FightLayer.h"
#include "CommonUtils.h"


cocos2d::Scene* PreloadLayer::createScene(int level)
{
	CommonUtil::getInstance()->m_currentLevel = level;
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = PreloadLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool PreloadLayer::init()
{
	/**  you can create scene with following comment code instead of using csb file.**/
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//预加载资源时显示的图片资源
	auto loading = Sprite::create("pic/background.jpg");
	loading->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(loading);

	//创建进度条精灵
	auto bar = Sprite::create("pic/progressbar.png");
	//初始化一个进度条对象
	m_progress = ProgressTimer::create(bar);
	//并未加载任何资源文件
	m_progress->setPercentage(0.0f);
	m_progress->setScale(0.5f);
	m_progress->setMidpoint(Vec2(0, 0.5));
	m_progress->setBarChangeRate(Vec2(1, 0));
	m_progress->setType(ProgressTimer::Type::BAR);

	m_progress->setPosition(Vec2(visibleSize.width / 2 - 50, 10));
	this->addChild(m_progress);

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(bg_music_1);

	//模拟需要加载的资源文件
	m_sourceCount = 2;
	m_progressInterval = 100.0 / (float)m_sourceCount;
	this->schedule(schedule_selector(PreloadLayer::load), 1.0f);


	return true;
}

void PreloadLayer::load(float delta) {
	// 以渐进的方式显示图片
	auto ac = ProgressTo::create(1, 100 - m_progressInterval*m_sourceCount);
	m_progress->runAction(ac);
	if (m_sourceCount < 0) {
		//当所有资源文件加载完成后 切换至主场景
		auto scene = FightLayer::createSceneWithLevel(CommonUtil::getInstance()->m_currentLevel);
		Director::getInstance()->replaceScene(scene);
		m_progress->stopAllActions();
	}
	//每次调用该方法说明加载了一个资源，自减更新资源总数
	m_sourceCount--;
}