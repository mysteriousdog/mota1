#ifndef __TELEPORT_H__
#define __TELEPORT_H__

#include "MTGame.h"
#include "scripting/deprecated/CCDictionary.h"

using namespace cocos2d;

class Teleport : public Ref
{
public:
	Teleport(__Dictionary *dict, int x, int y);
	~Teleport(void);
	//���͵�����λ��
	Point tileCoord;
	//���͵�Ŀ������ʿ��������
	Point heroTileCoord;
	//Ŀ���ͼ�Ĳ���
	int targetMap;
	//Ψһ��ID
	int index;
	//ͼƬ�������ļ�·��
	__String *imagePath;
	Sprite *teleportSprite;
};
#endif