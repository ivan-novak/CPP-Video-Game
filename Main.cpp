#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.h"
#include "VideoStudio.h"
#include "Textures.h"
#include <ctime>

#include <list>

using namespace sf;

const int objCount = 11;
const int countOfHimers = 4;


float getPosition(int i, int count)
{
	float delta = (End.x - Begin.x -200) / count;
	float result = Begin.x + 200 + delta*i  + delta* float(rand()) / float(RAND_MAX);
	return result;
}

float Track(float Track)
{
	return 4.0 + Track;
}


int main()
{
	Begin = { -30000, 10000 };
	End = { 30000, 0 };
	Camera camera(0, 0, VideoMode(1920, 1080), "Dark Souls", Style::Fullscreen);
	bool isRepeat = false;

	do
	{	srand(time(0));
		new Moon(-90000, 100, Track(26));
		new Decor(-9000, 45000, Track(16), Mountain);
		new Hills(0, 1000, Track(4));
		for (int i = 0; i < 2; i++) new Decor(getPosition(i, 2), 1950, Track(3.5), THouse4);
		for (int i = 0; i < 3; i++) new Decor(getPosition(i, 3), 1700, Track(3.2), THouse3);
		new Dragon(End.x + 1000, 5000, Track(4));
		for (int i = 0; i < 4; i++) new Decor(getPosition(i, 4), 1550, Track(3.1), THouse2);
		for (int i = 0; i < 5; i++) new Decor(getPosition(i, 5), 1000, Track(2.9), THouse1);
		new Forest(0, 100, Track(1));
		new Road(0, 0, Track(0));
		new Decor(End.x + 1500, 250, Track(0.5), TDeleganse2);
		DlgBox* DlgWelcome = new DlgBox(TMap, camera.getSize().x / 2, camera.getSize().y - 150, 0);
		DlgBox* DlgWin = new DlgBox(TWin, camera.getSize().x / 2, camera.getSize().y , 0);
		DlgBox* DlgLose = new DlgBox(TLose, camera.getSize().x / 2, camera.getSize().y - 200, 0);
		new Samurai(0, 100, Track(0));
		(new Samurai(0, 100, Track(0)))->SetDirection(-1);
		(new Mage(0, 500, Track(0)))->SetDirection(-1);
		for (int i = 0; i < 2; i++) new Samurai2(getPosition(i, 2), 25, Track(0));
		new Boss(End.x - 1000, 90, Track(0));
		new Decor(Begin.x - 1500, 70, Track(0), TDeleganse);
		new Decor(End.x + 600, 90, Track(0), TBonefire, 5, 5, 0, 0.2);
		Player* player = new Player(Begin.x, 90, Track(0));
		camera.SetCadr(player->Locate.x, player->Locate.y);
		new Decor(Begin.x - 400, 90, Track(0), TBonefire, 5, 5, 0, 0.2);
		player->TurnTo(new Wifi(Begin.x - 800, 90, Track(0)));
		for (int i = 0; i < countOfHimers-1; i++) new Himera(getPosition(i, countOfHimers-1), 10, Track(0));
		for (int i = 0; i < countOfHimers+1; i++) new Himera2(getPosition(i, countOfHimers+1), 10, Track(0));
		for (int i = 0; i < 3; i++) new Bonefire(getPosition(i, 3), 25, Track(0));
		new LiveBar(210, 60, 0);
		Message Buff;
		Event event;
		MsgBox(DlgWelcome, &camera);
		if (player->Run(&camera) == rs_OK) isRepeat = (MsgBox(DlgWin, &camera) == rs_OK);
		else isRepeat = (MsgBox(DlgLose, &camera) == rs_OK);
		characters.clear();
		Messager::clear();
	} while (isRepeat);
}

