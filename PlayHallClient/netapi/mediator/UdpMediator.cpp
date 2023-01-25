#include"UdpMediator.h"

#include"UdpNet.h"

UdpMediator::UdpMediator()
{
	//new 网络对象
	m_pNet = new UdpNet(this);
}
UdpMediator::~UdpMediator()  //使用时, 父类指针指向子类, 使用虚析构
{
	if( m_pNet )
	{
		delete m_pNet;
		m_pNet = NULL;
	}
}
//初始化网络
bool UdpMediator::OpenNet(const char *szBufIP, unsigned short port)
{
    return m_pNet->InitNet( szBufIP , port);
} 
//关闭网络
void UdpMediator::CloseNet()
{
	m_pNet->UnInitNet();
} 

//发送 : 同时兼容tcp udp 
bool UdpMediator::SendData( unsigned int lSendIP , char* buf , int nlen )
{
    return m_pNet->SendData(lSendIP , buf , nlen );
}

//处理 
void UdpMediator::DealData( unsigned int lSendIP , char* buf , int nlen )
{
	//将这几个参数 以信号的形式发送出去  -- IKernel类 的槽函数处理 -- 注意空间回收
    Q_EMIT SIG_ReadyData( lSendIP , buf , nlen);
}
