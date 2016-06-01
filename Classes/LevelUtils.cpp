#include "LevelUtils.h"


int LevelUtils::readLevelFromFile()
{
	//��ȡ��ǰ�ɹ��Ĺؿ���
	int successLevel = UserDefault::getInstance()->getIntegerForKey("SuccessLevel");
	if (successLevel) {
		return successLevel;
	}
	//Ĭ���ǵ�һ��
	return 1;
}

void LevelUtils::writeLevelToFile(int level) {
	//��ͨ�صĹؿ���д���ļ�
	UserDefault::getInstance()->setIntegerForKey("SuccessLevel", level);
}