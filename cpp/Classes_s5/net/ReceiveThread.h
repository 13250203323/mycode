/**
*�����ߣ��ɳ�
* �޸�ʱ��:
*/
/*
                   _ooOoo_
                  o8888888o
                  88" . "88
                  (| -_- |)
                  O\  =  /O
               ____/`---'\____
             .'  \\|     |//  `.
            /  \\|||  :  |||//  \
           /  _||||| -:- |||||-  \
           |   | \\\  -  /// |   |
           | \_|  ''\---/''  |   |
           \  .-\__  `-`  ___/-. /
         ___`. .'  /--.--\  `. . __
      ."" '<  `.___\_<|>_/___.'  >'"".
     | | :  `- \`.;`\ _ /`;.`/ - ` : | |
     \  \ `-.   \_ __\ /__ _/   .-` /  /
======`-.____`-.___\_____/___.-`____.-'======
                   `=---='
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
         ���汣��       ����BUG
*/
/* ****************************************************************************/
#ifndef __CC_NET_RESPONSE_THREAD_H__
#define __CC_NET_RESPONSE_THREAD_H__
// ������Ҫ ������������͹�������Ϣ
#include "pthread.h"
#include "cocos2d.h"


typedef struct _ServerDataFormat
{
	int len;
	unsigned short moduleid;               
	unsigned short methodid;
	unsigned short unicodeid;
	char* content;
} ServerDataFormat;


typedef void (cocos2d::Ref::*ReceiveThreadEvent)(ServerDataFormat*);
#define callFunc_selectormsg(_SELECTOR) (ReceiveThreadEvent)(&_SELECTOR)

#define M_ADDCALLBACKEVENT(varName)\
protected: cocos2d::Ref* m_##varName##listener;ReceiveThreadEvent varName##selector;\
public: void add##varName##ListenerEvent(ReceiveThreadEvent m_event,cocos2d::Ref* listener)  { m_##varName##listener=listener;varName##selector=m_event; }

class ReceiveThread
{
public:	
	~ReceiveThread(void);
	static ReceiveThread*   GetInstance(); // ��ȡ����ĵ���
	int start (void * =NULL); //�������߳����������������������������ָ�롣
	void stop();     //������ֹ��ǰ�̡߳�
	//void sleep (int tesec); //�����õ�ǰ�߳����߸���ʱ�䣬��λΪ�����롣
	//void detach();   //
	//void * wait();
    bool isRunning;	
private:
	ReceiveThread(void);
	pthread_t handle; 
	bool started;
	bool detached;
	
	static void * threadFunc(void *);
	static ReceiveThread* m_pInstance;	
	M_ADDCALLBACKEVENT(msg);// ����ص�����
	M_ADDCALLBACKEVENT(notcon);//�����ص�����
	
};

#endif
