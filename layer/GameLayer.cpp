#include "GameLayer.h"

GameLayer::GameLayer(void)
{
	sGlobal->gameLayer = this;
}

GameLayer::~GameLayer(void)
{
	this->unscheduleAllCallbacks();
}

bool GameLayer::init()
{
    bool bRet = false;
    do 
    {
		//解析tmx地图
		char temp[20];
		sprintf(temp, "%d.tmx", sGlobal->currentLevel);
		map = GameMap::gameMapWithTMXFile(temp);
		addChild(map, kZMap, kZMap);

		//调用Hero类的静态方法创建实例
		hero = Hero::heroWithinLayer();
		//设置Hero的起始位置
		hero->setPosition(map->positionForTileCoord(sGlobal->heroSpawnTileCoord));
		//将Hero加入GameLayer
		addChild(hero, kZHero, kZHero);

		schedule(CC_SCHEDULE_SELECTOR(GameLayer::update));

        bRet = true;
    } while (0);

    return bRet;
}

void GameLayer::update(float dt)
{
	//如果勇士不在行走状态，不需要更新场景位置
	if (hero->isHeroMoving)
	{
		setSceneScrollPosition(hero->getPosition());
	}
}

//传入勇士当前位置信息，将场景移动到相应位置
void GameLayer::setSceneScrollPosition(Point position)
{
	CCLOG("position = > x,y: %f, %f\n", position.x, position.y);
	//获取屏幕尺寸
	//Size screenSize = CCDirector::sharedDirector()->getWinSize();
	auto screenSize = Director::getInstance()->getWinSize();
	CCLOG("screenSize = > x,y: %f, %f\n", screenSize.width, screenSize.height);
	//计算Tilemap的宽高，单位是像素
	auto mapSizeInPixel = Size(map->getMapSize().width * map->getTileSize().width, 
		map->getMapSize().height * map->getTileSize().height);
	CCLOG("mapSize = > x,y: %f, %f\n", mapSizeInPixel.width, mapSizeInPixel.height);
	//取勇士当前x坐标和屏幕中点x的最大值，如果勇士的x值较大，则会滚动
	float x = MAX(position.x, screenSize.width / 2.0f);
	float y = MAX(position.y, screenSize.height / 2.0f);
	CCLOG("max x and y  = > x,y: %f, %f\n", x, y);
	//地图总宽度大于屏幕宽度的时候才有可能滚动
	if (mapSizeInPixel.width  > screenSize.width)
	{
		//场景滚动的最大距离不能超过地图总宽减去屏幕宽的1/2
		x = MIN(x, mapSizeInPixel.width - screenSize.width / 2.0f);
	}
	if (mapSizeInPixel.height > screenSize.height)
	{
		y = MIN(y, mapSizeInPixel.height - screenSize.height / 2.0f);
	}
	//勇士的实际位置
	Point heroPosition = Vec2(x, y);
	//屏幕中点位置
	Point screenCenter = Vec2(screenSize.width/2.0f, screenSize.height/2.0f);
	//计算勇士实际位置和重点位置的距离
	Point scrollPosition = screenCenter - heroPosition;
	//将场景移动到相应位置
	this->setPosition(scrollPosition);
	CCLOG("print the screen move position!\n");
	CCLOG("%f,%f\n", screenCenter.x, screenCenter.y);
	CCLOG("%f,%f\n", heroPosition.x, heroPosition.y);
	CCLOG("%f,%f\n", scrollPosition.x, scrollPosition.y);
}

//显示提示信息
void GameLayer::showTip(const char *tip, Point startPosition)
{
	//新建一个文本标签
	//auto tipLabel = Label::createWithTTF(tip, "fonts/arial.ttf", 20);
	auto tipLabel = Label::createWithSystemFont(tip, "Arial", 20);
	tipLabel->setPosition(startPosition + Vec2(16, 16));
	this->addChild(tipLabel, kZTip, kZTip);
	//定义动画效果
	auto action = Sequence::create(
		MoveBy::create(0.5f, Vec2(0, 32)),
		DelayTime::create(0.5f),
		FadeOut::create(0.2f),
		//CC_CALLBACK_1(GameLayer::onShowTipDone, this),
		CallFuncN::create(CC_CALLBACK_1(GameLayer::onShowTipDone, this)),
		//CCCallFuncN::actionWithTarget(this, callfuncN_selector(GameLayer::onShowTipDone)),
		NULL);
	tipLabel->runAction(action);
}

//提示消息显示完后的回调
void GameLayer::onShowTipDone(Ref* pSender)
{
	//删掉文本标签
	CCLOG("remove from parent\n");
	//this->getChildByTag(kZTip)->setVisible(false);
	this->getChildByTag(kZTip)->removeFromParentAndCleanup(true);
	if (sGlobal->hero->isTalking) {
		CCLOG("now the talk is over!!\n");
		sGlobal->hero->isTalking = false;
	}
}