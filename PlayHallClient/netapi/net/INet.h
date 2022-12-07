#pragma once

#include<winsock2.h>

//加载动态库的引入库
//#pragma comment(lib, "ws2_32.lib")  vs 特定的
#include<ws2tcpip.h>
#include<process.h>

#include<string>
#include<set>


class INetMediator;
class INet
{
public:
    INet(){}
    virtual ~INet(){} //使用时, 父类指针指向子类, 使用虚析构
    //初始化网络
    virtual bool InitNet(const char* szBufIP , unsigned short port) = 0 ;
    //关闭网络
    virtual void UnInitNet() = 0 ;
    //发送 : 同时兼容tcp udp
    virtual bool SendData( unsigned int lSendIP , char* buf , int nlen ) = 0;

    //ip long型向字符串转换
    static std::string GetIPString( long ip );

    //获取本地的所有网络ip 地址列表  key - value  std::set
    static std::set<long> GetValidIPList();
    //判断当前有没有连接到服务器 -- TCP客户端使用
    virtual bool IsConnected(){ return true;}

protected:
    //接收
    virtual void RecvData() = 0 ;
    INetMediator * m_pMediator; //中介 , 用于将网络数据向类外传输
    static bool m_isLoadlib;
};
