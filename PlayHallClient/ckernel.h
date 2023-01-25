#ifndef CKERNEL_H
#define CKERNEL_H

#include"maindialog.h"
#include <QObject>
#include "INetMediator.h"
#include "packdef.h"
#include <vector>
#include "logindialog.h"
#include "roomdialog.h"
#include "fiveinlinezone.h"
#include "fiveinline.h"

class CKernel;
//成员函数指针类型
typedef void (CKernel::*PFUN)(unsigned int lSendIP , char* buf , int nlen);

//单例
class CKernel : public QObject
{
    Q_OBJECT
public:
    static CKernel *GetInstance()
    {
        static CKernel kernel;
        return &kernel;
    }
signals:

private:
    void setNetPackMap();
    void ConfigSet();
    explicit CKernel(QObject *parent = 0);
    ~CKernel(){}
    CKernel(const CKernel &kernel){}
    CKernel &operator=(const CKernel &kernel)
    {
        return *this;
    }
    //成员属性 网络 ui类对象
    MainDialog * m_mainDialog;
    LoginDialog* m_loginDialog;
    FiveInLineZone* m_fiveInLineZone;
    RoomDialog* m_roomDialog;
    FiveInLine* m_fiveInLine;
    INetMediator *m_client;
    //协议映射表 协议头与处理函数的对应关系
    std::vector<PFUN>m_netPackFunMap;

    //个人信息
    int m_id;
    int m_roomid;
    int m_zoneid;
    bool m_isHost;
    QString m_userName;
    char m_serverIP[20];

    //定时器请求
    QTimer m_rqTimer;

public slots:

    //获取专区每个房间玩家数
    void slot_roomInfoInZone();
    //准备和开局
    void slot_fil_gameReady(int zoneid,int roomid,int userid);
    void slot_fil_gameStart(int zoneid,int roomid);
    void slot_fil_pieceDown(int blackorwhite,int x,int y);
    void slot_fil_win(int blackorwhite);
    //网络处理
    void slot_dealLoginRs(unsigned int lSendIP , char* buf , int nlen);
    void slot_dealRegisterRs(unsigned int lSendIP, char *buf, int nlen);
    void slot_dealJoinRoomRs(unsigned int lSendIP, char *buf, int nlen);
    void slot_dealRoomMemRs(unsigned int lSendIP, char *buf, int nlen);
    void slot_dealLeaveRoomRq(unsigned int lSendIP, char *buf, int nlen);
    void DestroyInstance();
    void slot_JoinZone(int zoneid);
    void slot_joinRoom(int roomid);
    void slot_leaveZone();
    void slot_leaveRoom();
    void slot_ReadyData( unsigned int lSendIP , char* buf , int nlen );
    void slot_loginCommit(QString tel,QString password);
    void slot_registerCommit(QString tel,QString password,QString name);
    void SendData(char *buf, int nlen);
    //游戏
    void slot_fil_playByCpuBegin(int zoneid,int roomid,int userid);
    void slot_fil_playByCpuEnd(int zoneid,int roomid,int userid);
    void slot_dealFilGameReadyRq(unsigned int lSendIP , char* buf , int nlen);
    void slot_dealFilGameStartRq(unsigned int lSendIP , char* buf , int nlen);
    void slot_dealFilPieceDownRq(unsigned int lSendIP , char* buf , int nlen);
    void slot_dealFilWinRq(unsigned int lSendIP , char* buf , int nlen);
    void slot_dealFilPlayByCpuBegin(unsigned int lSendIP , char* buf , int nlen);
    void slot_dealFilPlayByCpuEnd(unsigned int lSendIP , char* buf , int nlen);
    void slot_dealZoneRoomInfo(unsigned int lSendIP , char* buf , int nlen);

};


#endif // CKERNEL_H
