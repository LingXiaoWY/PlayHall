#include "clogic.h"


void CLogic::setNetPackMap()
{
    NetPackMap(DEF_PACK_REGISTER_RQ)    = &CLogic::RegisterRq;
    NetPackMap(DEF_PACK_LOGIN_RQ)       = &CLogic::LoginRq;
    NetPackMap(DEF_PACK_JOIN_ZONE)       = &CLogic::JoinZoneRq;
    NetPackMap(DEF_PACK_LEAVE_ZONE)       = &CLogic::LeaveZoneRq;
    NetPackMap(DEF_LEAVE_ROOM_RQ)       = &CLogic::LeaveRoomRq;

    NetPackMap(DEF_JOIN_ROOM_RQ)       = &CLogic::JoinRoomRq;
    NetPackMap(DEF_ZONE_INFO_RQ)       = &CLogic::ZoneInfoRq;

    NetPackMap(DEF_FIL_ROOM_READY)  = &CLogic::FIL_MsgSendRq;
    NetPackMap(DEF_FIL_GAME_START)  = &CLogic::FIL_MsgSendRq;
    NetPackMap(DEF_FIL_AI_BEGIN)   = &CLogic::FIL_MsgSendRq;
    NetPackMap(DEF_FIL_AI_END)     = &CLogic::FIL_MsgSendRq;
    NetPackMap(DEF_FIL_DISCARD_THIS)= &CLogic::FIL_MsgSendRq;
    NetPackMap(DEF_FIL_SURREND) = &CLogic::FIL_MsgSendRq;
    NetPackMap(DEF_FIL_WIN)         = &CLogic::FIL_MsgSendRq;

    NetPackMap(DEF_FIL_PIECEDOWN)         = &CLogic::FIL_PieceRq;
}

//注册
void CLogic::RegisterRq(sock_fd clientfd,char* szbuf,int nlen)
{
    printf("clientfd:%d RegisterRq\n", clientfd);
   
    //解析数据包 获取tel passwd name
    STRU_REGISTER_RQ* rq = (STRU_REGISTER_RQ *)szbuf;
    STRU_REGISTER_RS rs;
    //根据tel 查数据库 看有没有
    list<string>lstRes;
    char sqlStr[1000] = "";
    sprintf(sqlStr,"select tel from t_user where tel = %s",rq->m_tel);
    m_sql->SelectMysql(sqlStr,1,lstRes);
    //有 返回结果

    if(lstRes.size()>0)
    {
        rs.m_lResult = tel_is_exist;
    }

    else{
       //没有 接下来看昵称 有没有
        lstRes.clear();
        sprintf(sqlStr,"select name from t_user where name = %s",rq->m_name);
        m_sql->SelectMysql(sqlStr,1,lstRes);
       //有返回结果
        if(lstRes.size()>0)
            rs.m_lResult = name_is_exist;
       //没有 注册成功 更新数据库 写表
        else
        {
            rs.m_lResult = register_success;
            sprintf(sqlStr,"insert into t_user(tel,password,name) values(%s,'%s','%s');",rq->m_tel,rq->m_password,rq->m_name);
            cout<<sqlStr<<endl;
            int res = m_sql->UpdataMysql(sqlStr);
            if(!res)
            {
                rs.m_lResult = register_fail;
                cout<<"register failed"<<endl;
            }
        }
       //返回结果
        SendData(clientfd,(char *)&rs,sizeof(rs));
    }
}

