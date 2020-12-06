#include "GameMap.h"

GameMap::GameMap(void)
{
	sGlobal->gameMap = this;
}

GameMap::~GameMap(void)
{
	//this->unscheduleAllSelectors();
	//CC_SAFE_RELEASE(enemyArray);
	//CC_SAFE_RELEASE(npcDict);
	//CC_SAFE_RELEASE(teleportDict);
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
	floorLayer = this->getLayer("floor");
	wallLayer = this->getLayer("wall");
	itemLayer = this->getLayer("item");
	doorLayer = this->getLayer("door");

	initEnemy();
	initObject();
}

void GameMap::initEnemy()
{
	//��ȡ�����
	enemyLayer = this->getLayer("enemy");
	Size s = enemyLayer->getLayerSize();
	enemyArray = new Vector<Enemy*>();
	int index;
	//����enemy�㣬�����ڵĹ��ﱣ�浽������
	for (int x = 0; x < s.width; x++) {
		for (int y = 0; y < s.height; y++) {
			int gid = enemyLayer->getTileGIDAt(Vec2(x, y));
			if (gid != 0) {
				CCLOG("enemy cood => x,y: %d,%d \n",x,y);
				Enemy* enemy = new Enemy();
				//�����������
				enemy->position = Vec2(x, y);
				//���������ʼ��ͼ��ID
				enemy->startGID = gid;
				//���ӹ����������
				enemyArray->pushBack(enemy);
			}
		}
	}
	//���ڸ��µ��˶���
	schedule(CC_SCHEDULE_SELECTOR(GameMap::updateEnemyAnimation), 0.2f);
}

//���¹����ͼ��
void GameMap::updateEnemyAnimation(float dt)
{	
	Vector<Enemy*>::iterator  iter;
	Enemy *enemy, *needRemove = NULL;
	//�����������й�����������
	for (iter = enemyArray->begin(); iter != enemyArray->end(); ++iter) {
		enemy = *iter;
		if (enemy != NULL) {
			//��ȡ���ﵱǰ��ͼ��ID
			int gid = enemyLayer->getTileGIDAt(enemy->position);
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
		enemyArray->eraseObject(needRemove, true);
	}
}
//��ʼ�������
void GameMap::initObject()
{
	//��ʼ�������ź�npc����
	teleportDict = new Map<int, Teleport*>();
	npcDict = new Map<int, NPC*>();
	//��ȡ�����
	TMXObjectGroup* group = this->getObjectGroup("object");
	//��ȡ������ڵ����ж���
	auto objects = group->getObjects();
	//Vector<Map<std::string, std::string>*> objects = group->getObjects();
	ValueMap dict;
	//CCMutableArray<CCStringToStringDictionary*>::CCMutableArrayIterator it;
	//�������ж���
	for(auto it = objects.begin(); it != objects.end(); it++) 
	{
		dict = (*it).asValueMap();
		if(dict.empty())
			break;
		std::string key = "x";
		//��ȡx����
		int x = dict[key].asInt();
		key = "y";
		//��ȡy����
		int y = dict[key].asInt();
		CCLOG("x and y pixel: %d,%d\n",x,y);
		Point tileCoord = tileCoordForPosition(Vec2(x, y));
		
		//����ΨһID
		int index = tileCoord.x + tileCoord.y * this->getMapSize().width;
		CCLOG("point of object is x,y: %f, %f, index is : %d\n", tileCoord.x, tileCoord.y, index);
		key = "type";
		//��ȡ�������
		auto type = dict[key];
		//��������Ǵ�����
		if (type.asString() == "teleport"){
			Teleport *teleport = new Teleport(&dict, x, y);
			//teleportDict->setObject(teleport, index);
			//teleportDict[index] = teleport;
			teleportDict->insert(index, teleport);
		} else if (type.asString() == "npc"){
			NPC *npc = new NPC(&dict, x, y);
			npcDict->insert(index, npc);
			//npcDict->setObject(npc, index);
		}
	}
}

void GameMap::enableAnitiAliasForEachLayer()
{
	auto pChildrenArray = this->getChildren();
	SpriteBatchNode* child = NULL;
	Node* pObject = NULL;
	//����Tilemap������ͼ��
	//CCARRAY_FOREACH(pChildrenArray, pObject)
	for (auto it = pChildrenArray.begin(); it != pChildrenArray.end(); it++)
	{
		//child = (CCSpriteBatchNode*)pObject;
		child = static_cast<SpriteBatchNode*>(*it);
		if(!child) {
			break;
		}
		//��ͼ����������������
		child->getTexture()->setAntiAliasTexParameters();
	}
}

//��cocos2d-x����ת��ΪTilemap����
// pixed坐标 =》 tiled坐标	
Point GameMap::tileCoordForPosition(Point position)
{
	int x = position.x / this->getTileSize().width;
	int y = (((this->getMapSize().height - 1) * this->getTileSize().height) - position.y) / this->getTileSize().height;
	return Vec2(x, y);
}

//��Tilemap����ת��Ϊcocos2d-x����
// tiled 坐标 =》 pix 坐标
Point GameMap::positionForTileCoord(Point tileCoord)
{
	Point pos = Vec2((tileCoord.x * this->getTileSize().width),
				((this->getMapSize().height - tileCoord.y - 1) * this->getTileSize().height));
	return pos;
}

//���صذ��
TMXLayer* GameMap::getFloorLayer() const
{
	return floorLayer;
}
//����ǽ�ڲ�
TMXLayer* GameMap::getWallLayer() const
{
	return wallLayer;
}
//���ع����
TMXLayer* GameMap::getEnemyLayer() const
{
	return enemyLayer;
}
//������Ʒ��
TMXLayer* GameMap::getItemLayer() const
{
	return itemLayer;
}
//�����Ų�
TMXLayer* GameMap::getDoorLayer() const
{
	return doorLayer;
}

//���¹���ս��ʱ����ɫ״̬
void GameMap::updateEnemyHitEffect(float dt)
{
	//���´�����һ
	fightCount++;
	CCLOG("fightCount is %d\n", fightCount);
	if (fightCount % 2 == 1) {
		//���ù���sprite��ɫΪ��ɫ
		fightingEnemy->setColor(Color3B::WHITE);
	} else {
		//���ù���sprite��ɫΪ��ɫ
		fightingEnemy->setColor(Color3B::RED);
	}

	//�л�5�κ�ȡ����ʱ��
	if (fightCount == 5)
	{
		CCLOG("stop shining the enemy!\n");
		unschedule(CC_SCHEDULE_SELECTOR(GameMap::updateEnemyHitEffect));
		//删除怪物对应的图块，表示已经被消灭
		sGlobal->gameMap->getEnemyLayer()->removeTileAt(sGlobal->hero->targetTileCoord);
		sGlobal->hero->isHeroFighting = false;
	}
}
//��ʾ����������
void GameMap::showEnemyHitEffect(Point tileCoord)
{
	//���´���
	fightCount = 0;
	//��ȡ�����Ӧ��CCSprite����
	fightingEnemy = enemyLayer->getTileAt(tileCoord);
	if (fightingEnemy == NULL)
		return;
	//���ù���sprite��ɫΪ��ɫ
	fightingEnemy->setColor(Color3B::RED);
	//������ʱ�����´��״̬
	CCLOG("start to shining the enemy\n");
	this->schedule(CC_SCHEDULE_SELECTOR(GameMap::updateEnemyHitEffect), 0.18f);
}