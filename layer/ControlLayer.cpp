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

	//�����رհ�ť
	MenuItemImage *pCloseItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(ControlLayer::menuCloseCallback, this));
	pCloseItem->setPosition(Vec2(Director::getInstance()->getWinSize().width- 20, 20));
	Menu* pMenu = Menu::create(pCloseItem, NULL);
	pMenu->setPosition(Vec2());
	this->addChild(pMenu, 1);

	//��������ť
	MenuItem *down = MenuItemFont::create("down",CC_CALLBACK_1(ControlLayer::menuCallBackMove, this));
	MenuItem *left = MenuItemFont::create("left", CC_CALLBACK_1(ControlLayer::menuCallBackMove, this) );
	MenuItem *right = MenuItemFont::create("right", CC_CALLBACK_1(ControlLayer::menuCallBackMove, this) );
	MenuItem *up = MenuItemFont::create("up", CC_CALLBACK_1(ControlLayer::menuCallBackMove, this) );
	Menu* menu = Menu::create(down, left, right, up, NULL);
	//Ϊ�˷�����ң���ÿ��menuItem����tag
	down->setTag(kDown);
	left->setTag(kLeft);
	right->setTag(kRight);
	up->setTag(kUp);
	//�˵�����50ˮƽ����
	menu->alignItemsHorizontallyWithPadding(50);
	this->addChild(menu);
	return true;
}

void ControlLayer::menuCloseCallback(Node* pSender)
{
	Director::getInstance()->end();
}

void ControlLayer::menuCallBackMove(Node* pSender)
{
    Node *node = (Node *) pSender;
	//��ť��tag������Ҫ���ߵķ���
	int targetDirection = node->getTag();
	sGlobal->hero->move((HeroDirection) targetDirection);
}