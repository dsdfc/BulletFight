#ifndef _Box_H_
#define _Box_H_

#include "CommonUtils.h"
#include "cocos2d.h"
using namespace cocos2d;

class Box : public Sprite{
public:
	CREATE_FUNC(Box);
	Point location;
	Point getRandPosition();
	virtual bool init();
	virtual void updateCustome();
	void setPos();
	virtual bool onContactBox(const PhysicsContact& contact);
};


#endif
