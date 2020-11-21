#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "cocos2d.h"

using namespace cocos2d;

class Enemy : public Ref
{
public:
	Enemy(void);
	~Enemy(void);
	//������TileMap�ϵķ�λ
	Point position;
	//�����ʼ��ͼ��ID
	int startGID;
	//�������ֵ��е�index
	int index;
};

#endif