#ifndef _LevelUtils_H_
#define _LevelUtils_H_

#include "cocos2d.h"

using namespace cocos2d;

class LevelUtils : public Ref {
public:
	//��ȡ�ؿ�����
	static int readLevelFromFile();
	//��¼�����ļ�
	static void writeLevelToFile(int level);


};

#endif
