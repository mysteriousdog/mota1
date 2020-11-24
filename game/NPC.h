#ifndef __NPC_H__
#define __NPC_H__

#include "MTGame.h"
#include "scripting/deprecated/CCDictionary.h"

using namespace cocos2d;

class NPC : public Ref
{
public:
	//���캯����Ҫ���ݴ��ݵ����Ա���ʼ����������
	NPC(Map<std::string, std::string> *dict, int x, int y);
	~NPC(void);
	//������ʾnpc�ľ���
	Sprite *npcSprite;
	//������TileMap�����õ�name��
	std::string npcId;
	//npc���ڵ�TileMap����
	Point tileCoord;
	//ͼƬ�������ļ�·��
	std::string imagePath;
	//������Rect
	Rect rect;
	//��Ӧ�����е�type��
	std::string type;
};

#endif