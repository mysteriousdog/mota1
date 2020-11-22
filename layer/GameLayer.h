#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "cocos2d.h"
#include "MTGame.h"

using namespace cocos2d;

class GameMap;
class Hero;

class GameLayer : public Layer
{
public:
	GameLayer(void);
	~GameLayer(void);

	virtual bool init();

	void update(float dt);
	void setSceneScrollPosition(Point position);
	//��ʾtip
	void showTip(const char *tip, Point position);

	LAYER_NODE_FUNC(GameLayer);
protected:
	GameMap *map;
	Hero *hero;
	void onShowTipDone(Ref* pSender);
};

#endif