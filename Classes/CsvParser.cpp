#include "CSVParser.h"

__Array* CSVParser::parse(const char* fileName)
{
	auto sharedFileUtils = FileUtils::getInstance();

	string pathKey = sharedFileUtils->fullPathForFilename(fileName);

	string content = sharedFileUtils->getStringFromFile(pathKey);
	__String* contentStr = __String::create(content);

	__Array* rows = contentStr->componentsSeparatedByString("\n"); //将字符串分割
	__Array* ret = __Array::createWithCapacity(rows->count()); //创建数组对象，指定数组的初始容量

	Object* obj = nullptr;

	CCARRAY_FOREACH(rows, obj) {
		auto fieldStr = static_cast<__String *>(obj);
		__Array* fields = fieldStr->componentsSeparatedByString(",");
		ret->addObject(fields);
	}

	return ret;
}