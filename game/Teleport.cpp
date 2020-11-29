#include "Teleport.h"

Teleport::Teleport(ValueMap *dict, int x, int y)
{
	auto position = Vec2(x, y);
	//���͵����ڵ�TileMapλ��
	tileCoord = sGlobal->gameMap->tileCoordForPosition(Vec2(x, y));
	//�ó���ʿ��Ŀ������ʼλ��
	std::string key = "heroTileCoordX";
	//int x1 = dict->objectForKey(key)->toInt();
	int x1 =  dict->at(key).asInt();
	key = "heroTileCoordY";
	//int y1 = dict->objectForKey(key)->toInt();
	int y1 = dict->at(key).asInt();
	heroTileCoord = Vec2(x1, y1);
	//ȡ��Ŀ���ͼ�Ĳ���
	key = "targetMap";
	//targetMap = dict->objectForKey(key)->toInt();
	targetMap = dict->at(key).asInt();
	//��ȡimage��
	key = "image";
	//imagePath = dict->objectForKey(key);
	imagePath = dict->at(key).asString();
	//����������ʾTeleport�ľ���
	//teleportSprite = Sprite::createWithSpriteFrameName(imagePath);
	teleportSprite = Sprite::create(imagePath);
	//spriteWithFile(imagePath->m_sString.asInt());
	teleportSprite->setAnchorPoint(Vec2(0, 0));
	teleportSprite->setPosition(position);
	sGlobal->gameLayer->addChild(teleportSprite, kZTeleport);
}

Teleport::~Teleport(void)
{
}