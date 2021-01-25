#pragma once
#include "VideoStudio.h"
#include "Textures.h"
#include <list>  
#include <SFML/Graphics.hpp>


using namespace sf;
using namespace std;

Vector2f Begin = { -30000, 10000 };
Vector2f End = { 30000, 0 };

const int ac_Win = 5;
const int ac_Stay = 1;
const int ac_Go = 2;
const int ac_Attack = 3;
const int ac_Dead = 4;


const int ms_Locate = 100;
const int ms_Show = 101;
const int ms_Hide = 102;
const int ms_Alive = 104;
const int ms_Action = 105;
const int ms_Init = 106;
const int ms_Done = 107;
const int ms_PictureChange = 108;
const int ms_NewDiection = 109;
const int ms_HealthChange = 110;
const int ms_Idle = 0;

const int rs_None = -1;
const int rs_No = 0;
const int rs_OK = 1;
const int rs_Esc = 2;

const int Distance_Far = 500;
const int Distance_Near = 10;

//---------------------------------------------------------------------------------------------------------------------

//олицетвор€ет объекты игрового мира
//содержит набор клипов отображени€ состо€ний игрового мира 
//имеет координату глобального мира который сообщаетс€ всем клипам
//может владеть главным игровым циклом программы (обрабатывает событи€ Windows) (Run)
//каждый объект обрабатывает все сообщени€ игрового мира 
//изменение состо€ний объекта сообщаетс€ всем остальным объеутам
//производные классы обрабатывают эти сообщени€ дл€ модификации своего поведени€ 

class Basic
{
public:
	Basic();
	~Basic();
	Vector3<float> Locate;
	bool visieble = true;
	string name = "";
	bool isLive;
	float health = 1;
	int stamina;
	int damage;
	void MoveBy(double dx, double dy);
	void virtual DoMessage(Basic* source, int message);
	void virtual Death() {}
	Vector2u virtual Size();
	void virtual Attack(Basic* enemy) {}
	void virtual doEvent(Event event);
	void virtual Draw(Camera &screen);
	void virtual SetLocate(double x, double y);
	void virtual SetVisiable(bool visiable);
	void virtual SetAction(int action, int direction);
	void virtual SetDirection(int dir);
	void virtual PutMessage(int messagie);
	void virtual TurnTo(Basic* source);
	void virtual MoveTo(Basic* source);
	void virtual MoveFrom(Basic* source);
	void virtual AttackTo(Basic* source);
	bool virtual isFar(Basic* source);
	bool virtual isNear(Basic* source);
	bool virtual isTouch(Basic* source);
	void virtual SetHealth(float health);
	bool isSeeMe(Basic* source);
	void HealthBy(float dhealth);
	float Distance(Basic* source);
	int virtual Run(Camera* camera);
	Clip* getAction();
	int currentAction = 0;
	int runResult = rs_None;
protected:
	int direction = 1;
	Clip *actions[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };
};

//статический список всех объектов игрового мира
//объекты добавл€ютс€ при создании 
//служат дл€ главного цикла передачи сообщений и отрисовки камеры

list<Basic*> characters;

//очередь сообщени€ игрового мира
//каждый объект игрового мира может положить сообщение 
//текущий модальный элемент изымает очередное сообщение и передает всем объектам мира

static class Messager
{
public:
	static void put(Basic* source, int message);
	static bool get(Basic* &source, int &message);
	static void clear();
};

//ф-€ дл€ ф=визуализации сообщений на экране

int MsgBox(Basic* source, Camera* camera)
{
	source->SetLocate(camera->getSize().x/2, camera->getSize().y-((camera->getSize().y - source->getAction()->sizeY)/2));
	source->SetVisiable(true);
	int msg = source->Run(camera);
	source->SetVisiable(false);
	return msg;
}

class Moon : public Basic
{
public:
	Moon(double x, double y, float z);
};

class Forest : public Basic
{
public:
	Forest(double x, double y, float z);
};

