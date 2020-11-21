#ifndef _ANIMATION_MANAGER_H_
#define _ANIMATION_MANAGER_H_

#include "MTGame.h"

using namespace cocos2d;

class AnimationManager : public Singleton<AnimationManager>
{
public:
	AnimationManager();
	~AnimationManager();
	//��ʼ������ģ�滺���
	bool initAnimationMap();
	//�������ֵõ�һ������ģ��
	Animation* getAnimation(int key);
	//����һ������ʵ��
	Animate* createAnimate(int key);
	//����һ������ʵ��
	Animate* createAnimate(const char* key);
protected:
	//������ʿ���߶���ģ��
	Animation* createHeroMovingAnimationByDirection(HeroDirection direction);
	Animation* createFightAnimation();
	Animation* createNPCAnimation();
};
//���嶯��������ʵ���ı���
#define sAnimationMgr AnimationManager::instance()

#endif