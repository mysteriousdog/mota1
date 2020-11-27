#include "AnimationManager.h"

DECLARE_SINGLETON_MEMBER(AnimationManager);

AnimationManager::AnimationManager()
{
}

AnimationManager::~AnimationManager()
{
	//CCDirector���Լ����AnimationCache
	//AnimationCache::purgeSharedAnimationCache();
}

bool AnimationManager::initAnimationMap()
{
	char temp[20];
	sprintf(temp, "%d", aDown);
	//������ʿ�����ߵĶ���
	//AnimationCache::sharedAnimationCache()->addAnimation(createHeroMovingAnimationByDirection(kDown), temp);
	AnimationCache::getInstance()->addAnimation(createHeroMovingAnimationByDirection(kDown), temp);
	sprintf(temp, "%d", aRight);
	//������ʿ�����ߵĶ���
	//AnimationCache::sharedAnimationCache()->addAnimation(createHeroMovingAnimationByDirection(kRight), temp);
	AnimationCache::getInstance()->addAnimation(createHeroMovingAnimationByDirection(kRight), temp);
	sprintf(temp, "%d", aLeft);
	//������ʿ�����ߵĶ���
	//AnimationCache::sharedAnimationCache()->addAnimation(createHeroMovingAnimationByDirection(kLeft), temp);
	AnimationCache::getInstance()->addAnimation(createHeroMovingAnimationByDirection(kLeft), temp);
	sprintf(temp, "%d", aUp);
	//������ʿ�����ߵĶ���
	//AnimationCache::sharedAnimationCache()->addAnimation(createHeroMovingAnimationByDirection(kUp), temp);
	AnimationCache::getInstance()->addAnimation(createHeroMovingAnimationByDirection(kUp), temp);
	//����ս������
	sprintf(temp, "%d", aFight);
	//AnimationCache::sharedAnimationCache()->addAnimation(createFightAnimation(), temp);
	AnimationCache::getInstance()->addAnimation(createFightAnimation(), temp);
	//����NPC����
	//AnimationCache::sharedAnimationCache()->addAnimation(createNPCAnimation(), "npc0");
	AnimationCache::getInstance()->addAnimation(createNPCAnimation(), "npc0");
	return true;
}

Animation* AnimationManager::createHeroMovingAnimationByDirection(HeroDirection direction)
{
	//auto *heroTexture = TextureCache::sharedTextureCache()->addImage("hero.png");
	auto heroTexture = Director::getInstance()->getTextureCache()->addImage("hero.png");
	SpriteFrame *frame0, *frame1, *frame2, *frame3;
	//�ڶ���������ʾ��ʾ�����x, y, width, height������direction��ȷ����ʾ��y����
	frame0 = SpriteFrame::createWithTexture(heroTexture, Rect(32*0, 32*direction, 32, 32));
	frame1 = SpriteFrame::createWithTexture(heroTexture, Rect(32*1, 32*direction, 32, 32));
	frame2 = SpriteFrame::createWithTexture(heroTexture, Rect(32*2, 32*direction, 32, 32));
	frame3 = SpriteFrame::createWithTexture(heroTexture, Rect(32*3, 32*direction, 32, 32));
	Vector<SpriteFrame*>* animFrames = new Vector<SpriteFrame*>(4);
	animFrames->pushBack(frame0);
	animFrames->pushBack(frame1);
	animFrames->pushBack(frame2);
	animFrames->pushBack(frame3);
	Animation* animation = new Animation();
	//0.05f��ʾÿ֡������ļ��
	animation->createWithSpriteFrames(*animFrames, 0.05f);
	//animFrames->release();

	return animation;
}

//����ս������ģ��
Animation* AnimationManager::createFightAnimation()
{
	//����ÿ֡�����
	int fightAnim[] = 
	{
		4,6,8,10,13,15,17,19,20,22
	};
	Vector<SpriteFrame*>* animFrames = new Vector<SpriteFrame*>();
	auto *texture = Director::getInstance()->getTextureCache()->addImage("sword.png");
	SpriteFrame *frame;
	int x, y;
	for (int i = 0; i < 10; i++) 
	{
		//����ÿ֡�����������е�ƫ����
		x = fightAnim[i] % 5 - 1;
		y = fightAnim[i] / 5;
		frame = SpriteFrame::createWithTexture(texture, Rect(192*x, 192*y, 192, 192));
		//��17��19֡��y��������-8��ƫ��
		if (fightAnim[i] == 17 || fightAnim[i] == 19)
		{
			frame->setOffsetInPixels( Vec2(0, -8) );
		}
		animFrames->pushBack(frame);
	}
	Animation* animation = new Animation();
	animation->createWithSpriteFrames(*animFrames, 0.1f);
	//animFrames->release();
	return animation;
}

Animation* AnimationManager::createNPCAnimation()
{
	auto *heroTexture = Director::getInstance()->getTextureCache()->addImage("npc.png");
	SpriteFrame *frame0, *frame1, *frame2, *frame3;
	//�ڶ���������ʾ��ʾ�����x, y, width, height������direction��ȷ����ʾ��y����
	frame0 = SpriteFrame::createWithTexture(heroTexture, Rect(32*0, 0, 32, 32));
	frame1 = SpriteFrame::createWithTexture(heroTexture, Rect(32*1, 0, 32, 32));
	frame2 = SpriteFrame::createWithTexture(heroTexture, Rect(32*2, 0, 32, 32));
	frame3 = SpriteFrame::createWithTexture(heroTexture, Rect(32*3, 0, 32, 32));
	Vector<SpriteFrame*>* animFrames = new Vector<SpriteFrame*>(4);
	animFrames->pushBack(frame0);
	animFrames->pushBack(frame1);
	animFrames->pushBack(frame2);
	animFrames->pushBack(frame3);
	Animation* animation = new Animation();
	//0.05f��ʾÿ֡������ļ��
	animation->createWithSpriteFrames(*animFrames, 0.2f);
	//animFrames->release();

	return animation;
}

//��ȡָ������ģ��
Animation* AnimationManager::getAnimation(int key)
{
	char temp[20];
	sprintf(temp, "%d", key);
	return AnimationCache::getInstance()->getAnimation(temp);
}

//��ȡһ��ָ������ģ���ʵ��
Animate* AnimationManager::createAnimate(int key)
{
	//��ȡָ������ģ��
	Animation* anim = getAnimation(key);
	if(anim)
	{
		//���ݶ���ģ������һ������ʵ��
		return cocos2d::Animate::create(anim);
	}
	return NULL;
}

//��ȡһ��ָ������ģ���ʵ��
Animate* AnimationManager::createAnimate(const char* key)
{
	//��ȡָ������ģ��
	Animation* anim = AnimationCache::getInstance()->getAnimation(key);
	if(anim)
	{
		//���ݶ���ģ������һ������ʵ��
		return cocos2d::Animate::create(anim);
	}
	return NULL;
}