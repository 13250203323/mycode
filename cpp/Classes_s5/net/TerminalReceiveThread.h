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
#ifndef __CC_NET_TERMINAL_RESPONSE_THREAD_H__
#define __CC_NET_TERMINAL_RESPONSE_THREAD_H__
// ������Ҫ ������������͹�������Ϣ
#include "pthread.h"
#include "cocos2d.h"


typedef struct _ServerDataFormat_terminal
{
	int len;
	short moduleid;               
	short methodid;
	unsigned char unicodeid;
	char* content;
} ServerDataFormat_terminal;


typedef void (cocos2d::Ref::*TerminalReceiveThreadEvent)(ServerDataFormat_terminal*);
#define callFunc_selectormsg_terminal(_SELECTOR) (TerminalReceiveThreadEvent)(&_SELECTOR)

#define M_ADDCALLBACKEVENT_terminal(varName)\
protected: cocos2d::Ref* m_##varName##listener;TerminalReceiveThreadEvent varName##selector;\
public: void add##varName##ListenerEvent(TerminalReceiveThreadEvent m_event,cocos2d::Ref* listener)  { m_##varName##listener=listener;varName##selector=m_event; }

class TerminalReceiveThread
{
public:	
	~TerminalReceiveThread(void);
	static TerminalReceiveThread*   GetInstance(); // ��ȡ����ĵ���
	int start (void * =NULL); //�������߳����������������������������ָ�롣
	void stop();     //������ֹ��ǰ�̡߳�
	//void sleep (int tesec); //�����õ�ǰ�߳����߸���ʱ�䣬��λΪ�����롣
	//void detach();   //
	//void * wait();
    bool isRunning;	
private:
	TerminalReceiveThread(void);
	pthread_t handle; 
	bool started;
	bool detached;
	
	static void * threadFunc(void *);
	static TerminalReceiveThread* m_pInstance;	
	M_ADDCALLBACKEVENT_terminal(msg);// ����ص�����
	M_ADDCALLBACKEVENT_terminal(notcon);//�����ص�����
	
};

#endif
