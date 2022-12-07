#pragma once

#define _DEF_BUFFER  ( 4096 )
#define _DEF_CONTENT_SIZE	(1024)
#define MAX_SIZE (40)

//自定义协议   先写协议头 再写协议结构
//登录 注册 获取好友信息 添加好友 聊天 发文件 下线请求
#define DEF_PACK_BASE	(10000)
#define _DEF_PROTOCOL_COUNT (100)

//注册
#define _DEF_PACK_REGISTER_RQ	(DEF_PACK_BASE + 0 )
#define _DEF_PACK_REGISTER_RS	(DEF_PACK_BASE + 1 )

//登录
#define _DEF_PACK_LOGIN_RQ	(DEF_PACK_BASE + 2 )
#define _DEF_PACK_LOGIN_RS	(DEF_PACK_BASE + 3 )


//注册请求结果
#define tel_is_exist		(0)
#define register_success	(1)
#define name_is_exist       (2)


//返回的结果
//登录请求的结果
#define user_not_exist		(0)
#define password_error		(1)
#define login_success		(2)

//添加好友的结果
#define no_this_user		(0)
#define user_refuse			(1)
#define user_offline		(2)
#define add_success			(3)

typedef int PackType;

//协议结构
//注册
typedef struct STRU_REGISTER_RQ
{
    STRU_REGISTER_RQ():type(_DEF_PACK_REGISTER_RQ)
    {
        memset( tel  , 0, sizeof(tel));
        memset( name  , 0, sizeof(name));
        memset( password , 0, sizeof(password) );
    }
    //需要手机号码 , 密码, 昵称
    PackType type;
    char tel[MAX_SIZE];
    char name[MAX_SIZE];
    char password[MAX_SIZE];

}STRU_REGISTER_RQ;

typedef struct STRU_REGISTER_RS
{
    //回复结果
    STRU_REGISTER_RS(): type(_DEF_PACK_REGISTER_RS) , result(register_success)
    {
    }
    PackType type;
    int result;

}STRU_REGISTER_RS;

//登录
typedef struct STRU_LOGIN_RQ
{
    //登录需要: 手机号 密码
    STRU_LOGIN_RQ():type(_DEF_PACK_LOGIN_RQ)
    {
        memset( tel , 0, sizeof(tel) );
        memset( password , 0, sizeof(password) );
    }
    PackType type;
    char tel[MAX_SIZE];
    char password[MAX_SIZE];

}STRU_LOGIN_RQ;

typedef struct STRU_LOGIN_RS
{
    // 需要 结果 , 用户的id
    STRU_LOGIN_RS(): type(_DEF_PACK_LOGIN_RS) , result(login_success),userid(0)
    {
        memset(name,0,sizeof(name));
    }
    PackType type;
    int userid;
    int result;
    char name[MAX_SIZE];
}STRU_LOGIN_RS;


////////////  游戏相关内容  /////////////
#define DEF_PACK_JOIN_ZONE (DEF_PACK_BASE + 4)
#define DEF_PACK_LEAVE_ZONE (DEF_PACK_BASE + 5)

enum ENUM_PLAY_ZONE{Five_In_Line = 0x10,E_L_S,D_D_Z};
//加入专区
struct STRU_JOIN_ZONE//解决这是什么包，谁加入哪个专区
{
    STRU_JOIN_ZONE():type(DEF_PACK_JOIN_ZONE),userid(0),zoneid(0){}
    PackType type;
    int userid;
    int zoneid;
};

//退出专区
struct STRU_LEAVE_ZONE //解决这是什么包，退出专区
{
    STRU_LEAVE_ZONE():type(DEF_PACK_LEAVE_ZONE),userid(0){}
    PackType type;
    int userid;
};//发给服务器，服务器会同步房间成员信息

//房间 为了避免0 出现歧义（房间号是0，或者未初始化）所以房间号为1 - 120

//加入房间
#define DEF_JOIN_ROOM_RQ (DEF_PACK_BASE + 6)
#define DEF_JOIN_ROOM_RS (DEF_PACK_BASE + 7)
struct STRU_JOIN_ROOM_RQ
{
    STRU_JOIN_ROOM_RQ():type(DEF_JOIN_ROOM_RQ),userid(0),roomid(0){}
    PackType type;
    int userid;
    int roomid;
};