class Road : public Basic
{
public:
	Road(double x, double y, float z);
};

class Hills : public Basic
{
public:
	Hills(double x, double y, float z);
};


class Decor : public Basic
{
public:
	Decor(double x, double y, float z, Texture* texture, int countOfFrames = 1,  int countOfColums = 1, bool isPanorama = false, float steps = 2, float duration = 1.0, bool cycle = true);
};


//--------------------------------------------------------------------------------------------------------------------------------------------------

struct Message
{
	Basic* source;
	int message;
};

list<Message> query;

void Messager::put(Basic* source, int message)
{
	query.push_back({ source, message });
}

bool Messager::get(Basic* &source, int &message)
{
	if (query.empty()) return false;
	Message buff = query.front();
	query.pop_front();	
	source = buff.source;
	message = buff.message;
	return true;
}

void Messager::clear()
{
	query.clear();
}

Basic::Basic()
{
	PutMessage(ms_Init);
	characters.push_back(this);
}

Basic::~Basic()
{
	PutMessage(ms_Done);
	characters.remove(this);
}


void Basic::SetHealth(float health)
{
	if (health > 1) health = 1;
	if (health <= 0) health = 0;
	if (this->health == health) return;
	this->health = health;	
	PutMessage(ms_HealthChange);
	if (this->health <= 0) SetAction(ac_Dead,direction);
}

bool Basic::isSeeMe(Basic* source)
{
	if (source == nullptr) return false;
	return(source->Locate.x-Locate.x)*direction > 0;
}

void Basic::HealthBy(float dhealth)
{
	SetHealth(health+dhealth);
}

void Basic::PutMessage(int message)
{
	if (name == "") return;
	Messager::put(this, message);
}

Clip* Basic::getAction()
{
	return actions[abs(currentAction)];
}

void Basic::Draw(Camera &screen)
{
	if (!visieble) return;
	if (getAction() == nullptr) return;
	int buff = getAction()->getFrame();
	getAction()->Draw(screen, Locate.x, Locate.y, Locate.z, direction < 0);
	if (buff != getAction()->getFrame()) PutMessage(ms_PictureChange);
}

void Basic::MoveBy(double dx, double dy)
{
	SetLocate(Locate.x + dx, Locate.y + dy);
}

void Basic::SetLocate(double x, double y)
{
	if (Locate.x == x && Locate.y == y) return;
	Vector3<float> Buff = Locate;
	Locate.x = x;
	Locate.y = y;
	if (int(Locate.x - Buff.x) != 0 || int(Locate.y - Buff.y) != 0 || int(Locate.z - Buff.z) != 0) PutMessage(ms_Locate);
}

void Basic::SetDirection(int Dir)
{
	if (Dir != -1) Dir = 1;
	if (direction == Dir) return;
	direction = Dir;
	PutMessage(ms_NewDiection);
}

void Basic::SetVisiable(bool show)
{
	if (show == visieble) return;
	visieble = show;
	if (visieble) PutMessage(ms_Show);
	else PutMessage(ms_Hide);
}

void Basic::TurnTo(Basic* source)
{
	if (source->Locate.x - Locate.x < 0) SetDirection(-1); else SetDirection(1);
}

void Basic::MoveTo(Basic* source)
{
	TurnTo(source);
	SetAction(ac_Go,direction);
}

void Basic::MoveFrom(Basic* source)
{
	TurnTo(source);
	SetAction(ac_Go,-direction);
}

void Basic::AttackTo(Basic* source)
{
	TurnTo(source);
	SetAction(ac_Attack,direction);
}

Vector2u Basic::Size()
{
	if (getAction() == nullptr) return{ 0, 0 };
	return{ (unsigned int)getAction()->sizeX, (unsigned int)getAction()->sizeY};
}

float Basic::Distance(Basic* source)	
{
	double D =source->Locate.x - Locate.x;
	double B = source->Size().x + Size().x;
	return  abs(D/B);
	
	return abs(abs(source->Locate.x + source->Size().x / 2 - Locate.x + Size().x / 2) /(source->Size().x / 2 + Size().x / 2));
}

