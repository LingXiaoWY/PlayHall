#pragma once

#include<QObject>

#define _DEF_TCP_PORT  (8000)
#define _DEF_UDP_PORT  (1234)
#define _DEF_SERVER_IP ("192.168.3.133")

class INet;
class INetMediator : public QObject
{
    Q_OBJECT
signals:
    void SIG_ReadyData( unsigned int lSendIP , char* buf , int nlen );
    //客户端断开连接
    void SIG_disConnect();
public:
    INetMediator();
    ~INetMediator();//使用时, 父类指针指向子类, 使用虚析构
    //初始化网络
    virtual bool OpenNet( const char* szBufIP = "0.0.0.0"
                    , unsigned short port = _DEF_TCP_PORT ) = 0 ;

    //关闭网络
    virtual void CloseNet() = 0 ;
    //发送 : 同时兼容tcp udp
    virtual bool SendData( unsigned int lSendIP , char* buf , int nlen ) = 0;
    //接收 处理
    virtual void DealData( unsigned int lSendIP , char* buf , int nlen ) = 0 ;

    //设置 ip port
    void setIpAndPort(const char* szBufIP  , unsigned short port  );
    //客户端是否连接服务器
    virtual bool IsConnected(){
        return true;
    }

    virtual void disConnect(){}

protected:
    INet * m_pNet; //网络
    //ip地址 ipv4
    char  m_szBufIP[18];
    //port
    unsigned short m_port ;
};
