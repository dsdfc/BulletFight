#include "LevelUtils.h"


int LevelUtils::readLevelFromFile()
{
	//读取当前成功的关卡数
	int successLevel = UserDefault::getInstance()->getIntegerForKey("SuccessLevel");
	if (successLevel) {
		return successLevel;
	}
	//默认是第一关
	return 1;
}

void LevelUtils::writeLevelToFile(int level) {
	//将通关的关卡数写入文件
	UserDefault::getInstance()->setIntegerForKey("SuccessLevel", level);
}