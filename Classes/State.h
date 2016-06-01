#ifndef _State_H_
#define _State_H_

#include "CommonUtils.h"

class Enemy;

class State{
public:
	virtual ~State() {}
	virtual void Enter(Enemy*) = 0;         //状态进入时执行
	virtual void Execute(Enemy*) = 0;		 //状态正常更新
	virtual void Exit(Enemy*) = 0;			 // 状态结束时执行
};

#endif
