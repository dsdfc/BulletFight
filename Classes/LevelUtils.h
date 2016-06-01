#ifndef _LevelUtils_H_
#define _LevelUtils_H_

#include "cocos2d.h"

using namespace cocos2d;

class LevelUtils : public Ref {
public:
	//读取关卡数据
	static int readLevelFromFile();
	//记录过关文件
	static void writeLevelToFile(int level);


};

#endif
