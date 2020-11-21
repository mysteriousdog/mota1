#ifndef __HERO_H__
#define __HERO_H__

#include "MTGame.h"

using namespace cocos2d;

class Teleport;

//��ʿ��̳���CCNode
class Hero : public Node
{
public:
	Hero(void);
	~Hero(void);
	//��̬���������ڴ�����ʿʵ��
	static Hero *heroWithinLayer();
	//����ʿ��ָ�������ƶ�һ��
	void move(HeroDirection direction);
	//������ʿ����
	void setFaceDirection(HeroDirection direction);
	//��ʼս���߼�
	void fight();
	//��ʶ��ʿ�Ƿ����ƶ�״̬
	bool isHeroMoving;
	//��ʶ��ʿ�Ƿ���ս��״̬
	bool isHeroFighting;
	//��ʶ��ʿ�Ƿ��ڿ���״̬
	bool isDoorOpening;
	//ʰȡ��Ʒ
	void pickUpItem();
	//����
	void openDoor(int targetDoorGID);
	//��NPC����
	void actWithNPC();
	//����
	void doTeleport(Teleport *teleport);
protected:
	//������ʾ��ʿ����ľ���
	Sprite *heroSprite;
	//��ʱ����Ŀ���Tilemap����
	Point targetTileCoord;
	//��ʱ����Ŀ���cocos2d-x����
	Point targetPosition;
	//��ʱ��������ʼ��ͼ��ID
	int targetDoorGID;
	//��ʾս�������ľ���
	Sprite *fightSprite;
	//��ʼ������
	bool heroInit();
	//ս����ɺ�Ļص�����
	void onFightDone(Node* pTarget);
	//��ײ��ⷽ��
	CollisionType checkCollision(Point heroPosition);
	//�ƶ���ɺ�Ļص�����
	void onMoveDone(Node* pTarget, void* data);
	//���¿��Ŷ���
	void updateOpenDoorAnimation(float dt);
};

#endif