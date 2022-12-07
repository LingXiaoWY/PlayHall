#pragma once
#include"INetMediator.h"


class TcpServerMediator :
	public INetMediator
{
public:
	TcpServerMediator(void);
	~TcpServerMediator(void);

	//初始化网络
    bool OpenNet(const char* szBufIP ="0.0.0.0"
        , unsigned short port = _DEF_TCP_PORT );

	//关闭网络
	void CloseNet(); 
	//发送 : 同时兼容tcp udp 
    bool SendData( unsigned int lSendIP , char* buf , int nlen );
	//接收
    void DealData( unsigned int lSendIP , char* buf , int nlen );
};

