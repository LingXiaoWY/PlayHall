#pragma once

#include"INet.h"

class UdpNet:public INet
{
public:
	UdpNet( INetMediator * pMediator );
	~UdpNet(); //使用时, 父类指针指向子类, 使用虚析构
	//初始化网络
    bool InitNet(const char* szBufIP , unsigned short port);
	//关闭网络
	void UnInitNet(); 
	//发送 : 同时兼容tcp udp 
    bool SendData(unsigned int lSendIP , char* buf , int nlen );
	
protected:
	static unsigned int __stdcall RecvThread( void * lpvoid);

	//接收
	virtual void RecvData();
	SOCKET m_sock;
	HANDLE m_hThreadHandle; // 控制接收线程
	bool m_isStop;
    int m_port;
};
