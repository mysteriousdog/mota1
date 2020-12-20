#include "GameScene.h"
#include "../Def/FileDef.h"
#include "../file/CsvData.h"

GameScene::GameScene(void)
{
	sGlobal->gameScene = this;
}

GameScene::~GameScene(void)
{
}

bool GameScene::init()
{
	// load all the file
	sParseCsv->LoadCsv(MUSIC_FILE_NAME, MUSIC_FILE_PATH);
	//�½�һ��GameLayerʵ��
	GameLayer *gamelayer = GameLayer::create();
	//��GameLayerʵ����ӵ�������
	this->addChild(gamelayer, kGameLayer, kGameLayer);
	//�½�һ��ControlLayerʵ��
	ControlLayer *controlLayer = ControlLayer::create();
	//��ControlLayerʵ����ӵ�������
	this->addChild(controlLayer, kControlLayer, kControlLayer);
	//int audioID = cocos2d::AudioEngine::play2d("musics/bgm.m4a", true);
	// std::string filePath;
	// if (sParseCsv->GetCsvData(MUSIC_FILE_NAME, BGM_FLOOR, filePath)) {
	// 	std::cout<<"==========> "<<"filePath: "<<filePath<<std::endl;
	// 	int audioID = cocos2d::AudioEngine::play2d(filePath, true);
	// 	// ����bgm��Ӧ��ID
	// 	sMusic->SetMusicMap(BGM_FLOOR, audioID);
	// }
	sMusic->PLayMusic(BGM_FLOOR);
	return true;
}

Scene* GameScene::playNewGame()
{
	GameScene * scene = NULL;
	do 
    {
		//����Ϸ����ǰ��ͼ����Ϊ0
		sGlobal->currentLevel = 2;
		//��ʿ����λ��
		sGlobal->heroSpawnTileCoord = Vec2(6, 12);
		scene = GameScene::create();
		CC_BREAK_IF(! scene);
	} while (0);
	return scene;
}
//�л���Ϸ��ͼ֮ǰ
void GameScene::switchMap()
{
	//����һ�����ֲ㣬���ڵ�ͼ�л�ʱ����ʾ���뵭��Ч��
	auto fadeLayer = LayerColor::create(Color4B(0, 0, 0, 0));
	fadeLayer->setAnchorPoint(Vec2(0, 0));
	fadeLayer->setPosition(Vec2(0, 0));
	addChild(fadeLayer, kFadeLayer, kFadeLayer);
	//ִ�е��붯�������������resetGameLayer����
	auto action = Sequence::create(
		FadeIn::create(0.5f),
		//CC_CALLBACK_1(GameScene::resetGameLayer, this),
		CallFuncN::create(CC_CALLBACK_1(GameScene::resetGameLayer, this)),
		//CCCallFunc::actionWithTarget(this, callfunc_selector(GameScene::resetGameLayer)),
		NULL);
	fadeLayer->runAction(action);
}
//�л���Ϸ��ͼ
void GameScene::resetGameLayer(Ref* Spender)
{
	//ɾ���ϵ�GameLayer
	this->removeChildByTag(kGameLayer, true);
	//�����µ�GameLayer
	GameLayer *gamelayer = GameLayer::create();
	this->addChild(gamelayer, kGameLayer, kGameLayer);
	//���ֲ�ִ�е���Ч���������󣬵���removeFadeLayer����ɾ�����ֲ�
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