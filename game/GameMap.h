#ifndef __GAME_MAP_H__
#define __GAME_MAP_H__

#include "MTGame.h"

using namespace cocos2d;

class NPC;
class Teleport;

//�̳���CCTMXTIledMap
class GameMap : public TMXTiledMap
{
	//ֻ����������ȡ��ͼ��
	CC_PROPERTY_READONLY(TMXLayer*, floorLayer, FloorLayer);
	CC_PROPERTY_READONLY(TMXLayer*, wallLayer, WallLayer);
	CC_PROPERTY_READONLY(TMXLayer*, enemyLayer, EnemyLayer);
	CC_PROPERTY_READONLY(TMXLayer*, itemLayer, ItemLayer);
	CC_PROPERTY_READONLY(TMXLayer*, doorLayer, DoorLayer);
public:
	GameMap(void);
	~GameMap(void);
	//��̬��������������GameMapʵ��
	static GameMap* gameMapWithTMXFile(const char *tmxFile);
	//TiledMap��cocos2d-x����ϵ�໥ת���ķ���
	Point tileCoordForPosition(Point position);
	Point positionForTileCoord(Point tileCoord);
	void showEnemyHitEffect(Point tileCoord);
	//��ŵ�ͼ�Ϲ���������Լ�npc 
	Vector<Enemy*> *enemyArray;
	Map<int, Teleport*> *teleportDict;
	Map<int, NPC*> *npcDict;
protected:
	//TiledMap����ĳ�ʼ������
	void extraInit();
	//��ʼ����������
	void initEnemy();
	//��ʼ������
	void initObject();
	//������ͼ������������
	void enableAnitiAliasForEachLayer();
	//���¹��ﶯ��
	void updateEnemyAnimation(float dt);
	//��ʱ����ս��ʱ�Ĺ���
	Sprite* fightingEnemy;
	//��ʱ����������
	int fightCount;
	//���¹���ս��ʱ����ɫ״̬
	void updateEnemyHitEffect(float dt);
};

#endif