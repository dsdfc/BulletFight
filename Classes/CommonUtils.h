#ifndef _Common_H_
#define _Common_H_

#include "SimpleAudioEngine.h"
#include "CSVParser.h"

using namespace CocosDenshion;

#define bg_music_1 "sound/bg_music_1.mp3"
#define fire_fx_1 "sound/fire1.mp3"
#define hit_fx_1 "sound/hit1.mp3"
#define die_fx_1 "sound/die1.mp3"

#define MASK_ACTORBULLET 0x01  
#define MASK_ENEMY 0x01
#define MASK_ACTOR 0x10
#define MASK_ENEMYBULLET 0x10
#define MASK_BOX 0x111

static const int TAG_ENEMY_BULLET = 1;
static const int TAG_ACTOR_BULLET = 2;
static const int TAG_ENEMY = 3;
static const int TAG_ACTOR = 4;
static const int TAG_BOX = 5;
static const int TAG_GUN = 6;

//场景层次
#define LAYER_BG 0
#define LAYER_ACTOR 10
#define LAYER_CONTROL 20

static const int MAX_HP = 5;

#include "cocos2d.h"
using namespace cocos2d;

typedef enum {
	UP = 1,
	DOWN,
	LEFT,
	RIGHT,
	NONE
}Direction;

typedef enum {
	F_LEFT,
	F_RIGHT
}ActorFace;

//相对方向
typedef enum {
	RD_LEFT,
	RD_RIGHT,
	RD_NONE
}RelativeDirection;

enum GunType {
	SMG,  // 冲锋枪
	AMP,  // 狙击枪
	LMG,  // 轻机枪
	pistol, // 手枪
	shotgun // 猎枪
};

enum WINNER
{
	WINNER_ACTOR,
	WINNER_ENEMY,
	WINNER_NONE
};

class CommonUtil : Ref {
public:
	static CommonUtil* getInstance();

	CommonUtil();
	~CommonUtil();

	void loadMapData();
	void loadGunsData();
	void loadMapRowDist();
	int getCurrentStage();
	void upgradeStage();
	int getLastFloor();
	void chooseLevel(int level);
	void getRowDist();

	//叉乘
	static double mult(Point a, Point b, Point c);
	// 判断两条直线是否相交
	//aa, bb为一条线段两端点 cc, dd为另一条线段的两端点 相交返回true, 不相交返回false  
	static bool intersect(Point aa, Point bb, Point cc, Point dd);

	std::vector<Vec4> groundRect; // 存储矩形对角线的两点
	std::vector<int> groundFloorIndex; //矩形所对应的层数
	std::vector<Vec4> noJumpVec; //不能跳跃的区域

	std::map<int, Vec2> m_rowDistData; // 存储层数对应的向上和向下的距离
	std::map<std::string, std::vector<int>> m_gunMsgs; //存储枪械对应的数据
	std::vector<std::string> m_gunsNames; 

	int m_currentLevel;
	bool m_isInUpStage;  // 用来判断是否在上半部分场景
	Size m_MapSize = Size(1280, 960);

	float rowDistMax;
	float rowDistMin;
	WINNER m_currentWinner;
};

#endif