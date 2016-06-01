#include "Enemy.h"


// ¹¥»÷×´Ì¬

AttackState* AttackState::getInstance()
{
	static AttackState instance;
	return &instance;
}

void AttackState::Enter(Enemy* enemy)
{
	//log("AttackState::Enter");
}

void AttackState::Execute(Enemy* enemy)
{
	log("AttackState::Execute");
	//enemy->changeState(AvoidState::getInstance());
	enemy->updatePos();
	enemy->updateAvoidDrop();
	if (enemy->isBoxHere()) {
		log("enemy->isBoxHere() = true");
		TakeBoxState::getInstance()->Execute(enemy);
	}
	
}

void AttackState::Exit(Enemy* enemy)
{
	//log("AttackState::Exit");
}

//AvoidState* AvoidState::getInstance()
//{
//	static AvoidState instance;
//
//	return &instance;
//}
//
//void AvoidState::Enter(Enemy* enemy)
//{
//	//log("AvoidState::Enter");
//}
//void AvoidState::Execute(Enemy* enemy)
//{
//	enemy->changeState(TakeBoxState::getInstance());
//}
//void AvoidState::Exit(Enemy* enemy)
//{
//	//log("AvoidState::Exit");
//}

TakeBoxState* TakeBoxState::getInstance()
{
	static TakeBoxState instance;
	return &instance;
}

void TakeBoxState::Enter(Enemy* enemy)
{
	//log("TakeBoxState::Enter");
}
void TakeBoxState::Execute(Enemy* enemy)
{
	log("TakeBoxState::Execute");
	enemy->updateCloseToBox();
	if (!enemy->isBoxHere()) {
		AttackState::getInstance()->Execute(enemy);
	}
}
void TakeBoxState::Exit(Enemy* enemy)
{
	//log("TakeBoxState::Exit");
}