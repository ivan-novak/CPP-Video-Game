#pragma once
#include <SFML/Graphics.hpp>
#include "BasicMan.h"
#include <iostream>
#include <windows.h>
#include "VideoStudio.h"


using namespace sf;
using namespace std;


const int dir_Left = -1;
const int dir_Right = 1;
const int dir_Stop = 0;

const string nplayer = "player";
const string nboss = "boss";
const string nhimera = "himera";


class Player : public Basic
{
public:
	Player(double x, double y, float z);
	void virtual doEvent(Event event);
	void virtual DoMessage(Basic* source, int message);
};

class LiveBar : public Basic 

{
public:
	LiveBar(double x, double y, double z);
	void virtual Draw(Camera &screen);
	void virtual DoMessage(Basic* source, int message);
};

class DlgBox : public Basic
{
public:
	void virtual doEvent(Event event);
	DlgBox(Texture* texture, double x, double y, double z);
};

class Boss : public Basic
{
public:
	Boss(double x, double y, float z);
	void virtual DoMessage(Basic* source, int message);
};

class Wifi : public Basic
{
public:
	Wifi(double x, double y, float z);
	void virtual DoMessage(Basic* source, int message);
};

class Mage : public Basic
{
public:
	Mage(double x, double y, double z);
	void virtual DoMessage(Basic* source, int message);
};

class Samurai2 : public Basic
{
public:
	Samurai2(double x, double y, double z);
	void virtual DoMessage(Basic* source, int message);
};

class Bonefire : public Basic
{
public:
	Bonefire(double x, double y, double z);
	void virtual DoMessage(Basic* source, int message);
};

class Dragon : public Basic
{
public:
	Dragon(double x, double y, double z);
	void virtual doEvent(Event event);
};

class Samurai : public Basic
{
public:
	Samurai(double x, double y, double z);
	void virtual DoMessage(Basic* source, int message);
};

class Himera : public Basic
{
public:
	Himera(double x, double y, double z);
	void virtual DoMessage(Basic* source, int message);
};

