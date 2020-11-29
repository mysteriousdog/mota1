 #include "HelloWorldScene.h"

using namespace cocos2d;

HelloWorld::HelloWorld()
{

}

HelloWorld::~HelloWorld(void)
{
	for (int i = 0; i < 4; i++)
	{
		//释放数组中元素
		CC_SAFE_RELEASE(walkAnimation[i]);
	}
	//释放数组本身
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
		
		//解析tmx地图
		map = TMXTiledMap::create("0.tmx");
		addChild(map);

		auto pChildrenArray = map->getChildren();
		SpriteBatchNode* child = NULL;
		//CCObject* pObject = NULL;
		//遍历Tilemap的所有图层
		//CCARRAY_FOREACH(&pChildrenArray, pObject)
		for (auto it = pChildrenArray.begin(); it != pChildrenArray.end(); it++) {
			//child = (CCSpriteBatchNode*)pObject;
			child = static_cast<SpriteBatchNode*>(*it);
			if(!child)
				break;
			//给图层的纹理开启抗锯齿
			child->getTexture()->setAntiAliasTexParameters();
		}

		walkAnimation = new Animation*[4];
		walkAnimation[kDown] = createAnimationByDirection(kDown);
		walkAnimation[kRight] = createAnimationByDirection(kRight);
		walkAnimation[kLeft] = createAnimationByDirection(kLeft);
		walkAnimation[kUp] = createAnimationByDirection(kUp);

		//用frame0作为勇士的静态图
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
		//为了方便查找，给每个menuItem设置tag
		down->setTag(kDown);
		left->setTag(kLeft);
		right->setTag(kRight);
		up->setTag(kUp);
		//菜单项按间距50水平排列
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
	//第二个参数表示显示区域的x, y, width, height，根据direction来确定显示的y坐标
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
	//0.2f表示每帧动画间的间隔
	animation->createWithSpriteFrames(*animFrames, 0.07f);
	//animFrames->release();

	return animation;
}

void HelloWorld::menuCallBackMove(Ref* pSender)
{
	if (isHeroWalking)
		return;

    auto node = (Node *) pSender;
	//按钮的tag就是需要行走的方向
	int targetDirection = node->getTag();
	//移动的距离
	Point moveByPosition;
	//根据方向计算移动的距离
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
	//计算目标坐标，用当前勇士坐标加上移动距离
	Point targetPosition = heroSprite->getPosition() + moveByPosition;
	//调用checkCollision检测碰撞类型，如果是墙壁，则只需要设置勇士的朝向
	if (checkCollision(targetPosition) == kWall)
	{
		setFaceDirection((HeroDirection)targetDirection);
		return;
	}

	//利用CCSpawn将行走动画和移动同时执行
	auto action = Sequence::create(
			Spawn::create(
				Animate::create(walkAnimation[targetDirection]),
				MoveBy::create(0.28f, moveByPosition),
				NULL),
				CC_CALLBACK_2(HelloWorld::onWalkDone, (void*)targetDirection, this),
			//把方向信息传递给onWalkDone方法
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
	//将void*先转换为int，再从int转换到枚举类型
	int direction = (int) data;
	setFaceDirection((HeroDirection)direction);
	isHeroWalking = false;
	CCLOG("what!!!!!!\n");
	//heroSprite->setPosition(targetPosition);
	// setSceneScrollPosition(heroSprite->getPosition());
}

//从cocos2d-x坐标转换为Tilemap坐标
Point HelloWorld::tileCoordForPosition(Point position)
{
	int x = position.x / map->getTileSize().width;
	int y = (((map->getMapSize().height - 1) * map->getTileSize().height) - position.y) / map->getTileSize().height;
	return Vec2(x, y);
}

//从Tilemap坐标转换为cocos2d-x坐标
Point HelloWorld::positionForTileCoord(Point tileCoord)
{
	Point pos =  Vec2((tileCoord.x * map->getTileSize().width),
		((map->getMapSize().height - tileCoord.y - 1) * map->getTileSize().height));
	return pos;
}

//传入勇士当前位置信息，将场景移动到相应位置
void HelloWorld::setSceneScrollPosition(Point position)
{
	//获取屏幕尺寸
	Size screenSize = Director::getInstance()->getWinSize();
	//取勇士当前x坐标和屏幕中点x的最大值，如果勇士的x值较大，则会滚动
	float x = MAX(position.x, screenSize.width / 2.0f);
	float y = MAX(position.y, screenSize.height / 2.0f);
	//地图总宽度大于屏幕宽度的时候才有可能滚动
	if (map->getMapSize().width > screenSize.width)
	{
		//场景滚动的最大距离不能超过地图总宽减去屏幕宽的1/2
		x = MIN(x, (map->getMapSize().width * map->getTileSize().width) 
			- screenSize.width / 2.0f);
	}
	if (map->getMapSize().height > screenSize.height)
	{
		y = MIN(y, (map->getMapSize().height * map->getTileSize().height) 
			- screenSize.height / 2.0f);
	}
	//勇士的实际位置
	Point heroPosition = Vec2(x, y);
	//屏幕中点位置
	Point screenCenter = Vec2(screenSize.width/2.0f, screenSize.height/2.0f);
	//计算勇士实际位置和重点位置的距离
	Point scrollPosition = screenCenter - heroPosition;
	//将场景移动到相应位置
	//setPosition(scrollPosition);
	CCLOG("scene position: %f,%f", scrollPosition.x, scrollPosition.y);
}

void HelloWorld::update(float dt)
{
	//如果勇士不在行走状态，不需要更新场景位置
	if (isHeroWalking)
	{
		setSceneScrollPosition(heroSprite->getPosition());
	}
}

//判断碰撞类型
CollisionType HelloWorld::checkCollision(Point heroPosition) 
{
	//cocos2d-x坐标转换为Tilemap坐标
	Point tileCoord = tileCoordForPosition(heroPosition);
	//如果勇士坐标超过地图边界，返回kWall类型，阻止其移动
	if (heroPosition.x < 0 || tileCoord.x > map->getMapSize().width - 1 || tileCoord.y < 0 || tileCoord.y > map->getMapSize().height - 1)
		return kWall;
	//获取当前坐标位置的图块ID
	int tileGid = map->getLayer("wall")->getTileGIDAt(tileCoord);
	//如果图块ID不为0，表示有墙
	if (tileGid) {
		return kWall;
	}
	//可以通行
	return kNone;
}