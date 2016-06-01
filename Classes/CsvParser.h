#ifndef _CSVParser_H_
#define _CSVParser_H_

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class CSVParser {
public:
	static __Array* parse(const char* fileName);

};

#endif