class Himera2 : public Himera
{
public:
	Himera2(double x, double y, double z);
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Player::Player(double x, double y, float z)
{	
	name = nplayer;
	Locate.x = x;
	Locate.y = y;
	Locate.z = z;
	actions[ac_Go] = new Clip(*TNinjaGo, 155, 31, false, 9, 6);
	actions[ac_Stay] = new Clip(*TNinjaStay, 2,  2, false, 0, 0.7);
	actions[ac_Win] = new Clip(*TNinjaStay, 2, 2, false, 0, 0.7);
	actions[ac_Attack] = new Clip(*TNinjaAttack, 6, 6, false, 0, 1);
	actions[ac_Dead] = new Clip(*TNinjaDie, 5, 5, false, 0, 1, false);
	SetAction(ac_Stay,1);
	SetHealth(1);
}

//управляет поведением героя с клавиатуры
void Player::doEvent(Event event)
{
	if (currentAction == ac_Dead) return;
 	Basic::doEvent(event);
	if (Keyboard::isKeyPressed(Keyboard::H)) SetHealth(1); else
	if (Keyboard::isKeyPressed(Keyboard::A) && Locate.x > Begin.x && direction) SetAction(ac_Go,-1); else
	if (Keyboard::isKeyPressed(Keyboard::D) && Locate.x < End.x && direction) SetAction(ac_Go, 1); else
	if (Keyboard::isKeyPressed(Keyboard::F)) SetAction(ac_Attack,direction); else 
	SetAction(ac_Stay,direction);
}

void Player::DoMessage(Basic* source, int message)
{
	Basic::DoMessage(source, message);
	if (message != ms_HealthChange) return;
	if (source->name == nboss && source->health <= 0)
	{
		runResult = rs_OK;
		SetAction(ac_Win, direction);
		source->SetAction(ac_Dead, -direction);
		return;
	}
	if (source == this && health <= 0)
	{
		runResult = rs_No;
		SetAction(ac_Dead, direction);
		return;
	}
}


LiveBar::LiveBar(double x, double y, double z)
{
	Locate.x = x;
	Locate.y = y;
	Locate.z = z;
	actions[ac_Stay] = new Clip(*TLifeBar, 42, 5, false, 1, 0.0);
	actions[ac_Stay]->SetFrame(actions[ac_Stay]->countOfFrames - 1);
	SetAction(ac_Stay, 1);
}

void LiveBar::Draw(Camera &screen)
{
	Basic::Draw(screen);
}

void LiveBar::DoMessage(Basic* source, int message)
{
	if (source->name != nplayer) return;
	getAction()->SetFrame((double(getAction()->countOfFrames-1))*source->health);
	
}

void DlgBox::doEvent(Event event)
{
	if (!event.KeyPressed) return;
	if (event.key.code == Keyboard::Y) runResult = rs_OK;
	if (event.key.code == Keyboard::Return) runResult = rs_OK;
	if (event.key.code == Keyboard::N) runResult = rs_No;
	if (event.key.code == Keyboard::Escape) runResult = rs_No;
}

DlgBox::DlgBox(Texture* texture, double x, double y, double z)
{ 
	Locate.x = x;
	Locate.y = y;
	Locate.z = z;
	actions[ac_Stay] = new Clip(*texture, 1, 1, false, 1, 0.0);
	SetAction(ac_Stay, 1);
	getAction()->SetFrame(getAction()->countOfFrames - 1);
	SetVisiable(false);
}

Mage::Mage(double x, double y, double z)
{
	Locate.x = x;
	Locate.y = y;
	Locate.z = z;
	actions[ac_Go] = new Clip(*TMage, 69, 16, false, 2, 5);
	SetAction(ac_Go,-1);
}

void Mage::DoMessage(Basic* source, int message)
{
	Basic::DoMessage(source, message);
	if (source == nullptr) return; 
	if (source->name != nplayer ) return;
	if (!isTouch(source) && source->health < 0.2) MoveTo(source);
	if (isTouch(source) && source->getAction()->getFrame() == 0) source->HealthBy( 0.05);
}

Samurai2::Samurai2(double x, double y, double z)
{
	Locate.x = x;
	Locate.y = y;
	Locate.z = z;
	actions[ac_Stay] = new Clip(*TSamurai2Stay, 51, 5, false, 0, 3);
	actions[ac_Stay]->cycle = false;
	actions[ac_Attack] = new Clip(*TSamurai2Attack, 39, 5, false, 0, 3);
	SetAction(ac_Attack, 1);
}

void Samurai2::DoMessage(Basic* source, int message)
{
	Basic::DoMessage(source, message);
	actions[ac_Stay]->cycle = (source->name == nplayer && source->isTouch(this));
	if (actions[currentAction]->getFrame() == 0) SetAction(ac_Stay, direction);
	if (source == nullptr) return;
	if (source->name != nhimera) return;
	if (source->health <= 0) return;
	if (source->isFar(this)) return;
	SetAction(ac_Attack, direction);
	TurnTo(source);
	if (actions[currentAction]->getFrame() == 26) source->HealthBy(-0.34);
	cout << source->health << endl;
}


Bonefire::Bonefire(double x, double y, double z)
{
	Locate.x = x;
	Locate.y = y;
	Locate.z = z;
	actions[ac_Stay] = new Clip(*TBonefire, 5, 5, false, 0.2);
	SetAction(ac_Stay, -1);
}

void Bonefire::DoMessage(Basic* source, int message)
{
	Basic::DoMessage(source, message);
	if (source == nullptr) return;
	if (source->name != nplayer) return;
	if (isTouch(source) && source->getAction()->getFrame() == 0) source->HealthBy(0.05);
}

Dragon::Dragon(double x, double y, double z)
{
	Locate.x = x;
	Locate.y = y;
	Locate.z = z;
	actions[ac_Go] = new Clip(*TDragon, 18, 5, false, 5, 1);
	actions[ac_Go]->Revert = true;
	SetAction(ac_Go, -1);
}

void Dragon::doEvent(Event event)
{
	Basic::doEvent(event);
	if (getAction() != actions[ac_Go]) return;
}


Samurai::Samurai(double x, double y, double z)
{
	Locate.x = x;
	Locate.y = y;
	Locate.z = z;
	actions[ac_Go] = new Clip(*TSamuraiWalk, 38, 5, false, 2, 3);
	actions[ac_Stay]  = new Clip(*TSamuraiStay, 120, 5);
	SetAction(ac_Go,1);
}

void Samurai::DoMessage(Basic* source, int message)
{
	Basic::DoMessage(source, message);
	if (source->name == nplayer)
	{
		if (!isFar(source) && source->currentAction == ac_Go) SetAction(ac_Stay,direction);
		else SetAction(ac_Go, direction);
	};
}


Himera::Himera(double x, double y, double z)
{
	name = nhimera;
	Locate.x = x;
	Locate.y = y;
	Locate.z = z;
	actions[ac_Go] = new Clip(*THimeraWalk, 12,  12, false, 3, 2);
	actions[ac_Stay] = new Clip(*THimeraStay, 18,  18, false, 0, 2.5);
	actions[ac_Attack] = new Clip(*THimeraAttackTail, 9, 9, false, 0, 1);
	actions[ac_Dead] = new Clip(*THimeraDied, 28, 5, false, 0, 1, false);
	actions[ac_Win] = new Clip(*THimeraWin, 2, 5, false, 0, 1, false);
	SetAction(ac_Stay,1);
}


void Himera::DoMessage(Basic* source, int message)
{
	Basic::DoMessage(source, message);
	if (currentAction == ac_Dead) return;
	if (source == nullptr) return; 
	if (source->name == nboss)
	{
		if (source->health <= 0) SetHealth(0);
		return;
	}
	if (source->name != nplayer) return;
	if (source->health <= 0)
	{
		SetAction(ac_Win, direction);
		return;
	}
	if (Distance(source) > 1.5) SetAction(ac_Stay, direction);
	if (isTouch(source))  
	{ 
		AttackTo(source);
		if (source->currentAction == ac_Attack && message == ms_PictureChange && source->getAction()->getFrame() == 1 && source->isSeeMe(this)) HealthBy(-0.1);
		if (getAction()->getFrame() == 4)
		{
			getAction()->SetFrame(getAction()->getFrame() + 1);
			source->HealthBy(-0.05);
		}
		return;
	} 	
	if (source->health <= 0)  return;
	if (isNear(source))
	{
		if (health > 0.1) MoveTo(source); 
		return;
	}	


}

Himera2::Himera2(double x, double y, double z) : Himera(x, y, z)
{
	actions[ac_Attack] = new Clip(*THimeraAttack2, 17, 5, false, 0, 1);
}

Boss::Boss(double x, double y, float z)
{
	name = nboss;
	Locate.x = x;
	Locate.y = y;
	Locate.z = z;
	actions[ac_Go] = new Clip(*TBossAttack, 17,  5, false, 0, 1);
	actions[ac_Stay] = new Clip(*TBossStay, 4,  4, false, 0, 1);
	actions[ac_Win] = new Clip(*TBossStay, 4, 4, false, 0, 1);
	actions[ac_Attack] = new Clip(*TBossAttack, 17,  5, false, 0, 1);
	actions[ac_Dead] = new Clip(*TBossDie, 25,  5, false, 0, 1, false);
	actions[ac_Attack]->Revert = true;
	actions[ac_Stay]->Revert = true;
	actions[ac_Dead]->Revert = true;
	SetAction(ac_Stay,-1);
}


void Boss::DoMessage(Basic* source, int message)
{
	Basic::DoMessage(source, message);
	if (source == nullptr) return; 
	if (currentAction == ac_Dead) return;
	if (source->name != nplayer) return;
	if (source->health <= 0 || Distance(source) > 1.5) 
	{
		SetAction(ac_Stay, direction);
		return;
	}
	if (source->health <= 0)
	{
		SetAction(ac_Win, direction);
		return;
	}
	if (isTouch(source))
	{
		AttackTo(source);
		if (source->currentAction == ac_Attack && message == ms_PictureChange && source->getAction()->getFrame() == 1 && source->isSeeMe(this)) HealthBy(-0.1);
		if (currentAction == ac_Attack  && getAction()->getFrame() == 4)
		{
			getAction()->SetFrame(getAction()->getFrame() + 1);
			source->HealthBy(-0.05);
		}
		return;
	}
}

Wifi::Wifi(double x, double y, float z)
{
	Locate.x = x;
	Locate.y = y;
	Locate.z = z;
	actions[ac_Stay] = new Clip(*TWifiSit, 14, 5, true, 0, 3);
	actions[ac_Attack] = new Clip(*TWifiTolk, 18, 5, false, 0, 3);
	SetAction(ac_Stay, 1);
}

void Wifi::DoMessage(Basic* source, int message)
{
	if (source->name == nplayer)
	{
		TurnTo(source);
		if (isFar(source)) SetAction(ac_Stay, direction);
		else SetAction(ac_Attack, direction);
	};
}