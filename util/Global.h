#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "MTGame.h"

using namespace cocos2d;

class GameScene;
class GameLayer;
class GameMap;
class Hero;
class ControlLayer;

class Global : public Singleton<Global>
{
public:
	Global(void);
	~Global(void);

	GameScene *gameScene;
	//��Ϸ��ͼ��
	GameLayer *gameLayer;
	ControlLayer *controlLayer;
	//��Ϸ��ͼ
	GameMap *gameMap;
	//��ʿ
	Hero *hero;
	//Ŀ���ͼ�Ĳ���
	int currentLevel;
	//��ʿ���ֵ���ʼλ��
	Point heroSpawnTileCoord;
};

#define sGlobal Global::instance()

#endif