//登录
void CLogic::LoginRq(sock_fd clientfd ,char* szbuf,int nlen)
{
    printf("clientfd:%d LoginRq\n", clientfd);

//    STRU_LOGIN_RS rs;
//    rs.m_lResult = password_error;
//    SendData( clientfd , (char*)&rs , sizeof rs );

    //拆包 获取 tel password
    STRU_LOGIN_RQ* rq = (STRU_LOGIN_RQ*)szbuf;
    STRU_LOGIN_RS rs;
    //根据 tel 查 id password name
    char sqlstr[1000] = "";
    list<string>lstRes;
    sprintf(sqlstr,"select id,password,name from t_user where tel = '%s';",rq->m_tel);
    m_sql->SelectMysql(sqlstr,3,lstRes);

    if(lstRes.size() == 0)
    {
        //没有 返回结果
        rs.m_lResult = user_not_exist;
    }
    else{
        int id = atoi(lstRes.front().c_str());
        lstRes.pop_front();
        string strPasswd = lstRes.front();
        lstRes.pop_front();
        string strName = lstRes.front();
        lstRes.pop_front();
        //有 看密码是否一致
        if(strcmp(rq->m_password,strPasswd.c_str())!=0)
        {
            //不一致 返回结果
            rs.m_lResult = password_error;
        }
        else
        {
            //一致
            rs.m_lResult = login_success;
            UserInfo *info = nullptr;
            //如果之前有用户信息 强制下线 先回收
            if(m_mapIdToUserInfo.find(id,info))
            {
                //强制下线

                //回收
                m_mapIdToUserInfo.erase(id);
                delete info;
            }
            //把id和套接字捆绑在一起
            //保存用户信息
            info = new UserInfo;
            info->m_id = id;
            info->m_sockfd = clientfd;
            strcpy(info->m_userName,strName.c_str());
            strcpy(rs.m_name,strName.c_str());
            rs.m_userid = id;
            //把id和套接字 捆绑在一起
            m_mapIdToUserInfo.insert(id,info);
            cout<<rs.m_lResult<<endl;
            //返回结果 id name result
            SendData(clientfd,(char *)&rs,sizeof(rs));

            //好友列表

            //离线信息

            //推送信息
            return;
        }

    }
    SendData(clientfd,(char *)&rs,sizeof(rs));
}

void CLogic::JoinZoneRq(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("clientfd:%d JoinZoneRq\n",clientfd);

    //拆包 更新信息
    STRU_JOIN_ZONE *rq = (STRU_JOIN_ZONE*)szbuf;
    UserInfo *info = nullptr;
    if(!m_mapIdToUserInfo.find(rq->userid,info))
        return;
    info->m_zoneid = rq->zoneid;
}

//加入房间请求，加入时可能多个线程同时又客户端请求，房间列表应加锁处理
void CLogic::JoinRoomRq(sock_fd clientfd, char *szbuf, int nlen)
{
//    printf("clientfd:%d JoinRoomRq\n",clientfd);
    //拆包
    STRU_JOIN_ROOM_RQ* rq = (STRU_JOIN_ROOM_RQ*)szbuf;
    STRU_JOIN_ROOM_RS rs;
    pthread_mutex_lock(&m_roomListMutex);
    list<int> tmplist,&userlist = m_roomUserlist[rq->roomid];
    switch(userlist.size())
    {
    case 0:
        rs.result = 1;
        rs.roomid = rq->roomid;
        rs.status = _host;
        rs.userid = rq->userid;
        userlist.push_back(rq->userid);
        break;
    case 1:
        rs.result = 1;
        rs.roomid = rq->roomid;
        rs.status = _player;
        rs.userid = rq->userid;
        userlist.push_back(rq->userid);
        break;
    case 2:
        rs.result = 0;
        break;
    default:
        rs.result = 0;
        break;
    }
    tmplist = userlist;
    pthread_mutex_unlock(&m_roomListMutex);
    SendData(clientfd,(char *)&rs,sizeof(rs));



    //首先 0 - 120 数组 看房间里面人数
    //0 加入房间的就是房主 加入到房间列表 返回
    //1 玩家 返回加入成功 需要同步信息 玩家给房主发 房主给玩家发 加入房间列表


    //玩家给加入的人发，加入的人给玩家发
    if(tmplist.size() > 0)
    {
        int loginid = rq->userid;
        UserInfo *loginInfo = nullptr;

        if(!m_mapIdToUserInfo.find(loginid,loginInfo))
            return;
        //写成员信息的请求
        STRU_ROOM_MEMBER loginrq;
        loginrq.userid = loginid;
        loginrq.status = rs.status;
        strcpy(loginrq.name,loginInfo->m_userName);
        bool flag = false;
        for(auto ite = tmplist.begin();ite !=tmplist.end();ite++)
        {
            int status = _player;
            if(!flag)
            {
                status = _host;
                flag = true;
            }
            int roomMemid = *ite;
            if(roomMemid != loginid)
            {
                //根据id 拿到用户信息
                UserInfo *MemInfo = nullptr;
                if(!m_mapIdToUserInfo.find(roomMemid,MemInfo))
                       return;
                //写成员信息的请求
                STRU_ROOM_MEMBER Memrq;
                strcpy(Memrq.name,MemInfo->m_userName);
                Memrq.status = status;
                Memrq.userid = roomMemid;
                //彼此发送
                SendData(loginInfo->m_sockfd,(char *)&Memrq,sizeof(Memrq));
                SendData(MemInfo->m_sockfd,(char *)&loginrq,sizeof(loginrq));
            }
            else
            {
                SendData(loginInfo->m_sockfd,(char *)&loginrq,sizeof(loginrq));
            }
        }
    }
    //2 加入失败

}

