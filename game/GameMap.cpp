#include "GameMap.h"

GameMap::GameMap(void)
{
	sGlobal->gameMap = this;
}

GameMap::~GameMap(void)
{
	this->unscheduleAllSelectors();
	CC_SAFE_RELEASE(enemyArray)
	CC_SAFE_RELEASE(npcDict)
	CC_SAFE_RELEASE(teleportDict)
}

//��̬��������������GameMapʵ��
GameMap* GameMap::gameMapWithTMXFile(const char *tmxFile)
{
	//newһ������
	GameMap *pRet = new GameMap();
	//����init����
	if (pRet->initWithTMXFile(tmxFile))
	{
		//���ö����init����
		pRet->extraInit();
		//��ʵ������autorelease�أ�ͳһ��������ƶ������������
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//TliedMap����ĳ�ʼ������
void GameMap::extraInit()
{
	//��������ͼ������������
	enableAnitiAliasForEachLayer();
	//��ʼ���������
	floorLayer = this->layerNamed("floor");
	wallLayer = this->layerNamed("wall");
	itemLayer = this->layerNamed("item");
	doorLayer = this->layerNamed("door");

	initEnemy();
	initObject();
}

void GameMap::initEnemy()
{
	//��ȡ�����
	enemyLayer = this->layerNamed("enemy");
	Size s = enemyLayer->getLayerSize();
	enemyArray = new Vector<Enemy*>();
	int index;
	//����enemy�㣬�����ڵĹ��ﱣ�浽������
	for (int x = 0; x < s.width; x++) {
		for (int y = 0; y < s.height; y++) {
			int gid = enemyLayer->tileGIDAt(ccp(x, y));
			if (gid != 0) {
				Enemy* enemy = new Enemy();
				//�����������
				enemy->position = ccp(x, y);
				//���������ʼ��ͼ��ID
				enemy->startGID = gid;
				//���ӹ����������
				enemyArray->addObject(enemy);
			}
		}
	}
	//���ڸ��µ��˶���
	schedule(schedule_selector(GameMap::updateEnemyAnimation), 0.2f);
}

//���¹����ͼ��
void GameMap::updateEnemyAnimation(float dt)
{	
	Vector<Enemy*>::CCMutableArrayIterator  iter;
	Enemy *enemy, *needRemove = NULL;
	//�����������й�����������
	for (iter = enemyArray->begin(); iter != enemyArray->end(); ++iter) {
		enemy = *iter;
		if (enemy != NULL) {
			//��ȡ���ﵱǰ��ͼ��ID
			int gid = enemyLayer->tileGIDAt(enemy->position);
			//������ﱻɾ���ˣ���Ҫ������enemyArray��Ҳɾ��
			if (gid == 0)
			{
				needRemove = enemy;
				continue;
			}
			gid++;
			//�����һ��ͼ��ID ��ʼͼ��ID
			if (gid - enemy->startGID > 3) {
				gid = enemy->startGID;
			}
			//�����������µ�ͼ��
			enemyLayer->setTileGID(gid, enemy->position);
		}
	}
	//ɾ��������Ĺ���
	if (needRemove != NULL) {
		enemyArray->removeObject(needRemove, true);
	}
}
//��ʼ�������
void GameMap::initObject()
{
	//��ʼ�������ź�npc����
	teleportDict = new Map<int, Teleport*>();
	npcDict = new Map<int, NPC*>();
	//��ȡ�����
	CCTMXObjectGroup* group = this->objectGroupNamed("object");
	//��ȡ������ڵ����ж���
	CCMutableArray<CCStringToStringDictionary*> * objects = group->getObjects();
	CCStringToStringDictionary* dict;
	CCMutableArray<CCStringToStringDictionary*>::CCMutableArrayIterator it;
	//�������ж���
	for( it = objects->begin(); it != objects->end(); it++) 
	{
		dict = (*it);
		if(!dict)
			break;
		std::string key = "x";
		//��ȡx����
		int x = dict->objectForKey(key)->toInt();
		key = "y";
		//��ȡy����
		int y = dict->objectForKey(key)->toInt();
		Point tileCoord = tileCoordForPosition(ccp(x, y));
		//����ΨһID
		int index = tileCoord.x + tileCoord.y * this->getMapSize().width;
		key = "type";
		//��ȡ�������
		CCString *type = dict->objectForKey(key);
		//��������Ǵ�����
		if (type->m_sString == "teleport"){
			Teleport *teleport = new Teleport(dict, x, y);
			teleportDict->setObject(teleport, index);
		}
		//���������NPC����
		else if (type->m_sString == "npc"){
			NPC *npc = new NPC(dict, x, y);
			npcDict->setObject(npc, index);
		}
	}
}

void GameMap::enableAnitiAliasForEachLayer()
{
	CCArray * pChildrenArray = this->getChildren();
	CCSpriteBatchNode* child = NULL;
	CCObject* pObject = NULL;
	//����Tilemap������ͼ��
	CCARRAY_FOREACH(pChildrenArray, pObject)
	{
		child = (CCSpriteBatchNode*)pObject;
		if(!child)
			break;
		//��ͼ����������������
		child->getTexture()->setAntiAliasTexParameters();
	}
}

//��cocos2d-x����ת��ΪTilemap����
Point GameMap::tileCoordForPosition(Point position)
{
	int x = position.x / this->getTileSize().width;
	int y = (((this->getMapSize().height - 1) * this->getTileSize().height) - position.y) / this->getTileSize().height;
	return Vec2(x, y);
}

//��Tilemap����ת��Ϊcocos2d-x����
Point GameMap::positionForTileCoord(Point tileCoord)
{
	Point pos =  ccp((tileCoord.x * this->getTileSize().width),
		((this->getMapSize().height - tileCoord.y - 1) * this->getTileSize().height));
	return pos;
}

//���صذ��
TMXLayer* GameMap::getFloorLayer()
{
	return floorLayer;
}
//����ǽ�ڲ�
TMXLayer* GameMap::getWallLayer()
{
	return wallLayer;
}
//���ع����
TMXLayer* GameMap::getEnemyLayer()
{
	return enemyLayer;
}
//������Ʒ��
TMXLayer* GameMap::getItemLayer()
{
	return itemLayer;
}
//�����Ų�
TMXLayer* GameMap::getDoorLayer()
{
	return doorLayer;
}

//���¹���ս��ʱ����ɫ״̬
void GameMap::updateEnemyHitEffect(ccTime dt)
{
	//���´�����һ
	fightCount++;
	if (fightCount % 2 == 1) {
		//���ù���sprite��ɫΪ��ɫ
		fightingEnemy->setColor(ccWHITE);
	} else {
		//���ù���sprite��ɫΪ��ɫ
		fightingEnemy->setColor(ccRED);
	}

	//�л�5�κ�ȡ����ʱ��
	if (fightCount == 5)
	{
		unschedule(schedule_selector(GameMap::updateEnemyHitEffect));
	}
}
//��ʾ����������
void GameMap::showEnemyHitEffect(Point tileCoord)
{
	//���´���
	fightCount = 0;
	//��ȡ�����Ӧ��CCSprite����
	fightingEnemy = enemyLayer->tileAt(tileCoord);
	if (fightingEnemy == NULL)
		return;
	//���ù���sprite��ɫΪ��ɫ
	fightingEnemy->setColor(ccRED);
	//������ʱ�����´��״̬
	this->schedule(schedule_selector(GameMap::updateEnemyHitEffect), 0.18f);
}