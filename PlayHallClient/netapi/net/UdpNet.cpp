#include"UdpNet.h"

#include"INetMediator.h"



UdpNet::UdpNet( INetMediator * pMediator ):m_sock( INVALID_SOCKET ),m_isStop(false)
{
	m_pMediator = pMediator;
}
UdpNet::~UdpNet()  //使用时, 父类指针指向子类, 使用虚析构
{
    UdpNet::UnInitNet();
}
//初始化网络	//加载库 创建套接字 绑定
bool UdpNet::InitNet(const char* szBufIP , unsigned short port)
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


	//2.雇人-- 创建套接字 进程与外界网络通信需要的接口 决定了与外界通讯的方式(tcp udp)
	m_sock = socket( AF_INET , SOCK_DGRAM , IPPROTO_UDP ); // ipv4 udp
	if ( m_sock == INVALID_SOCKET) {
		WSACleanup();
        return false;
	}

	//设置有限广播权限
	BOOL bval = TRUE;
	setsockopt( m_sock, SOL_SOCKET/*哪一层*/ , /*什么属性*/ SO_BROADCAST , 
		/*设置的值 设置和取消*/(const char*)&bval , sizeof(bval) );

	//3.找地-- 绑定ip地址 
	sockaddr_in addr;
	addr.sin_family = AF_INET ;
    addr.sin_addr.S_un.S_addr = inet_addr( szBufIP );/*INADDR_ANY*/ /*inet_addr("192.168.31.115")*/   //绑定任意网卡
    addr.sin_port = htons( port );  //htons 转换为网络字节序 大端存储  43232
    m_port = port;

	if( bind( m_sock ,(const sockaddr* ) &addr , sizeof(addr) ) == SOCKET_ERROR )
	{
		UnInitNet();
        return false;
	}

	// 收数据 -- 创建线程 CreateThread  WinAPI  strcpy  C/C++ RunTime 库函数 创建内存块
	m_hThreadHandle = (HANDLE)_beginthreadex(  NULL, 0 ,&RecvThread ,this , 0 , NULL );     //( CreateThread 创建内存块 )
	//_endthreadex(); -- 回收内存块      //( ExitThread 不回收内存块 ) --内存泄露

	return true;
}

unsigned int __stdcall UdpNet::RecvThread( void * lpvoid)
{
	UdpNet* pthis = (UdpNet*) lpvoid;
	pthis->RecvData();
	
	return 0;
}

//关闭网络
void UdpNet::UnInitNet()
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
    if( m_isLoadlib ){
        WSACleanup();
        m_isLoadlib = false;
    }
} 
//发送 : 同时兼容tcp udp 
bool UdpNet::SendData( unsigned int lSendIP , char* buf , int nlen )
{
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
    addr.sin_port = htons( m_port );
	addr.sin_addr.S_un.S_addr = lSendIP;

	if( sendto( m_sock , buf , nlen , 0 , (const sockaddr * )&addr ,sizeof(addr) ) <= 0 ) // 目标
		return false;
	
	return true;
}
//接收

void UdpNet::RecvData()
{
    char buf[ 4096 ] = "";
	SOCKADDR_IN addrClient;
	int nSize = sizeof(addrClient);
	m_isStop = false;
	while( !m_isStop )
	{
		int nRes = recvfrom( m_sock , buf , sizeof(buf) , 0 , (sockaddr*)&addrClient , &nSize );
		
		if( nRes > 0 )
		{    //处理 --中介处理
			char * packbuf = new char[nRes];
            memcpy( packbuf  ,buf , nRes);
			if( m_pMediator ){
				m_pMediator->DealData( addrClient.sin_addr.S_un.S_addr , packbuf , nRes );
				//   要回收这个空间 delete[] packbuf;
			}
		}	
	}
}
