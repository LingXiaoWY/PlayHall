#include<TCPKernel.h>
#include "packdef.h"
#include<stdio.h>
#include<sys/time.h>
#include"clogic.h"

using namespace std;


//设置网络协议映射
void TcpKernel::setNetPackMap()
{
    //清空映射
    bzero( m_NetPackMap , sizeof(m_NetPackMap) );

    //协议映射赋值
    m_logic->setNetPackMap();

}


TcpKernel::TcpKernel()
{

}

TcpKernel::~TcpKernel()
{
    if( m_logic ) delete m_logic;
}

TcpKernel *TcpKernel::GetInstance()
{
    static TcpKernel kernel;
    return &kernel;
}

int TcpKernel::Open( int port)
{
    initRand();

    m_sql = new CMysql;
    // 数据库 使用127.0.0.1 地址  用户名root 密码colin123 数据库 wechat 没有的话需要创建 不然报错
    if(  !m_sql->ConnectMysql( _DEF_DB_IP , _DEF_DB_USER, _DEF_DB_PWD, _DEF_DB_NAME )  )
    {
        printf("Conncet Mysql Failed...\n");
        return FALSE;
    }
    else
    {
        printf("MySql Connect Success...\n");
    }
    //初始网络
    m_tcp = new Block_Epoll_Net;
    bool res = m_tcp->InitNet( port , &TcpKernel::DealData ) ;
    if( !res )
        err_str( "net init fail:" ,-1);

    m_logic = new CLogic(this);

    setNetPackMap();

    return TRUE;
}

void TcpKernel::Close()
{
    m_sql->DisConnect();

}

//随机数初始化
void TcpKernel::initRand()
{
    struct timeval time;
    gettimeofday( &time , NULL);
    srand( time.tv_sec + time.tv_usec );
}

void TcpKernel::DealData(sock_fd clientfd,char *szbuf,int nlen)
{
    PackType type = *(PackType*)szbuf;
    if( (type >= DEF_PACK_BASE) && ( type < DEF_PACK_BASE + DEF_PACK_COUNT) )
    {
        PFUN pf = NetPackMap( type );
        if( pf )
        {
            (TcpKernel::GetInstance()->m_logic->*pf)( clientfd , szbuf , nlen);
        }
    }

    return;
}

void TcpKernel::EventLoop()
{
    printf("event loop\n");
    m_tcp->EventLoop();
}

void TcpKernel::SendData(sock_fd clientfd, char *szbuf, int nlen)
{
    m_tcp->SendData(clientfd , szbuf ,nlen );
}