enum ENUM_ROOM_STATUS{_host,_player,_watcher};
struct STRU_JOIN_ROOM_RS
{
    STRU_JOIN_ROOM_RS():type(DEF_JOIN_ROOM_RS),userid(0),roomid(0),result(0){}
    PackType type;
    int userid;
    int roomid;
    int status;
    int result; // 0 - fail 1 -- success
};

#define DEF_ROOM_MEMBER (DEF_PACK_BASE + 8)
#define DEF_LEAVE_ROOM_RQ (DEF_PACK_BASE + 9)

//房间成员
struct STRU_ROOM_MEMBER//解决这是什么包，谁，哪个房间，叫什么名字
{
    STRU_ROOM_MEMBER():type(DEF_ROOM_MEMBER),userid(0)
    {
        status = _player;
        memset(name,0,sizeof(name));
    }
    PackType type;
    int userid;
    int status;// 房间内的身份
    char name[MAX_SIZE];
    //int state; 当前状态 1.准备状态 2.游戏中 3.空闲
};

//退出房间
struct STRU_LEAVE_ROOM_RQ//解决这是什么包，谁，退出了房间
{
    STRU_LEAVE_ROOM_RQ():type(DEF_LEAVE_ROOM_RQ),userid(0),status(_player){}
    PackType type;
    int userid;
    int roomid;
    int status;
};//会被转发，如果自己不是房主，房主退出，自己也跟着退出

//专区内每个房间人数
#define DEF_ZONE_ROOM_INFO (DEF_PACK_BASE + 10)
#define DEF_ZONE_INFO_RQ (DEF_PACK_BASE + 11)
#define DEF_ZONE_ROOM_NUM (121)

//请求
struct STRU_ZONE_INFO_RQ //解决这是什么包，请求专区每个房间人数
{
    STRU_ZONE_INFO_RQ():type(DEF_ZONE_INFO_RQ),zoneid(0){}
    PackType type;
    int zoneid;
};

struct STRU_ZONE_ROOM_INFO
{
    STRU_ZONE_ROOM_INFO():type(DEF_ZONE_ROOM_INFO),zoneid(0)
    {
        memset(roomInfo,0,sizeof(roomInfo));
    }

    PackType type;
    int zoneid;
    int roomInfo[DEF_ZONE_ROOM_NUM];
};

///////////////////////////////////

#define DEF_FIL_ROOM_READY     (DEF_PACK_BASE + 12)
#define DEF_FIL_GAME_START     (DEF_PACK_BASE + 13)
#define DEF_FIL_AI_BEGIN       (DEF_PACK_BASE + 14)
#define DEF_FIL_AI_END         (DEF_PACK_BASE + 15)
#define DEF_FIL_DISCARD_THIS   (DEF_PACK_BASE + 16)
#define DEF_FIL_SURREND        (DEF_PACK_BASE + 17)
#define DEF_FIL_PIECEDOWN      (DEF_PACK_BASE + 18)
#define DEF_FIL_WIN            (DEF_PACK_BASE + 19)

//确认协议
//准备 开始 托管 弃权（当前一次） 投降 落子

struct STRU_FIL_RQ
{
    STRU_FIL_RQ(PackType type):
        type(type),userid(0),
        zoneid(0),roomid(0){}
    PackType type; //准备 开始 胜利 托管 弃权（当前一次） 投降 复用
    int userid;
    int zoneid;
    int roomid;
};//只有知道 什么专区 什么房间 才能找到响应的人

//当所有人都准备好，需要开始
//都准备才能开始 两种方式 根据客户端判断 服务器发包


struct STRU_FIL_RS
{
    STRU_FIL_RS():
        type(),userid(0),
        zoneid(0),roomid(0){}
    PackType type;
    int userid;
    int zoneid;
    int roomid;
};

//落子
struct STRU_FIL_PIECEDOWN //什么专区的什么时间，谁在x,y位置放了一个什么子
{
    STRU_FIL_PIECEDOWN():
        type(DEF_FIL_PIECEDOWN),
        userid(0),zoneid(0),color(0),
        roomid(0),x(0),y(0){}
    PackType type; //落子
    int userid;
    int zoneid;
    int roomid;
    int color;
    int x,y;
};
