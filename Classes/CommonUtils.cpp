#include "CommonUtils.h"


CommonUtil* CommonUtil::getInstance()
{
	static CommonUtil _instance;

	return &_instance;
}

CommonUtil::CommonUtil() {
	loadGunsData();
	loadMapRowDist();
	m_isInUpStage = true;
	m_currentWinner = WINNER_NONE;
}

CommonUtil::~CommonUtil() {

}

void CommonUtil::loadMapData()
{
	groundRect.clear();
	// 加载地板数据
	std::string groundCsv = StringUtils::format("data/stage_ground_%d.csv", m_currentLevel);
	__Array* data = CSVParser::parse(groundCsv.c_str());

	for (int i = 1; i < data->count(); i++) {
		__Array* row = static_cast<__Array *>(data->getObjectAtIndex(i)); // 从data数组中取出一行数据

		Vec4 vec;
		for (int j = 0; j < row->count(); j++) {
			__String* str = static_cast<__String*>(row->getObjectAtIndex(j));
			if (j == 1) {
				vec.x = str->intValue();
			}
			else if (j == 2) {
				vec.y = str->intValue();
			}
			else if (j == 3) {
				vec.z = str->intValue();
			}
			else if (j == 4) {
				vec.w = str->intValue();
			}
			else if (j == 5) {
				groundFloorIndex.push_back(str->intValue());
			}
		}
		groundRect.push_back(vec);
	}
	//for (auto x : groundVec) {
	//	log("x.x = %f, x.y = %f, x.z = %f, x.w = %f", x.x, x.y, x.z, x.w);
	//}

	//log("---------------------------------------");

	//for (auto x : groundFloor) {
	//	log("floor = %d", x);
	//}
}

void CommonUtil::loadGunsData()
{
	std::string gunsCsv = "data/guns.csv";
	__Array* data = CSVParser::parse(gunsCsv.c_str());

	std::string key;
	
	for (int i = 1; i < data->count(); i++) {

		std::vector<int> gunsData;
		__Array* row = static_cast<__Array *>(data->getObjectAtIndex(i)); // 从data数组中取出一行数据
		
		__String* str = static_cast<__String*>(row->getObjectAtIndex(0));
		key = str->getCString();
		m_gunsNames.push_back(key);

		for (int j = 1; j < row->count(); j++) {
			__String* str = static_cast<__String*>(row->getObjectAtIndex(j));
			gunsData.push_back(str->intValue());
		}
		m_gunMsgs[key] = gunsData;
	}

	for (auto x : m_gunMsgs) {
		log("key = %s", x.first.c_str());
		for (auto y : x.second) {
			log("value = %d", y);
		}
	}


}

void CommonUtil::loadMapRowDist()
{
	std::string rowsCsv = "data/rowDist.csv";
	__Array* data = CSVParser::parse(rowsCsv.c_str());

	int floorKey;
	int distValue;
	Vec2 dists;
	for (int i = 1; i < data->count(); i++)
	{
		__Array* row = static_cast<__Array *>(data->getObjectAtIndex(i)); // 从data数组中取出一行数据

		__String* str = static_cast<__String*>(row->getObjectAtIndex(0));
		floorKey = str->intValue();

		for (int j = 1; j < row->count(); j++)
		{
			__String* str = static_cast<__String*>(row->getObjectAtIndex(j));
			distValue = str->intValue();
			log("distValue = %d", distValue);
			if (j == 1)
			{
				dists.x = distValue;
			}
			if (j == 2)
			{
				dists.y = distValue;
			}
		}
		m_rowDistData[floorKey] = dists;
	}

	for (auto x : m_rowDistData) {
		log("key = %d", x.first);

		log("max = %f", x.second.x);
		log("min = %f", x.second.y);
	}
}

int CommonUtil::getCurrentStage()
{
	return m_currentLevel;
}

// 升级场景
void CommonUtil::upgradeStage()
{
	m_currentLevel = m_currentLevel + 1;
	loadMapData();                                 //重新加载关卡数据
	m_isInUpStage = true;
}

//获取最后一层的层数
int CommonUtil::getLastFloor()
{
	int lastfloor = groundFloorIndex[0];
	for (auto x : groundFloorIndex) {
		if (x > lastfloor) {
			lastfloor = x;
		}
	}

	return lastfloor;
}

void CommonUtil::chooseLevel(int level)
{
	m_currentLevel = level;
	loadMapData();
	m_isInUpStage = true;
}


double CommonUtil::mult(Point a, Point b, Point c)
{
	return (a.x - c.x)*(b.y - c.y) - (b.x - c.x)*(a.y - c.y);
}

bool CommonUtil::intersect(Point aa, Point bb, Point cc, Point dd)
{
	if (max(aa.x, bb.x) < min(cc.x, dd.x))
	{
		return false;
	}
	if (max(aa.y, bb.y) < min(cc.y, dd.y))
	{
		return false;
	}
	if (max(cc.x, dd.x) < min(aa.x, bb.x))
	{
		return false;
	}
	if (max(cc.y, dd.y) < min(aa.y, bb.y))
	{
		return false;
	}
	if (mult(cc, bb, aa)*mult(bb, dd, aa) < 0)
	{
		return false;
	}
	if (mult(aa, dd, cc)*mult(dd, bb, cc) < 0)
	{
		return false;
	}
	return true;
}

