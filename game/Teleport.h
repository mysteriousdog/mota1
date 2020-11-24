#ifndef __TELEPORT_H__
#define __TELEPORT_H__

#include "MTGame.h"
#include "scripting/deprecated/CCDictionary.h"

using namespace cocos2d;

class Teleport : public Ref
{
public:
	Teleport(Map<std::string, std::string> *dict, int x, int y);
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
	std::string imagePath;
	Sprite *teleportSprite;
};
#endif