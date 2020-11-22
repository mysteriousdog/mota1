#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

//��������ͷ�ļ�
#include "MTGame.h"
//ʹ��cocos2d�����ռ�
using namespace cocos2d;
//GameScene�̳�CCScene
class GameScene : public Scene
{
public:
	GameScene(void);
	~GameScene(void);
	//��̬�������ڴ����µ���Ϸ�������ʵ��
	static Scene *playNewGame();
	//��ʼ������
	virtual bool init();
	//Scene�ľ�̬��������
	SCENE_NODE_FUNC(GameScene);
	//
	void switchMap();
	void resetGameLayer();
	void removeFadeLayer();
};

#endif