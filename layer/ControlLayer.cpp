#include "MTGame.h"

ControlLayer::ControlLayer(void)
{
	sGlobal->controlLayer = this;
}

ControlLayer::~ControlLayer(void)
{

}

bool ControlLayer::init()
{
	if ( !Layer::init() )
	{
		return false;
	}

	//�����رհ�
	auto pCloseItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(ControlLayer::menuCloseCallback, this));
	pCloseItem->setPosition(Vec2(Director::getInstance()->getWinSize().width- 20, 20));
	Menu* pMenu = Menu::create(pCloseItem, NULL);
	pMenu->setPosition(Vec2());
	this->addChild(pMenu, 1);

	// //��������ť
	// MenuItem *down = MenuItemFont::create("btn_down",CC_CALLBACK_1(ControlLayer::menuCallBackMove, this));
	// MenuItem *left = MenuItemFont::create("btn_left", CC_CALLBACK_1(ControlLayer::menuCallBackMove, this) );
	// MenuItem *right = MenuItemFont::create("btn_right", CC_CALLBACK_1(ControlLayer::menuCallBackMove, this) );
	// MenuItem *up = MenuItemFont::create("btn_up", CC_CALLBACK_1(ControlLayer::menuCallBackMove, this) );
	// Menu* menu = Menu::create(down, left, right, up, NULL);
	// //Ϊ�˷�����ң���ÿ��menuItem����tag
	// down->setTag(kDown);
	// left->setTag(kLeft);
	// right->setTag(kRight);
	// up->setTag(kUp);
	// //�˵�����50ˮƽ����
	// menu->alignItemsHorizontallyWithPadding(50);
	// this->addChild(menu);

	//键盘事件监听
    auto keyBoardListener = EventListenerKeyboard::create();
    keyBoardListener->onKeyPressed = CC_CALLBACK_2(ControlLayer::OnKeyPressed, this);
    keyBoardListener->onKeyReleased = CC_CALLBACK_2(ControlLayer::OnKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);
    this->scheduleUpdate();

	return true;
}

void ControlLayer::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
}

void ControlLayer::menuCallBackMove(Ref* pSender)
{
    Node *node = (Node *) pSender;
	//��ť��tag������Ҫ���ߵķ���
	int targetDirection = node->getTag();
	CCLOG("the direction get from click menucallbackis %d\n", targetDirection);
	sGlobal->hero->move((HeroDirection) targetDirection);
}
void ControlLayer::OnKeyPressed(EventKeyboard::KeyCode keyCode , Event* event){
    
    Vec2 location;
	if (sGlobal->hero->isHeroMoving || sGlobal->hero->isTalking) {
		return;
	}
    if (keyCode == EventKeyboard::KeyCode::KEY_A) {
        CCLOG("left!!! \n");
		sGlobal->hero->movingDirection = kLeft;
		//sGlobal->hero->move((HeroDirection)(kLeft));
    } else if (keyCode == EventKeyboard::KeyCode::KEY_D) {
        CCLOG("right!!! \n");
		sGlobal->hero->movingDirection = kRight;
		//sGlobal->hero->move((HeroDirection)(kRight));
    } else if (keyCode == EventKeyboard::KeyCode::KEY_W) {
        CCLOG("forword!!! \n");
		//sGlobal->hero->move((HeroDirection)(kUp));
		sGlobal->hero->movingDirection = kUp;
    } else if (keyCode == EventKeyboard::KeyCode::KEY_S) {
        CCLOG("back!!! \n");
		//sGlobal->hero->move((HeroDirection)(kDown));
		sGlobal->hero->movingDirection = kDown;
    } else if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
        CCLOG("escape!!! \n");
    } else {
        return;
    }

}

void ControlLayer::OnKeyReleased(EventKeyboard::KeyCode keyCode , Event* event){
    //CCLOG("Released key %d\n",keyCode);
    // if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
    //     if (pause == 0) {
    //         ShowShop();
    //     } else if (pause == 1) {
    //         BackGame(NULL);
    //     }
    // }
	sGlobal->hero->movingDirection = STAY_GROUND;
}

void ControlLayer::update(float delta){
	int direction = sGlobal->hero->movingDirection;
	if (direction == STAY_GROUND) {
		return;
	} else if(!sGlobal->hero->isHeroMoving && !sGlobal->hero->isTalking) {
		sGlobal->hero->move((HeroDirection)(direction));
	}
}