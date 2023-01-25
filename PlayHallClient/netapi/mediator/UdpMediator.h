#pragma once

#include"INetMediator.h"

class UdpMediator: public  INetMediator
{
public:
	UdpMediator();
	~UdpMediator(); //使用时, 父类指针指向子类, 使用虚析构
	//初始化网络
    bool OpenNet(const char* szBufIP ="0.0.0.0"
        , unsigned short port = _DEF_UDP_PORT );

	//关闭网络
	void CloseNet(); 
	//发送 : 同时兼容tcp udp 
    bool SendData( unsigned int lSendIP , char* buf , int nlen );
	//接收
    void DealData( unsigned int lSendIP , char* buf , int nlen );
};
