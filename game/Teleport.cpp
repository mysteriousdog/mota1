#include "Teleport.h"

Teleport::Teleport(Map<std::string, std::string> *dict, *dict, int x, int y)
{
	auto position = Vec2(x, y);
	//���͵����ڵ�TileMapλ��
	tileCoord = sGlobal->gameMap->tileCoordForPosition(Vec2(x, y));
	//�ó���ʿ��Ŀ������ʼλ��
	std::string key = "heroTileCoordX";
	//int x1 = dict->objectForKey(key)->toInt();
	int x1 = atoi(dict->at(key).c_str());
	key = "heroTileCoordY";
	//int y1 = dict->objectForKey(key)->toInt();
	int y1 = atoi(dict->at(key).c_str());
	heroTileCoord = Vec2(x1, y1);
	//ȡ��Ŀ���ͼ�Ĳ���
	key = "targetMap";
	//targetMap = dict->objectForKey(key)->toInt();
	targetMap = atoi(dict->at(key).c_str());
	//��ȡimage��
	key = "image";
	//imagePath = dict->objectForKey(key);
	imagePath = dict->at(key);
	//����������ʾTeleport�ľ���
	teleportSprite = Sprite::createWithSpriteFrameName(imagePath); 
	//spriteWithFile(imagePath->m_sString.c_str());
	teleportSprite->setAnchorPoint(Vec2(0, 0));
	teleportSprite->setPosition(position);
	sGlobal->gameLayer->addChild(teleportSprite, kZTeleport);
}

Teleport::~Teleport(void)
{
}