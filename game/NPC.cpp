#include "NPC.h"

NPC::NPC(Map<std::string, std::string> *dict, int x, int y)
{
	//��ȡ����
	std::string key = "name";
	npcId = dict->at(key);
	//��ȡ����
	key = "type";
	type = dict->at(key);
	//��ȡimage��
	key = "image";
	imagePath = dict->at(key);
	//��ȡrectX��rectY
	key = "rectX";
	int x1 = atoi(dict->at(key).c_str());
	key = "rectY";
	int y1 = atoi(dict->at(key).c_str());
	rect = Rect(x1, y1, 32, 32);
	//positionΪcocos2d-x���꣬tileCoordΪTileMap����
	Vec2 position = Vec2(x, y);
	tileCoord = sGlobal->gameMap->tileCoordForPosition(position);
	//����������ʾnpc�ľ���
	//npcSprite = Sprite::spriteWithFile(imagePath->m_sString.c_str(), rect);
	npcSprite = Sprite::createWithSpriteFrameName(imagePath);
	npcSprite->setAnchorPoint(Vec2(0, 0));
	npcSprite->setPosition(position);
	sGlobal->gameLayer->addChild(npcSprite, kZNPC);
	//�Ӷ����������и���npcId��ȡ��������ʼ���ò���
	Animate* animation = sAnimationMgr->createAnimate(npcId.c_str());
	if (animation != NULL) {
		auto action = RepeatForever::create(animation);
		npcSprite->runAction(action);
	}
}

NPC::~NPC(void)
{
	//�ͷ�CCString����
	CC_SAFE_RELEASE(npcId)
	CC_SAFE_RELEASE(imagePath)
	CC_SAFE_RELEASE(type)
}
