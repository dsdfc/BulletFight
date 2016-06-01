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

	//Ԥ������Դʱ��ʾ��ͼƬ��Դ
	auto loading = Sprite::create("pic/background.jpg");
	loading->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(loading);

	//��������������
	auto bar = Sprite::create("pic/progressbar.png");
	//��ʼ��һ������������
	m_progress = ProgressTimer::create(bar);
	//��δ�����κ���Դ�ļ�
	m_progress->setPercentage(0.0f);
	m_progress->setScale(0.5f);
	m_progress->setMidpoint(Vec2(0, 0.5));
	m_progress->setBarChangeRate(Vec2(1, 0));
	m_progress->setType(ProgressTimer::Type::BAR);

	m_progress->setPosition(Vec2(visibleSize.width / 2 - 50, 10));
	this->addChild(m_progress);

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(bg_music_1);

	//ģ����Ҫ���ص���Դ�ļ�
	m_sourceCount = 2;
	m_progressInterval = 100.0 / (float)m_sourceCount;
	this->schedule(schedule_selector(PreloadLayer::load), 1.0f);


	return true;
}

void PreloadLayer::load(float delta) {
	// �Խ����ķ�ʽ��ʾͼƬ
	auto ac = ProgressTo::create(1, 100 - m_progressInterval*m_sourceCount);
	m_progress->runAction(ac);
	if (m_sourceCount < 0) {
		//��������Դ�ļ�������ɺ� �л���������
		auto scene = FightLayer::createSceneWithLevel(CommonUtil::getInstance()->m_currentLevel);
		Director::getInstance()->replaceScene(scene);
		m_progress->stopAllActions();
	}
	//ÿ�ε��ø÷���˵��������һ����Դ���Լ�������Դ����
	m_sourceCount--;
}