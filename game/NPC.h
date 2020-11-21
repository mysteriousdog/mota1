#ifndef __NPC_H__
#define __NPC_H__

#include "MTGame.h"
#include "scripting/deprecated/CCDictionary.h"

using namespace cocos2d;

class NPC : public Ref
{
public:
	//���캯����Ҫ���ݴ��ݵ����Ա���ʼ����������
	NPC(__Dictionary *dict, int x, int y);
	~NPC(void);
	//������ʾnpc�ľ���
	Sprite *npcSprite;
	//������TileMap�����õ�name��
	__String *npcId;
	//npc���ڵ�TileMap����
	Point tileCoord;
	//ͼƬ�������ļ�·��
	__String *imagePath;
	//������Rect
	Rect rect;
	//��Ӧ�����е�type��
	__String *type;
};

#endif