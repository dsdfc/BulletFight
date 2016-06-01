#ifndef _EnemyControlState_H_
#define _EnemyControlState_H_

#include "State.h"

class Enemy;

//����״̬
class AttackState : public State{
private:
	AttackState() {}
	AttackState(const AttackState&);
	AttackState& operator=(const AttackState&);

public:
	static AttackState* getInstance();
	virtual void Enter(Enemy*);
	virtual void Execute(Enemy*);
	virtual void Exit(Enemy*);
};

// ���״̬
class AvoidState : public State{
private:
	AvoidState() {}
	AvoidState(const Enemy*);
	AvoidState& operator=(const AvoidState&);

public:
	static AvoidState* getInstance();
	virtual void Enter(Enemy*);
	virtual void Execute(Enemy*);
	virtual void Exit(Enemy*);
};

// ��ȡ����״̬
class TakeBoxState : public State{
private:
	TakeBoxState() {}
	TakeBoxState(const Enemy*);
	TakeBoxState& operator=(const TakeBoxState&);

public:
	static TakeBoxState* getInstance();
	virtual void Enter(Enemy*);
	virtual void Execute(Enemy*);
	virtual void Exit(Enemy*);
};

#endif //_EnemyControlState_H_