//главный цикл программы, который инициируетс€ текущим модальным элементам (Player и окна сообщений)
//
//обрабатывает системные сообщени€ windows 
//изымает сообщени€ из очереди сообщений и передает всем объектам игрового мира 
//позиционирует камеру на себ€
//просит камеру отрисовать все объекты игрового мира
//цикл обработки продолжаютс€ пока свойства runResult = 0 


int Basic::Run(Camera* camera)
{
	runResult = rs_None;
	while (runResult == rs_None)
	{
		list<Basic*>::iterator ptr;
		Message Buff;
		Event event;
		Timer = camera->clock.restart();
		while (camera->pollEvent(event)) if (event.type == Event::Closed) camera->close();
		camera->DoEvent(event);
		doEvent(event);
		Buff.message = ms_Idle;
		Buff.source = this;
		Messager::get(Buff.source, Buff.message);
		for (ptr = characters.begin(); ptr != characters.end(); ptr++) (*ptr)->DoMessage(Buff.source, Buff.message);
		if(Locate.z > 0) camera->SetCadr(Locate.x, Locate.y);
		for (ptr = characters.begin(); ptr != characters.end(); ptr++) (*ptr)->Draw(*camera);
		camera->display();
	}
	return runResult;
}

void Basic::doEvent(Event event)
{
}

bool Basic::isFar(Basic* source)
{
	return Distance(source) > 3;
}

bool Basic::isNear(Basic* source)
{
	return !isFar(source);
}

bool Basic::isTouch(Basic* source)
{
	return  Distance(source) < 1;
}

void  Basic::SetAction(int action, int direction)
{
	SetDirection(direction);
	if (currentAction == action) return;
	currentAction = action;
	PutMessage(ms_Action);
}

void Basic::DoMessage(Basic* source, int message)
{

	if(message != ms_Idle && getAction() == nullptr || currentAction != ac_Go ) return;
	if (Locate.x < Begin.x && direction == -1) SetDirection(1);
	if (Locate.x > End.x && direction == 1) SetDirection(-1);
	MoveBy(getAction()->getSpeed()*Timer.asSeconds()*direction, 0);
}

//---------------------------------------------------------------------------------------------------------------

Moon::Moon(double x, double y, float z)
{
	Locate.z = z;
	actions[ac_Stay] = new Clip(*TMoon);
	actions[ac_Stay]->visieble = true;
	SetLocate(x, y);
	SetAction(ac_Stay,1);
};

Forest::Forest(double x, double y, float z)
{
	Locate.z = z;
	actions[ac_Stay] = new Clip(*TForest);
	actions[ac_Stay]->isPanorama = true;
	actions[ac_Stay]->visieble = true;
	SetLocate(x, y);
	SetAction(ac_Stay,1);
};


Hills::Hills(double x, double y, float z)
{
	Locate.z = z;
	actions[ac_Stay] = new Clip(*THills);
	actions[ac_Stay]->isPanorama = true;
	actions[ac_Stay]->visieble = true;
	SetLocate(x, y);
	SetAction(ac_Stay,1);
};

Road::Road(double x, double y, float z)
{
	Locate.z = z;
	actions[ac_Stay] = new Clip(*TRoad);
	actions[ac_Stay]->isPanorama = true;
	actions[ac_Stay]->visieble = true;
	SetLocate(x, y);
	SetAction(ac_Stay,1);
};


Decor::Decor(double x, double y, float z, Texture* texture, int countOfFrames, int countOfColums, bool isPanorama, float steps, float duration, bool cycle)
{
	Locate.z = z;
	actions[ac_Stay] = new Clip(*texture, countOfFrames, countOfColums, isPanorama, steps,  duration, cycle);
	actions[ac_Stay]->visieble = true;
	SetLocate(x, y);
	SetAction(ac_Stay,1);
};


