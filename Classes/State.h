#ifndef _State_H_
#define _State_H_

#include "CommonUtils.h"

class Enemy;

class State{
public:
	virtual ~State() {}
	virtual void Enter(Enemy*) = 0;         //״̬����ʱִ��
	virtual void Execute(Enemy*) = 0;		 //״̬��������
	virtual void Exit(Enemy*) = 0;			 // ״̬����ʱִ��
};

#endif
