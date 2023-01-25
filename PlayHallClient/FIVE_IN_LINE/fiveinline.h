#ifndef FIVEINLINE_H
#define FIVEINLINE_H

#include <QWidget>
#include <vector>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class FiveInLine;
}

//界面是580×580 固定

//外边距
#define FIL_MARGIN_HEIGHT (50)
#define FIL_MARGIN_WIDTH  (50)

//行列数 五子棋规范为15 × 15
#define FIL_COLS          (15)
#define FIL_ROWS          (15)

//边与边之间的边距
#define FIL_SPACE         (30)

//棋子的大小
#define FIL_PIECE_SIZE    (20)

//棋盘边缘缩进的距离
#define FIL_DISTANCE      (10)

//1. 首先绘制棋盘 网格线 棋子
//2. 鼠标点击，出现棋子，然后鼠标移动，棋子跟着移动，鼠标释放，棋子落子
//需要涉及到鼠标点击 鼠标移动 鼠标释放
//3. 定时刷新界面 1秒 25帧 重绘25次
//4.1 落子 出界 要放弃 合法的 发送信号
//4.2 落子的槽函数 对更新数组 切换回合
//5. 判断输赢

//关于输赢
//1. 根据当前点，

//对局定时器
#define DEF_TIMER         (30)

struct stru_win
{
    stru_win():board(FIL_COLS,std::vector<int>(FIL_ROWS,0)),playerCount(0),cpuCount(0){}

    void clear(){
        playerCount = 0;
        cpuCount = 0;
    }
    std::vector<std::vector<int>>board;
    int playerCount; //该赢法玩家的棋子个数
    int cpuCount; //该赢法电脑的棋子个数
};


class FiveInLine : public QWidget
{
    Q_OBJECT

signals:
    //落子信号：鼠标释放 捕捉落点位置
    void SIG_pieceDown(int blackorwhite,int x,int y); //什么颜色的子落到了什么位置
    void SIG_playerWin(int blackorwhite);
public slots:
    void slot_pieceDown(int blackorwhite,int x,int y);
    void slot_startGame();
    void slot_countTimer();
public:
    explicit FiveInLine(QWidget *parent = 0);
    ~FiveInLine();

    //重绘事件：绘图背景 棋盘 棋子等
    void paintEvent(QPaintEvent *event);

    //鼠标点击 出现棋子
    void mousePressEvent(QMouseEvent *event);
    //鼠标移动 棋子移动
    void mouseMoveEvent(QMouseEvent *event);
    //鼠标释放 棋子落子
    void mouseReleaseEvent(QMouseEvent *event);
    //获取当前是黑白回合
    int getBlackOrWhite() const;

    //是否出界
    bool isCrossLine(int x,int y);

    //切换回合函数
    void changeBlackAndWhite();

    //判断是否胜利
    bool isWin(int x,int y);

    //清空
    void clear();

    //五子棋AI
    //初始化所有赢法棋子布局
    void InitAiVector();

    //电脑落子
    void pieceDownByCpu();

    //设置电脑颜色
    void setCpuColor(int newCpuColor);


    void setSelfStatus(int _Status);
public:
    enum ENUM_BLACK_OR_WHITE{None = 0,Black,White};
private:
    Ui::FiveInLine *ui;
    //记录当前是黑子白子的回合
    int m_blackOrWhite;
    //鼠标移动中鼠标所在位置
    QPoint m_movePoint;
    //移动标志 标志棋子是否随着鼠标移动的，鼠标是否是一个一直在点击的状态
    bool m_moveFlag;


    //存棋盘上的棋子的二维数组
    std::vector<std::vector<int>>m_board;

    //判断输赢时需要的方向
    enum enum_direction{d_z,d_y,d_s,d_x,d_zs,d_yx,d_zx,d_ys,d_count};

    //棋子颜色
    QBrush m_pieceColor[3];

    //重绘定时器
    QTimer m_timer;

    //结束标志
    bool m_isOver;


    //根据方向对坐标的偏移，每次是一个单位
    int dx[d_count] = {-1,1,0,0,-1,1,-1,1};
    int dy[d_count] = {0,0,-1,1,-1,1,1,-1};

    //网络版本 玩家身份 不是自己回合不能动
    int m_status;

    //五子棋AI
    std::vector<stru_win>m_vecWin;

    //电脑的回合颜色
    int m_cpuColor;

    //回合定时
    //每秒的定时
    QTimer m_countTimer;
    int m_colorCounter;
};

#endif // FIVEINLINE_H
