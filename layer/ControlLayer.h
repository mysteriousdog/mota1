#ifndef __CONTROLLAYER_H__
#define __CONTROLLAYER_H__

#include "MTGame.h"

using namespace cocos2d;

class ControlLayer: public Layer
{
public:
	ControlLayer(void);
	~ControlLayer(void);
	//node��������ô˺���
	virtual bool init();
	//����ť����¼��Ļص�
	void menuCallBackMove(Node* pSender);
	//�رհ�ť����¼��Ļص�
	void menuCloseCallback(Node* pSender);
	//ʹ��CCLayer��׼�Ĵ���ʵ���ķ�ʽ������node����
	//LAYER_NODE_FUNC(ControlLayer);
	CREATE_FUNC(ControlLayer);
};

#endif
