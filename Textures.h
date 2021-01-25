#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class FileTexture : public Texture
{
public:
	FileTexture(char fileName[]) : Texture()
	{
		loadFromFile(fileName);
	};
};

FileTexture* TForest = new FileTexture("images/Forest.png");
FileTexture* TMoon = new FileTexture("images/Mooon.jpg");
FileTexture* TNinjaGo = new FileTexture("images/ninjaGo.png");
FileTexture* THouse1 = new FileTexture("images/House1.png");
FileTexture* THouse2 = new FileTexture("images/House2.png");
FileTexture* THouse3 = new FileTexture("images/House3.png");
FileTexture* THouse4 = new FileTexture("images/House4.png");
FileTexture* THills = new FileTexture("images/Hills.png");
FileTexture* TRoad = new FileTexture("images/Road.png");
FileTexture* Mountain = new FileTexture("images/Mountain2.png");
FileTexture* TMage = new FileTexture("images/Mage.png");
FileTexture* TSamuraiWalk = new FileTexture("images/SamuraiWalk.png");
FileTexture* TSamuraiStay = new FileTexture("images/SamuraiStay.png");
FileTexture* TSamurai2Attack = new FileTexture("images/StrelokAttack.png");
FileTexture* TSamurai2Stay = new FileTexture("images/StrelokStay.png");
FileTexture* THimeraStay = new FileTexture("images/HimeraStay.png");
FileTexture* THimeraAttackTail = new FileTexture("images/HimeraAttakcTail.png");	
FileTexture* THimeraWalk = new FileTexture("images/HimeraWalk.png");
FileTexture* THimeraAttack2 = new FileTexture("images/himeraAttack2.png");
FileTexture* THimeraWin = new FileTexture("images/HImeraWin.png");
FileTexture* THimeraDied = new FileTexture("images/HimeraDied.png");
FileTexture* TNinjaAttack = new FileTexture("images/ninjaAttack.png");
FileTexture* TNinjaStay = new FileTexture("images/ninjaStay.png");
FileTexture* TNinjaDie = new FileTexture("images/ninjaDie.png");
FileTexture* TBossAttack = new FileTexture("images/BossAttack.png");
FileTexture* TBossStay = new FileTexture("images/BossStay.png");
FileTexture* TBossDie = new FileTexture("images/BossDie.png");
FileTexture* TDeleganse = new FileTexture("images/Deleganse.png");
FileTexture* TDeleganse2 = new FileTexture("images/Deleganse2.png");
FileTexture* TDragon = new FileTexture("images/Dragon.png");
FileTexture* TWifiTolk = new FileTexture("images/WifiTolk.png");
FileTexture* TWifiSit = new FileTexture("images/WifiSit.png");
FileTexture* TBonefire = new FileTexture("images/bonefire.png");
FileTexture* TLifeBar = new FileTexture("images/LifeBar.png");
FileTexture* TMap = new FileTexture("images/papirus.png");
FileTexture* TWin = new FileTexture("images/Congratulation.png");
FileTexture* TLose = new FileTexture("images/Condolences.png");
//FileTexture* Moon = new FileTexture("images/Moon.jpg");
