#ifndef _TCPKERNEL_H
#define _TCPKERNEL_H


#include"block_epoll_net.h"
#include "Mysql.h"


//类成员函数指针 , 用于定义协议映射表
class TcpKernel;
typedef int sock_fd;


#define NetPackMap(a)  TcpKernel::GetInstance()->m_NetPackMap[ a - DEF_PACK_BASE ]

class CLogic;
typedef void (CLogic::*PFUN)(sock_fd,char*,int nlen);

class TcpKernel
{
public:
    //单例模式
    static TcpKernel* GetInstance();

    //开启核心服务
    int Open(int port);
    //初始化随机数
    void initRand();
    //设置协议映射
    void setNetPackMap();
    //关闭核心服务
    void Close();
    //处理网络接收
    static void DealData(sock_fd clientfd, char*szbuf, int nlen);
    //事件循环
    void EventLoop();
    //发送数据
    void SendData( sock_fd clientfd, char*szbuf, int nlen );

private:
    TcpKernel();
    ~TcpKernel();
    //数据库
    CMysql * m_sql;
    //网络
    Block_Epoll_Net * m_tcp;
    //协议映射表
    PFUN m_NetPackMap[DEF_PACK_COUNT];

    CLogic* m_logic;
    friend class CLogic;
};

#endif
