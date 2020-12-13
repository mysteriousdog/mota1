#include "GameScene.h"

GameScene::GameScene(void)
{
	sGlobal->gameScene = this;
}

GameScene::~GameScene(void)
{
}

bool GameScene::init()
{
	//新建一个GameLayer实例
	GameLayer *gamelayer = GameLayer::create();
	//将GameLayer实例添加到场景中
	this->addChild(gamelayer, kGameLayer, kGameLayer);
	//新建一个ControlLayer实例
	ControlLayer *controlLayer = ControlLayer::create();
	//将ControlLayer实例添加到场景中
	this->addChild(controlLayer, kControlLayer, kControlLayer);
	int audioID = cocos2d::AudioEngine::play2d("musics/bgm.m4a", true);
	sMusic->SetMusicMap("bgm", audioID);
	auto m = new Music();
	m->SetMusicMap("bgm", audioID);
	return true;
}

Scene* GameScene::playNewGame()
{
	GameScene * scene = NULL;
	do 
    {
		//新游戏，当前地图层数为0
		sGlobal->currentLevel = 2;
		//勇士出生位置
		sGlobal->heroSpawnTileCoord = Vec2(6, 12);
		scene = GameScene::create();
		CC_BREAK_IF(! scene);
	} while (0);
	return scene;
}
//切换游戏地图之前
void GameScene::switchMap()
{
	//创建一个遮罩层，用于地图切换时的显示淡入淡出效果
	auto fadeLayer = LayerColor::create(Color4B(0, 0, 0, 0));
	fadeLayer->setAnchorPoint(Vec2(0, 0));
	fadeLayer->setPosition(Vec2(0, 0));
	addChild(fadeLayer, kFadeLayer, kFadeLayer);
	//执行淡入动画，结束后调用resetGameLayer方法
	auto action = Sequence::create(
		FadeIn::create(0.5f),
		//CC_CALLBACK_1(GameScene::resetGameLayer, this),
		CallFuncN::create(CC_CALLBACK_1(GameScene::resetGameLayer, this)),
		//CCCallFunc::actionWithTarget(this, callfunc_selector(GameScene::resetGameLayer)),
		NULL);
	fadeLayer->runAction(action);
}
//切换游戏地图
void GameScene::resetGameLayer(Ref* Spender)
{
	//删除老的GameLayer
	this->removeChildByTag(kGameLayer, true);
	//创建新的GameLayer
	GameLayer *gamelayer = GameLayer::create();
	this->addChild(gamelayer, kGameLayer, kGameLayer);
	//遮罩层执行淡出效果，结束后，调用removeFadeLayer方法删除遮罩层
	auto action = Sequence::create(
		FadeOut::create(0.5f),
		//CC_CALLBACK_1(GameScene::removeFadeLayer, this),
		CallFuncN::create(CC_CALLBACK_1(GameScene::removeFadeLayer, this)),
		//CCCallFunc::actionWithTarget(this, callfunc_selector(GameScene::removeFadeLayer)),
		NULL);
	this->getChildByTag(kFadeLayer)->runAction(action);
}

void GameScene::removeFadeLayer(Ref* Spender)
{
	this->removeChildByTag(kFadeLayer, true);
}