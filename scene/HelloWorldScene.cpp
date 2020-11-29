 #include "HelloWorldScene.h"

using namespace cocos2d;

HelloWorld::HelloWorld()
{

}

HelloWorld::~HelloWorld(void)
{
	for (int i = 0; i < 4; i++)
	{
		//�ͷ�������Ԫ��
		CC_SAFE_RELEASE(walkAnimation[i]);
	}
	//�ͷ����鱾��
	CC_SAFE_DELETE_ARRAY(walkAnimation);

	//this->unscheduleAllSelectors();
	this->unscheduleAllCallbacks();
}

Scene* HelloWorld::scene()
{
    Scene * scene = NULL;
    do 
    {
        scene = Scene::create();
        CC_BREAK_IF(! scene);

        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        scene->addChild(layer);
    } while (0);

    return scene;
}

bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        auto pCloseItem = MenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
			CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
        CC_BREAK_IF(! pCloseItem);
        pCloseItem->setPosition(Vec2(Director::getInstance()->getWinSize().width - 20, 20));
        auto pMenu = Menu::create(pCloseItem, NULL);
        pMenu->setPosition(Vec2(0, 0));
        CC_BREAK_IF(! pMenu);
        this->addChild(pMenu, 1);
		
		//����tmx��ͼ
		map = TMXTiledMap::create("0.tmx");
		addChild(map);

		auto pChildrenArray = map->getChildren();
		SpriteBatchNode* child = NULL;
		//CCObject* pObject = NULL;
		//����Tilemap������ͼ��
		//CCARRAY_FOREACH(&pChildrenArray, pObject)
		for (auto it = pChildrenArray.begin(); it != pChildrenArray.end(); it++) {
			//child = (CCSpriteBatchNode*)pObject;
			child = static_cast<SpriteBatchNode*>(*it);
			if(!child)
				break;
			//��ͼ��������������
			child->getTexture()->setAntiAliasTexParameters();
		}

		walkAnimation = new Animation*[4];
		walkAnimation[kDown] = createAnimationByDirection(kDown);
		walkAnimation[kRight] = createAnimationByDirection(kRight);
		walkAnimation[kLeft] = createAnimationByDirection(kLeft);
		walkAnimation[kUp] = createAnimationByDirection(kUp);

		//��frame0��Ϊ��ʿ�ľ�̬ͼ
		heroSprite = Sprite::createWithSpriteFrame(walkAnimation[kDown]->getFrames()->at(0)->getSpriteFrame());
		//heroSprite->setPosition(ccp(48, 48));
		heroSprite->setAnchorPoint(Vec2(0, 0));
		heroSprite->setPosition(positionForTileCoord(Vec2(1, 11)));
		addChild(heroSprite);
		isHeroWalking = false;

		auto down = MenuItemFont::create("down", this, CC_CALLBACK_1(HelloWorld::menuCallBackMove, this));
		auto left = MenuItemFont::create("left", this, CC_CALLBACK_1(HelloWorld::menuCallBackMove, this) );
		auto right = MenuItemFont::create("right", this, CC_CALLBACK_1(HelloWorld::menuCallBackMove, this) );
		auto up = MenuItemFont::create("up", this, CC_CALLBACK_1(HelloWorld::menuCallBackMove, this) );
		auto menu = Menu::create(down, left, right, up, NULL);
		//Ϊ�˷�����ң���ÿ��menuItem����tag
		down->setTag(kDown);
		left->setTag(kLeft);
		right->setTag(kRight);
		up->setTag(kUp);
		//�˵�����50ˮƽ����
		menu->alignItemsHorizontallyWithPadding(50);
		addChild(menu);

		schedule(CC_SCHEDULE_SELECTOR(HelloWorld::update));

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

Animation* HelloWorld::createAnimationByDirection(HeroDirection direction)
{
	//auto heroTexture = TextureCache::sharedTextureCache()->addImage("hero.png");
	auto heroTexture = Director::getInstance()->getTextureCache()->addImage("hero.png");
	SpriteFrame *frame0, *frame1, *frame2, *frame3;
	//�ڶ���������ʾ��ʾ�����x, y, width, height������direction��ȷ����ʾ��y����
	frame0 = SpriteFrame::createWithTexture(heroTexture, cocos2d::Rect(32*0, 32*direction, 32, 32));
	frame1 = SpriteFrame::createWithTexture(heroTexture, cocos2d::Rect(32*1, 32*direction, 32, 32));
	frame2 = SpriteFrame::createWithTexture(heroTexture, cocos2d::Rect(32*2, 32*direction, 32, 32));
	frame3 = SpriteFrame::createWithTexture(heroTexture, cocos2d::Rect(32*3, 32*direction, 32, 32));
	auto animFrames = new Vector<SpriteFrame*>(4);
	animFrames->pushBack(frame0);
	animFrames->pushBack(frame1);
	animFrames->pushBack(frame2);
	animFrames->pushBack(frame3);
	auto animation = new Animation();
	//0.2f��ʾÿ֡������ļ��
	animation->createWithSpriteFrames(*animFrames, 0.07f);
	//animFrames->release();

	return animation;
}

