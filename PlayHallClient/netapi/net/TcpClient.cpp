#include "TcpClient.h"


#include"INetMediator.h"



TcpClient::TcpClient( INetMediator * pMediator ):m_sock( INVALID_SOCKET ),m_isStop(false)
{
    m_pMediator = pMediator;
    m_isConnected = false;
    m_hThreadHandle = nullptr;
}

TcpClient::~TcpClient()  //使用时, 父类指针指向子类, 使用虚析构
{
    TcpClient::UnInitNet();
}
//初始化网络	//加载库 创建套接字 绑定
bool TcpClient::InitNet(const char *szBufIP, unsigned short port)
{
    if( !m_isLoadlib )
    {
        //1.做个小买卖 炸串 --加载库
        WORD wVersionRequested;
        WSADATA wsaData;
        int err;

    /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
        wVersionRequested = MAKEWORD(2, 2);

        err = WSAStartup(wVersionRequested, &wsaData);
        if (err != 0) {
            return false;
        }

        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
            WSACleanup();
            return false;
        }
        m_isLoadlib = true;
    }

    m_isConnected = false;

    //2.雇人-- 创建套接字 进程与外界网络通信需要的接口 决定了与外界通讯的方式(tcp udp)
    m_sock = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP ); // ipv4 udp
    if ( m_sock == INVALID_SOCKET) {
        WSACleanup();
        return false;
    }

    //3. 连接服务器
    sockaddr_in addr;
    addr.sin_family = AF_INET ;
    addr.sin_addr.S_un.S_addr = inet_addr( szBufIP );  /*inet_addr("192.168.31.115")*/ ;  //绑定任意网卡
    addr.sin_port = htons( port );  //htons 转换为网络字节序 大端存储  43232


    if( connect( m_sock ,(const sockaddr* ) &addr , sizeof(addr) ) == SOCKET_ERROR )
    {
        UnInitNet();
        return false;
    }

    //设置客户端发送缓冲区
    //     int nSendBuf=64*1024;//设置为64K
    //     setsockopt(m_sock,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));
    //设置客户端接收缓冲区
    //     int nRecvBuf=64*1024;//设置为64K
    //     setsockopt(m_sock,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));

//    DWORD nNetTimeout= 300;//1秒 - 1000

//    //发送时限 , 超时退出避免长时间阻塞
//    setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(DWORD));

    //禁用 TCP-NODELAY
    int value = 1;
    setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, (char *)&value, sizeof(int));


    m_isConnected = true;
    // 收数据 -- 创建线程 CreateThread  WinAPI  strcpy  C/C++ RunTime 库函数 创建内存块
    m_hThreadHandle = (HANDLE)_beginthreadex(  NULL, 0 ,&RecvThread ,this , 0 , NULL );     //( CreateThread 创建内存块 )
    //_endthreadex(); -- 回收内存块      //( ExitThread 不回收内存块 ) --内存泄露

    return true;
}

unsigned int __stdcall TcpClient::RecvThread( void * lpvoid)
{
    TcpClient* pthis = (TcpClient*) lpvoid;
    pthis->RecvData();

    return 0;
}

//关闭网络
void TcpClient::UnInitNet()
{
    m_isStop = true ; // 尝试退出线程循环
    if( m_hThreadHandle )
    {
        if( WaitForSingleObject(m_hThreadHandle , 100 ) == WAIT_TIMEOUT )
        {
            TerminateThread( m_hThreadHandle , -1 );
        }
        CloseHandle(m_hThreadHandle);
        m_hThreadHandle = NULL;
    }
    if( m_sock && m_sock != INVALID_SOCKET )
        closesocket(m_sock);

    if( m_isLoadlib )
    {
        WSACleanup();
        m_isLoadlib = false;
    }
}
//发送 : 同时兼容tcp udp
bool TcpClient::SendData(unsigned int lSendIP , char* szbuf , int nlen )
{
    if( !szbuf|| nlen <= 0 ) return false;

    //防止粘包  策略: 先发包大小 再发数据包
    // m_sock  <--> lSendIP
/*	send( m_sock , (char*)&nlen , sizeof(int) , 0  );

    if( send( m_sock , buf , nlen , 0  ) <= 0 )
        return false;

    return true;
*/

    lSendIP = m_sock;

    int DataLen = nlen + 4;
    std::vector<char> vecbuf;
    vecbuf.resize( DataLen );

 //   char* buf = new char[ DataLen ];
    char* buf = &*vecbuf.begin();
    char* tmp = buf;
    *(int*) tmp = nlen;
    tmp+= sizeof(int);

    memcpy( tmp , szbuf , nlen);

    int res = send( lSendIP ,buf,DataLen , 0);
//    delete[] buf;
    return res;
}
//接收

void TcpClient::RecvData()
{
    int nPackSize = 0; // 存储包大小
    int nRes = 0;
    while( !m_isStop )
    {
        //接收先接收包大小 在接受数据包
        nRes = recv( m_sock , (char*)&nPackSize , sizeof(int) , 0   );
        //从接收缓冲区拷贝包大小
        if( nRes == 0)
        {
            this->m_isConnected = false;
            this->m_pMediator->disConnect();
            closesocket(m_sock);
            m_sock = INVALID_SOCKET;
            break;
        }
        if( nRes < 0 )
        {
            if((errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN))
            {
                continue;
            }
            /*
            EAGAIN：套接字已标记为非阻塞，而接收操作被阻塞或者接收超时
            EBADF：sock不是有效的描述词
            ECONNREFUSE：远程主机阻绝网络连接
            EWOULDBLOCK：用于非阻塞模式，不需要重新读或者写
            EFAULT：内存空间访问出错
            EINTR：操作被信号中断
            EINVAL：参数无效
            ENOMEM：内存不足
            ENOTCONN：与面向连接关联的套接字尚未被连接上
            ENOTSOCK：sock索引的不是套接字 当返回值是0时，为正常关闭连接
            */
            closesocket(m_sock);
            m_sock = INVALID_SOCKET;
            break;
        }
        int offset = 0;
        char * buf = new char[nPackSize];
        while( nPackSize )
        {
            nRes = recv( m_sock , buf + offset , nPackSize  , 0   );
            if( nRes > 0 )
            {
                nPackSize -= nRes;
                offset += nRes;
            }
        }
        this->m_pMediator->DealData( m_sock , buf , offset  );  //需要在这个函数回收 堆区 空间
    }
}

bool TcpClient::IsConnected()
{
    return  m_isConnected;
}

