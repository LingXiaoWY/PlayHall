#include"INetMediator.h"



INetMediator::INetMediator(){}
//使用时, 父类指针指向子类, 使用虚析构
INetMediator::~INetMediator(){}

void INetMediator::setIpAndPort(const char *szBufIP, unsigned short port)
{
    strcpy_s( m_szBufIP , sizeof(m_szBufIP), szBufIP);
    m_port = port;
}
