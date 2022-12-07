#include "TcpClientMediator.h"

#include"TcpClient.h"

TcpClientMediator::TcpClientMediator()
{
	//new 网络对象
	m_pNet = new TcpClient(this);
}
TcpClientMediator::~TcpClientMediator()  //使用时, 父类指针指向子类, 使用虚析构
{
	if( m_pNet )
	{
		delete m_pNet;
		m_pNet = NULL;
	}
}
//初始化网络
bool TcpClientMediator::OpenNet(const char *szBufIP, unsigned short port)
{

    strcpy_s( m_szBufIP, sizeof(m_szBufIP), szBufIP);
    m_port = port;

    return m_pNet->InitNet(szBufIP , port);
} 
//关闭网络
void TcpClientMediator::CloseNet()
{
	m_pNet->UnInitNet();
} 
//发送 : 同时兼容tcp udp 
bool TcpClientMediator::SendData( unsigned int lSendIP , char* buf , int nlen )
{
    Q_UNUSED(lSendIP);
    if( IsConnected() )
        return m_pNet->SendData( 0 , buf, nlen);
    else
    {
        m_pNet->UnInitNet();
        delete m_pNet;
        m_pNet = new TcpClient(this);
        if( this->OpenNet( m_szBufIP , m_port ) )
        {
            return m_pNet->SendData( 0,buf,nlen);
        }
        else
        {
            return -1;
        }
    }
}
#include<iostream>
//处理 
void TcpClientMediator::DealData(unsigned int lSendIP , char* buf , int nlen )
{
	//将这几个参数 以信号的形式发送出去  -- IKernel类 的槽函数处理 -- 注意空间回收
    //std::cout<< lSendIP << ":" << buf <<std::endl;
    Q_EMIT SIG_ReadyData( lSendIP , buf , nlen);
}

void TcpClientMediator::disConnect()
{
    Q_EMIT SIG_disConnect();
}

bool TcpClientMediator::IsConnected()
{
    return m_pNet->IsConnected();
}