void CLogic::LeaveRoomRq(sock_fd clientfd, char *szbuf, int nlen)
{
   printf("clientfd:%d LeaveRoomRq\n",clientfd);
   //拆包
    STRU_LEAVE_ROOM_RQ *rq = (STRU_LEAVE_ROOM_RQ*)szbuf;
   //谁 什么身份 离开那个房间
    int leaveid = rq->userid;
    list<int>&lst = m_roomUserlist[rq->roomid];

    for(auto ite = lst.begin();ite != lst.end();ite++){
        //根据身份不同 房主 player list 房间信息
        int memid  = *ite;
        if(leaveid!=memid)
        {
            UserInfo* memInfo = nullptr;
            if(!m_mapIdToUserInfo.find(memid,memInfo))
                continue;
            SendData(memInfo->m_sockfd,szbuf,nlen);
        }
    }
    pthread_mutex_lock(&m_roomListMutex);
    if(rq->status == _host)
        lst.clear();
    else if(rq->status == _player)
    {
        for(auto ite = lst.begin();ite!=lst.end();ite++)
        {
            if(leaveid == *ite)
            {
                ite = lst.erase(ite);
                break;
            }
        }
    }
    pthread_mutex_unlock(&m_roomListMutex);

}

void CLogic::ZoneInfoRq(sock_fd clientfd, char *szbuf, int nlen)
{

//    printf("clientfd:%d ZoneInfoRq\n",clientfd);
    //拆包
    STRU_ZONE_INFO_RQ *rq = (STRU_ZONE_INFO_RQ *)szbuf;
    STRU_ZONE_ROOM_INFO rs;
    rs.zoneid = rq->zoneid;
    //根据专区拿到房间列表
    for(unsigned int i=1;i<m_roomUserlist.size();i++){
        list<int>& lst = m_roomUserlist[i];
        rs.roominfo[i] = lst.size();
    }
    SendData(clientfd,(char*)&rs,sizeof(rs));
}

void CLogic::LeaveZoneRq(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("clientfd:%d LeaveZoneRq\n",clientfd);
    //拆包
    STRU_LEAVE_ZONE *rq = (STRU_LEAVE_ZONE *)szbuf;
    UserInfo *info = nullptr;
    if(!m_mapIdToUserInfo.find(rq->userid,info))
        return;
    info->m_zoneid = 0;
}

//服务器转发五子棋游戏命令给房间成员
void CLogic::FIL_MsgSendRq(sock_fd clientfd,char *szbuf,int nlen)
{
    printf("clientfd:%d FIL_MsgSendRq\n",clientfd);

    //拆包
    STRU_FIL_RQ * rq = (STRU_FIL_RQ*)szbuf;
    //什么专区 什么房间 谁 发了什么
    //根据专区 拿到房间列表 根据房间 拿到房间内成员 转发给房间里所有人

    list<int>&lst = m_roomUserlist[rq->roomid];
    for(int i:lst)
    {
        int userid = i;
        UserInfo* info = nullptr;
        if(!m_mapIdToUserInfo.find(userid,info))
            continue;
        SendData(info->m_sockfd,szbuf,nlen);
    }

}

//转发落子命令给其他成员
void CLogic::FIL_PieceRq(sock_fd clientfd, char *szbuf, int nlen)
{
//    printf("clientfd:%d FIL_PieceRq\n",clientfd);
    //与FIL_MsgSendRq基本相同
    STRU_FIL_PIECEDOWN *rq = (STRU_FIL_PIECEDOWN*)szbuf;

    list<int>& lst = m_roomUserlist[rq->roomid];
    for(int i:lst)
    {
        UserInfo* info = nullptr;
        if(!m_mapIdToUserInfo.find(i,info))
            continue;
        SendData(info->m_sockfd,szbuf,nlen);
    }
}

