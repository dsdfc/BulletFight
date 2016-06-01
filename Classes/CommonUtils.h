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

//�������
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

//��Է���
typedef enum {
	RD_LEFT,
	RD_RIGHT,
	RD_NONE
}RelativeDirection;

enum GunType {
	SMG,  // ���ǹ
	AMP,  // �ѻ�ǹ
	LMG,  // ���ǹ
	pistol, // ��ǹ
	shotgun // ��ǹ
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

	//���
	static double mult(Point a, Point b, Point c);
	// �ж�����ֱ���Ƿ��ཻ
	//aa, bbΪһ���߶����˵� cc, ddΪ��һ���߶ε����˵� �ཻ����true, ���ཻ����false  
	static bool intersect(Point aa, Point bb, Point cc, Point dd);

	std::vector<Vec4> groundRect; // �洢���ζԽ��ߵ�����
	std::vector<int> groundFloorIndex; //��������Ӧ�Ĳ���
	std::vector<Vec4> noJumpVec; //������Ծ������

	std::map<int, Vec2> m_rowDistData; // �洢������Ӧ�����Ϻ����µľ���
	std::map<std::string, std::vector<int>> m_gunMsgs; //�洢ǹе��Ӧ������
	std::vector<std::string> m_gunsNames; 

	int m_currentLevel;
	bool m_isInUpStage;  // �����ж��Ƿ����ϰ벿�ֳ���
	Size m_MapSize = Size(1280, 960);

	float rowDistMax;
	float rowDistMin;
	WINNER m_currentWinner;
};

#endif