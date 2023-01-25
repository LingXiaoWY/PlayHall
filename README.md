# PlayHall
1.开发环境：客户端：Windows下QT，服务端：Ubuntu20.04下</br></br>
2.协议：自定义协议 传输层 TCP、UDP</br></br>
3.服务器基于高并发Server,能够处理万次请求</br></br>
4.基础功能：登录系统、房间系统、游戏系统、聊天系统</br>
登录系统：注册、登录、显示游戏大厅，进入游戏专区</br>
房间系统：选定房间、创建房间快速匹配、加入房间 离开房间</br>
游戏系统：游戏的管理、准备、开局、AI托管</br></br>
5.登录系统实现：client通过与server进行交互，查询数据库，验证登录/注册用户是否存在，注册时，MD5加密算法将用户密码进行加密，把明文变密文</br></br>
6.房间系统实现：编写一个房间类，当有人创建房间时构造房间，加入到房间链表中，用户可以根据自己的喜好加入相应的房间，当所有房主离开时，房间销毁。</br></br>
7.游戏系统实现：当前开发的游戏大厅中仅有五子棋一个游戏，为双人对局，当双方准备就绪时，房主就可以开始游戏，在对局过程中，任意一方都可以进行投降、托管（不能都为托管状态），当一方投降或者胜利时结束游戏，，使用到的技术有：通过点击事件来触发信号，槽函数接收并处理信号，五子棋获胜的判定，AI的编写</br>

流程图
-------------
<div align=center><img src="https://img9999.wchunge.cn/i/2022/12/08/pjz77i.jpg" height="765"/> </div>

类图
-------------
<div align=center><img src="https://img.fwfly.com/img/2023/01/23/5158p.png" height="765"/> </div>

使用客户端时，修改服务器IP地址
-------------
<div align=center><img src="https://img.fwfly.com/img/2023/01/25/ib9fj5.png" height="765"/> </div>

MySQL数据库建表信息
-------------
<div align=center><img src="https://img.fwfly.com/img/2023/01/25/ib9m5z.jpg" height="400"/> </div>

流程图直链网址：https://img9999.wchunge.cn/i/2022/12/08/pjz77i.jpg</br>
类图直链网址：https://img.fwfly.com/img/2023/01/23/5158p.png</br>
B站实机演示网址：https://www.bilibili.com/video/BV1w3411D75y/</br>
