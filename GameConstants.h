#ifndef __GAME_CONSTANTS_H__
#define __GAME_CONSTANTS_H__

typedef enum {
	kDown =  0,//���·���
	kLeft = 1,//������
	kRight= 2,//���ҷ���
	kUp = 3,//���Ϸ���
	kNormal,
} HeroDirection;//��ʿ����

typedef enum
{
	kNone = 1,//����ͨ��
	kWall,//ǽ
	kEnemy,//����
	kItem,//��Ʒ
	kDoor,//��
	kNPC,//npc
	kTeleport,//���͵�
} CollisionType;//��ײ����

typedef enum
{
	aDown = 0,//�������߶���
	aLeft,//�������߶���
	aRight,//�������߶���
	aUp,//�������߶���
	aFight,//���⶯��
} AnimationKey;//����ģ���ֵ

enum 
{
	kZMap = 0,//��ͼ��zOrder
	kZNPC,
	kZTeleport,
	kZHero,//��ʿ�����zOrder
	kZTip,//��ʾ��Ϣ��zOrder
};//GameLayer�и����ֵ���ʾzOrder��tag

enum 
{
	kGameLayer = 0,
	kControlLayer,
	kFadeLayer,
};

#endif