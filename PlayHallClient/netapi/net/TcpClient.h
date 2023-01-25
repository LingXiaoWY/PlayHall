#pragma once

#include"INet.h"


class TcpClient : public INet
{
public:
	TcpClient( INetMediator * pMediator);
	~TcpClient(void);

	//初始化网络
    bool InitNet(const char* szBufIP , unsigned short port);
	//关闭网络
	void UnInitNet(); 
	//发送 : 同时兼容tcp udp 
    bool SendData( unsigned int lSendIP , char* buf , int nlen );

    bool IsConnected();
protected:
	static unsigned int __stdcall RecvThread( void * lpvoid);

	//接收
	virtual void RecvData();
	SOCKET m_sock;
	HANDLE m_hThreadHandle; // 控制接收线程
	bool m_isStop;
    bool m_isConnected;
};

