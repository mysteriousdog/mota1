#include "NPC.h"

NPC::NPC(ValueMap *dict, int x, int y)
{
	//��ȡ����
	std::string key = "name";
	npcId = dict->at(key).asString();
	//��ȡ����
	key = "type";
	type = dict->at(key).asString();
	//��ȡimage��
	key = "image";
	imagePath = dict->at(key).asString();
	//��ȡrectX��rectY
	key = "rectX";
	int x1 = dict->at(key).asInt();
	key = "rectY";
	int y1 = dict->at(key).asInt();
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
	//CC_SAFE_RELEASE(npcId)
	//C_SAFE_RELEASE(imagePath)
	//CC_SAFE_RELEASE(type)
}
