#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

typedef enum {
	kDown =  0,
	kLeft = 1,
	kRight= 2,
	kUp = 3,
} HeroDirection;

typedef enum
{
	kNone = 1,//����ͨ��
	kWall,//ǽ
	kEnemy,//����
} CollisionType;//��ײ����

using namespace cocos2d;

class HelloWorld : public cocos2d::Layer
{
public:
	HelloWorld();
	~HelloWorld();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::Scene* scene();
    
    // a selector callback
    virtual void menuCloseCallback(Ref* pSender);

    // implement the "static node()" method manually
    LAYER_NODE_FUNC(HelloWorld);

	TMXTiledMap *map;

	Sprite* heroSprite;
	HeroDirection heroDirection;
	Animation **walkAnimation;
	Animation* createAnimationByDirection(HeroDirection direction);

	void menuCallBackMove(Ref* pSender);

	void setFaceDirection(HeroDirection direction);
	void onWalkDone(Ref* pTarget, void* data);

	Point tileCoordForPosition(Point position);
	Point positionForTileCoord(Point tileCoord);

	void setSceneScrollPosition(Point position);
	bool isHeroWalking;
	void update(float dt);
	Point targetPosition;

	CollisionType checkCollision(Point heroPosition);
};

#endif  // __HELLOWORLD_SCENE_H__