void HelloWorld::menuCallBackMove(Ref* pSender)
{
	if (isHeroWalking)
		return;

    auto node = (Node *) pSender;
	//��ť��tag������Ҫ���ߵķ���
	int targetDirection = node->getTag();
	//�ƶ��ľ���
	Point moveByPosition;
	//���ݷ�������ƶ��ľ���
	switch (targetDirection)
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
	//����Ŀ�����꣬�õ�ǰ��ʿ��������ƶ�����
	Point targetPosition = heroSprite->getPosition() + moveByPosition;
	//����checkCollision�����ײ���ͣ������ǽ�ڣ���ֻ��Ҫ������ʿ�ĳ���
	if (checkCollision(targetPosition) == kWall)
	{
		setFaceDirection((HeroDirection)targetDirection);
		return;
	}

	//����CCSpawn�����߶������ƶ�ͬʱִ��
	auto action = Sequence::create(
			Spawn::create(
				Animate::create(walkAnimation[targetDirection]),
				MoveBy::create(0.28f, moveByPosition),
				NULL),
				CC_CALLBACK_2(HelloWorld::onWalkDone, (void*)targetDirection, this),
			//�ѷ�����Ϣ���ݸ�onWalkDone����
			//CCCallFuncND::actionWithTarget(this, callfuncND_selector(HelloWorld::onWalkDone), (void*)targetDirection),
			NULL);
	heroSprite->runAction(action);
	isHeroWalking = true;
}

void HelloWorld::setFaceDirection(HeroDirection direction)
{
	heroSprite->setTextureRect(Rect(0,32*direction,32,32));
}

void HelloWorld::onWalkDone(Ref* pTarget, void* data)
{
	//��void*��ת��Ϊint���ٴ�intת����ö������
	int direction = (int) data;
	setFaceDirection((HeroDirection)direction);
	isHeroWalking = false;
	CCLOG("what!!!!!!\n");
	//heroSprite->setPosition(targetPosition);
	// setSceneScrollPosition(heroSprite->getPosition());
}

//��cocos2d-x����ת��ΪTilemap����
Point HelloWorld::tileCoordForPosition(Point position)
{
	int x = position.x / map->getTileSize().width;
	int y = (((map->getMapSize().height - 1) * map->getTileSize().height) - position.y) / map->getTileSize().height;
	return Vec2(x, y);
}

//��Tilemap����ת��Ϊcocos2d-x����
Point HelloWorld::positionForTileCoord(Point tileCoord)
{
	Point pos =  Vec2((tileCoord.x * map->getTileSize().width),
		((map->getMapSize().height - tileCoord.y - 1) * map->getTileSize().height));
	return pos;
}

//������ʿ��ǰλ����Ϣ���������ƶ�����Ӧλ��
void HelloWorld::setSceneScrollPosition(Point position)
{
	//��ȡ��Ļ�ߴ�
	Size screenSize = Director::getInstance()->getWinSize();
	//ȡ��ʿ��ǰx�������Ļ�е�x�����ֵ�������ʿ��xֵ�ϴ�������
	float x = MAX(position.x, screenSize.width / 2.0f);
	float y = MAX(position.y, screenSize.height / 2.0f);
	//��ͼ�ܿ�ȴ�����Ļ��ȵ�ʱ����п��ܹ���
	if (map->getMapSize().width > screenSize.width)
	{
		//���������������벻�ܳ�����ͼ�ܿ��ȥ��Ļ���1/2
		x = MIN(x, (map->getMapSize().width * map->getTileSize().width) 
			- screenSize.width / 2.0f);
	}
	if (map->getMapSize().height > screenSize.height)
	{
		y = MIN(y, (map->getMapSize().height * map->getTileSize().height) 
			- screenSize.height / 2.0f);
	}
	//��ʿ��ʵ��λ��
	Point heroPosition = Vec2(x, y);
	//��Ļ�е�λ��
	Point screenCenter = Vec2(screenSize.width/2.0f, screenSize.height/2.0f);
	//������ʿʵ��λ�ú��ص�λ�õľ���
	Point scrollPosition = screenCenter - heroPosition;
	//�������ƶ�����Ӧλ��
	//setPosition(scrollPosition);
	CCLOG("scene position: %f,%f", scrollPosition.x, scrollPosition.y);
}

void HelloWorld::update(float dt)
{
	//�����ʿ��������״̬������Ҫ���³���λ��
	if (isHeroWalking)
	{
		setSceneScrollPosition(heroSprite->getPosition());
	}
}

//�ж���ײ����
CollisionType HelloWorld::checkCollision(Point heroPosition) 
{
	//cocos2d-x����ת��ΪTilemap����
	Point tileCoord = tileCoordForPosition(heroPosition);
	//�����ʿ���곬����ͼ�߽磬����kWall���ͣ���ֹ���ƶ�
	if (heroPosition.x < 0 || tileCoord.x > map->getMapSize().width - 1 || tileCoord.y < 0 || tileCoord.y > map->getMapSize().height - 1)
		return kWall;
	//��ȡ��ǰ����λ�õ�ͼ��ID
	int tileGid = map->getLayer("wall")->getTileGIDAt(tileCoord);
	//���ͼ��ID��Ϊ0����ʾ��ǽ
	if (tileGid) {
		return kWall;
	}
	//����ͨ��
	return kNone;
}