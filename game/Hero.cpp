﻿#include "Hero.h"

Hero::Hero()
{
	sGlobal->hero = this;
}

Hero::~Hero()
{

}

//静态方法，用于生成Hero实例
Hero* Hero::heroWithinLayer()
{
	//new一个对象
	Hero *pRet = new Hero();
	//调用init方法
	if (pRet && pRet->heroInit())
	{
		//将实例放入autorelease池，统一由引擎控制对象的生命周期
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool Hero::heroInit()
{
	//根据向下行走动画的第一帧创建精灵
	//heroSprite = CCSprite::spriteWithSpriteFrame(sAnimationMgr->getAnimation(kDown)->getFrames()->getObjectAtIndex(0));
	// char temp[20];
	// sprintf(temp, "%d", kDown);
	CCLOG("th size of kdown animation arrays is :%d\n", sAnimationMgr->getAnimation(kDown)->getFrames().size());
	heroSprite = Sprite::createWithSpriteFrame(sAnimationMgr->getAnimation(kDown)->getFrames().at(0)->getSpriteFrame());
	//设置锚点
	heroSprite->setAnchorPoint(Vec2(0, 0));
	//将用于显示的heroSprite加到自己的节点下
	this->addChild(heroSprite);
	//创建空的战斗动画精灵
	fightSprite = new Sprite();
	//手动设置fightSprite为autorelease
	fightSprite->autorelease();
	this->addChild(fightSprite);
	//一开始不处于move状态。
	isHeroMoving = false;
	isHeroFighting = false;
	isDoorOpening = false;
	return true;
}

void Hero::move(HeroDirection direction) 
{
	if (isHeroMoving)
		return;

	//移动的距离
	Point moveByPosition;
	//根据方向计算移动的距离
	switch (direction)
	{
	case kDown:
		moveByPosition = Vec2(0, -32);
		break;
	case kLeft:
		moveByPosition = Vec2(-32, 0);
		break;
	case kRight:
		moveByPosition = Vec2(32, 0);
		break;
	case kUp:
		moveByPosition = Vec2(0, 32);
		break;
	}
	//计算目标坐标，用当前勇士坐标加上移动距离
	targetPosition = this->getPosition() + moveByPosition;
	//调用checkCollision检测碰撞类型，如果是墙壁、怪物、门，则只需要设置勇士的朝向
	CollisionType collisionType = checkCollision(targetPosition);
	CCLOG("the collisiontype is %d\n", collisionType);
	if (collisionType == kWall || collisionType == kEnemy || collisionType == kDoor || collisionType == kNPC)
	{
		setFaceDirection((HeroDirection)direction);
		return;
	}
	//heroSprite仅播放行走动画
	heroSprite->runAction(sAnimationMgr->createAnimate(direction));
	CCLOG("hero run action done, the direction is: %d\n", direction);
	//主体进行位移，结束时调用onMoveDone方法
	auto action = Sequence::create(
		MoveBy::create(0.20f, moveByPosition),
		//把方向信息传递给onMoveDone方法
		//CC_CALLBACK_2(Hero::onMoveDone, this, (void*)direction),
		//CC_CALLBACK_1(Hero::onMoveDone, this),
		//CallFunc::create(this, callfuncND_selector(Hero::onMoveDone), (void*)direction),
		//CallFunc::create(CC_CALLFUNC_SELECTOR(Hero::onMoveDone)),
		CallFuncN::create(CC_CALLBACK_1(Hero::onMoveDone, this, (void*)direction)),
		NULL);
	//auto action = MoveBy::create(0.20f, moveByPosition);
	this->runAction(action);
	isHeroMoving = true;
}

void Hero::onMoveDone(Node* pTarget, void* data)
{
	//将void*先转换为int，再从int转换到枚举类型
	//int direction = reinterpret_cast<int>(data);
	CCLOG("onMoveDown end!\n");
	uintptr_t direction = reinterpret_cast<uintptr_t>(data);
	CCLOG("the trans direction is %l\n", direction);
	setFaceDirection((HeroDirection)direction);
	isHeroMoving = false;
	sGlobal->gameLayer->setSceneScrollPosition(this->getPosition());
}

void Hero::setFaceDirection(HeroDirection direction)
{
	heroSprite->setTextureRect(Rect(0,32*direction,32,32));
	CCLOG("set face direction done! now face direction is: %d\n", direction);
}

//判断碰撞类型
CollisionType Hero::checkCollision(Point heroPosition) 
{
	//cocos2d-x坐标转换为Tilemap坐标
	targetTileCoord = sGlobal->gameMap->tileCoordForPosition(heroPosition);
	//如果勇士坐标超过地图边界，返回kWall类型，阻止其移动
	if (heroPosition.x < 0 || targetTileCoord.x > sGlobal->gameMap->getMapSize().width - 1 || targetTileCoord.y < 0 || targetTileCoord.y > sGlobal->gameMap->getMapSize().height - 1)
		return kWall;
	//获取墙壁层对应坐标的图块ID
	int targetTileGID = sGlobal->gameMap->getWallLayer()->getTileGIDAt(targetTileCoord);
	//如果图块ID不为0，表示有墙
	if (targetTileGID) {
		return kWall;
	}
	//获取怪物层对应坐标的图块ID
	targetTileGID = sGlobal->gameMap->getEnemyLayer()->getTileGIDAt(targetTileCoord);
	//如果图块ID不为0，表示有怪物
	if (targetTileGID) {
		//开始战斗
		fight();
		return kEnemy;
	}
	//获取物品层对应坐标的图块ID
	targetTileGID = sGlobal->gameMap->getItemLayer()->getTileGIDAt(targetTileCoord);
	//如果图块ID不为0，表示有物品
	if (targetTileGID) {
		//拾取物品
		pickUpItem();
		return kItem;
	}
	//获取门层对应坐标的图块ID
	targetTileGID = sGlobal->gameMap->getDoorLayer()->getTileGIDAt(targetTileCoord);
	//如果图块ID不为0，表示有门
	if (targetTileGID) {
		//打开门
		openDoor(targetTileGID);
		return kDoor;
	}
	//从npc字典中查询
	int index = targetTileCoord.x + targetTileCoord.y * sGlobal->gameMap->getMapSize().width;
	NPC *npc = sGlobal->gameMap->npcDict->at(index);
	if (npc != NULL)
	{
		actWithNPC();
		return kNPC;
	}
	//从Teleport字典中查询
	Teleport *teleport = sGlobal->gameMap->teleportDict->at(index);
	if (teleport != NULL)
	{
		doTeleport(teleport);
		return kTeleport;
	}
	//可以通行
	return kNone;
}

//开始战斗
void Hero::fight()
{
	//已经在战斗中，避免重复战斗
	if (isHeroFighting)
		return;
	isHeroFighting = true;
	//显示怪物受到打击的效果
	sGlobal->gameMap->showEnemyHitEffect(targetTileCoord);
	//显示损失的生命值，先用假数据替代一下
	char temp[30] = {0};
	sprintf(temp, "lost hp: -%d", 100);
	sGlobal->gameLayer->showTip(temp, getPosition());
	//将用于显示战斗动画的精灵设置为可见
	fightSprite->setVisible(true);
	//计算显示战斗动画的位置为勇士和怪物的中间点
	auto pos = Vec2((targetPosition.x - getPosition().x) / 2 + 16, (targetPosition.y - getPosition().y) / 2 + 16);
	fightSprite->setPosition(pos);
	//创建战斗动画
	auto action = Sequence::create(
		sAnimationMgr->createAnimate(aFight),
		CC_CALLBACK_1(Hero::onFightDone, this),
		//CCCallFuncN::actionWithTarget(this, callfuncN_selector(Hero::onFightDone)),
		NULL);
	fightSprite->runAction(action);
}
//战斗结束的回调
void Hero::onFightDone(Ref* pSender)
{
	//删除怪物对应的图块，表示已经被消灭
	sGlobal->gameMap->getEnemyLayer()->removeTileAt(targetTileCoord);
	isHeroFighting = false;
}
//拾取物品
void Hero::pickUpItem()
{
	//显示提示消息
	sGlobal->gameLayer->showTip("get an item, hp +100", this->getPosition());
	//将物品从地图上移除
	sGlobal->gameMap->getItemLayer()->removeTileAt(targetTileCoord);
}
//打开门
void Hero::openDoor(int gid)
{
	//如果门正在被开启，则返回
	if (isDoorOpening)
		return;
	//保存正在被开启的门的初始GID
	targetDoorGID = gid;
	isDoorOpening = true;
	//定时器更新门动画
	schedule(CC_SCHEDULE_SELECTOR(Hero::updateOpenDoorAnimation), 0.1f);
}
//更新开门动画
void Hero::updateOpenDoorAnimation(float dt)
{
	//计算动画下一帧的图块ID，TileMap的图块编号方式是横向递增1，所以每列相同的位置的图块ID相差了每行图块的个数
	int nextGID = sGlobal->gameMap->getDoorLayer()->getTileGIDAt(targetTileCoord) + 4;
	//如果超过了第四帧动画，就将当前位置的图块删除，并取消定时器
	if (nextGID - targetDoorGID > 4 * 3) {
		sGlobal->gameMap->getDoorLayer()->removeTileAt(targetTileCoord);
		unschedule(CC_SCHEDULE_SELECTOR(Hero::updateOpenDoorAnimation));
		isDoorOpening = false;
	} else {
		//更新动画至下一帧
		sGlobal->gameMap->getDoorLayer()->setTileGID(nextGID, targetTileCoord);
	}
}
//与NPC交互
void Hero::actWithNPC()
{
	sGlobal->gameLayer->showTip("talking with npc", getPosition());
}
//传送点逻辑
void Hero::doTeleport(Teleport *teleport)
{
	//从传送点的属性中后去目标地图的层数
	sGlobal->currentLevel = teleport->targetMap;
	//获取勇士在新地图中的起始位置
	sGlobal->heroSpawnTileCoord = teleport->heroTileCoord;
	//开始切换游戏地图
	sGlobal->gameScene->switchMap();
}
