#include "TcpServerMediator.h"

#include"TcpServer.h"

TcpServerMediator::TcpServerMediator()
{
	//new 网络对象
	m_pNet = new TcpServer(this);
}
TcpServerMediator::~TcpServerMediator()  //使用时, 父类指针指向子类, 使用虚析构
{
	if( m_pNet )
	{
		delete m_pNet;
		m_pNet = NULL;
	}
}

//初始化网络
bool TcpServerMediator::OpenNet(const char *szBufIP, unsigned short port)
{
    return m_pNet->InitNet( szBufIP , port);
} 
//关闭网络
void TcpServerMediator::CloseNet()
{
	m_pNet->UnInitNet();
} 
//发送 : 同时兼容tcp udp 
bool TcpServerMediator::SendData( unsigned int lSendIP , char* buf , int nlen )
{
    return m_pNet->SendData(lSendIP , buf , nlen );
}

#include<iostream>
//处理 
void TcpServerMediator::DealData(unsigned int lSendIP , char* buf , int nlen )
{
	//将这几个参数 以信号的形式发送出去  -- IKernel类 的槽函数处理 -- 注意空间回收

//	std::cout<< lSendIP << ":" << buf <<std::endl;

    Q_EMIT SIG_ReadyData( lSendIP , buf , nlen);
}
