#include "fiveinline.h"
#include "ui_fiveinline.h"



FiveInLine::FiveInLine(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FiveInLine),m_board(FIL_COLS,std::vector<int>(FIL_ROWS,0))//多长 初值多少
  ,m_movePoint(0,0),m_moveFlag(false)
  ,m_blackOrWhite(Black)
  ,m_isOver(false)
  ,m_colorCounter(DEF_TIMER)
    //vector 初始化 有参构造 参数：多长 初值多少
{
    ui->setupUi(this);
    InitAiVector();
    m_pieceColor[None] = QBrush(QColor(0,0,0,0));//最后一个参数，表示透明度
    m_pieceColor[Black] = QBrush(QColor(0,0,0));
    m_pieceColor[White] = QBrush(QColor(255,255,255));

    connect(&m_timer,SIGNAL(timeout()),this,SLOT(repaint()));
    //初始将标题置空
    ui->lb_color->setText("");
    ui->lb_timer->setText("");
    ui->lb_winner->setText("");
    //repaint()会触发重绘
    m_timer.start(1000/40); //1秒25次
    //落子和落子处理 连接
    setCpuColor(None);
    connect(&m_countTimer,SIGNAL(timeout()),this,SLOT(slot_countTimer()));

}

FiveInLine::~FiveInLine()
{
    m_countTimer.stop();
    m_timer.stop();
    delete ui;
}

void FiveInLine::slot_pieceDown(int blackorwhite, int x, int y)
{
    //更新数组切换回合
    if(blackorwhite != getBlackOrWhite())
        return ;
   //落子 没有子才能放
    if(m_board[x][y] == None)
    {
        m_board[x][y] = blackorwhite;
        //更新该点棋子颜色后，判断输赢
        if(isWin(x,y))
        {

            m_countTimer.stop();
            ui->lb_timer->hide();
            ui->lb_color->setText("");
            QString str = getBlackOrWhite() == Black ? "黑方胜利" : "白方胜利";
            ui->lb_winner->setText(str);
        }
        else{
            if( m_cpuColor != getBlackOrWhite()){
                //更新玩家的每种赢法 玩家的棋子个数
                for(int i=0;i<m_vecWin.size();i++)
                {
                    if(m_vecWin[i].board[x][y] == 1){
                        m_vecWin[i].playerCount += 1;
                        m_vecWin[i].cpuCount = 100; //无效值
                    }
                }
            }
            else
            {
                //更新电脑的每种赢法 电脑的棋子个数
                for(int i=0;i<m_vecWin.size();i++)
                {
                    if(m_vecWin[i].board[x][y] == 1){
                        m_vecWin[i].cpuCount += 1;
                        m_vecWin[i].playerCount = 100; //无效值
                    }
                }
            }
            m_colorCounter = DEF_TIMER;
            //更换回合
            changeBlackAndWhite();
            //判断是否是电脑的回合
            //电脑下棋
            if(m_cpuColor == getBlackOrWhite())
                pieceDownByCpu();
        }
    }
}

void FiveInLine::slot_startGame()
{
    clear();
    m_isOver = false;
    m_countTimer.start(1000);
    ui->lb_color->setText("黑子回合");
    ui->lb_timer->setText(QString("%1秒").arg(DEF_TIMER));
}

void FiveInLine::slot_countTimer()
{
    m_colorCounter--;
    if(m_colorCounter < 0)
    {
        changeBlackAndWhite();
        m_colorCounter = DEF_TIMER;
        if(getBlackOrWhite() == m_cpuColor)
            pieceDownByCpu();
        return;
    }
    //界面文本更新
    ui->lb_timer->setText(QString("%1秒").arg(m_colorCounter,2,10,QChar('0')));
}


//重绘事件：绘图背景 棋盘 棋子等
void FiveInLine::paintEvent(QPaintEvent *event)
{
    //绘制棋盘 RGB 255 160 0
    QPainter painer(this); //QPainter 用于绘图，使用有参构造，传入当前对象，得到当前空间的
    painer.setBrush(QBrush(QColor(255,160,0)));//通过画刷设置颜色
    painer.drawRect(FIL_MARGIN_WIDTH - FIL_DISTANCE,
                    FIL_MARGIN_HEIGHT-FIL_DISTANCE,
                    (FIL_COLS + 1) * FIL_SPACE + 2 * FIL_DISTANCE,
                    (FIL_ROWS + 1) * FIL_SPACE + 2 * FIL_DISTANCE);
    //绘制网格线

    for(int i=1;i<=FIL_COLS;++i)
    {
        painer.setBrush(QBrush(QColor(0,0,0))); //通过画刷设置颜色
        QPoint p1(FIL_MARGIN_WIDTH + FIL_SPACE  * i,FIL_MARGIN_HEIGHT + FIL_SPACE);
        QPoint p2(FIL_MARGIN_WIDTH + FIL_SPACE  * i,FIL_MARGIN_HEIGHT + FIL_SPACE * FIL_ROWS);
        painer.drawLine(p1,p2);
    }

    for(int i=1;i<=FIL_ROWS;++i)
    {
        painer.setBrush(QBrush(QColor(0,0,0))); //通过画刷设置颜色
        QPoint p1(FIL_MARGIN_WIDTH + FIL_SPACE,FIL_MARGIN_HEIGHT + FIL_SPACE * i);
        QPoint p2(FIL_MARGIN_WIDTH + FIL_SPACE  * FIL_COLS,FIL_MARGIN_HEIGHT + FIL_SPACE * i);
        painer.drawLine(p1,p2);
    }

    //画中心点
    painer.setBrush(m_pieceColor[Black]); // 通过画刷设置颜色
    painer.drawEllipse(QPoint(290,290),3,3);

    //画棋子
    //x,y 点对应的颜色 --> m_pieceColor[ m_board[x][y] ]
    for(int x = 0; x < FIL_COLS; ++x)
    {
        for(int y=0;y<FIL_ROWS;++y)
        {
            if(m_board[x][y] != None){
                painer.setBrush(m_pieceColor[m_board[x][y]]); //通过画刷设置颜色
                painer.drawEllipse(
                QPoint(FIL_MARGIN_WIDTH + FIL_SPACE + FIL_SPACE * x,
                       FIL_MARGIN_HEIGHT + FIL_SPACE + FIL_SPACE * y),
                            FIL_PIECE_SIZE / 2, FIL_PIECE_SIZE / 2
                            );//画圆 中心点坐标 半径 x，半径 y
            }
        }
    }
    //显示移动的棋子 当前回合是谁 就画谁
    if(m_moveFlag)
    {
        //m_movePoint
        painer.setBrush(m_pieceColor[getBlackOrWhite()]);//通过画刷设置颜色
        painer.drawEllipse(QPoint(m_movePoint.x(),m_movePoint.y()),FIL_PIECE_SIZE / 2, FIL_PIECE_SIZE / 2);//画圆 中心点坐标 半径 x，半径 y
    }
    event->accept();
}

//鼠标点击 出现棋子
void FiveInLine::mousePressEvent(QMouseEvent *event)
{
    //捕捉点
    m_movePoint = event->pos();
    //位置坐标
    //相对坐标 (相对窗口的)
    //QMouseEvent::pos()
    //绝对坐标(相对win10桌面的)
    //QCursor::pos() 鼠标位置

    //加入结束判断
    if(m_isOver||m_status != getBlackOrWhite())
        goto quit;
    //QMouseEvent::globalPos();
    //点击状态
    m_moveFlag = true;
quit:
    event->accept();
}

//鼠标移动 棋子移动
void FiveInLine::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isOver)
        goto quit;
    if(m_moveFlag)
        m_movePoint = event->pos();

quit:
    event->accept();
}

//鼠标释放 棋子落子
void FiveInLine::mouseReleaseEvent(QMouseEvent *event)
{
    int x = 0;
    int y = 0;
    float fx = (float)event->pos().x();
    float fy = (float)event->pos().y();
    m_moveFlag = false;
    //4.1 落子 出界 要放弃
    //将相对坐标 换算为棋盘坐标
    //会涉及到 取整和四舍五入问题
    if(m_isOver||m_status != getBlackOrWhite())
        goto quit;
    //落子是四舍五入而不是取整
    fx = (fx - FIL_MARGIN_WIDTH - FIL_SPACE) / FIL_SPACE;
    fy = (fy - FIL_MARGIN_HEIGHT - FIL_SPACE) / FIL_SPACE;

    //类似于 4.4 4.5 4.6 根据 <0.5 和 > 0.5 分为两个情况
    x = fx - (int)fx > 0.5 ? ((int)fx + 1): (int)fx;
    y = fy - (int)fy > 0.5 ? ((int)fy + 1): (int)fy;

    //是否出界
    if(isCrossLine(x,y))
        return;
    Q_EMIT SIG_pieceDown(getBlackOrWhite(),x,y);
quit:
    event->accept();
}

int FiveInLine::getBlackOrWhite() const
{
    return m_blackOrWhite;
}

bool FiveInLine::isCrossLine(int x, int y)
{
    if(x < 0 || x >= 15 || y < 0 || y >= 15)
        return true;
    return false;
}

void FiveInLine::changeBlackAndWhite()
{
    //0, 1, 2
    m_blackOrWhite = m_blackOrWhite + 1;
    if(m_blackOrWhite == 3)
        m_blackOrWhite = Black;

    if(m_blackOrWhite == Black)
        ui->lb_color->setText("黑子回合");
    else
        ui->lb_color->setText("白子回合");
}

bool FiveInLine::isWin(int x, int y)
{
    //看四条直线 八个方向的同色棋子个数 只要有一个个数到5 就结束了
    int count = 1;
    for(int dr = d_z; dr < d_count;dr+=2){
        for(int i=1;i<=4;i++)
        {
            //获取偏移后的棋子坐标
            int ix = dx[dr] * i + x;
            int iy = dy[dr] * i + y;
            if(isCrossLine(ix,iy))
                break;
            if(m_board[ix][iy] == m_board[x][y])
                count++;
            else
                break;
        }
        for(int i=1;i<=4;i++)
        {
            //获取偏移后的棋子坐标
            int ix = dx[dr+1] * i + x;
            int iy = dy[dr+1] * i + y;
            if(isCrossLine(ix,iy))
                break;
            if(m_board[ix][iy] == m_board[x][y])
                count++;
            else
                break;
        }
        if(count >= 5)//不在
            break;
        else count = 1;
    }
    if(count >= 5)
    {
        m_isOver = true;
        return true;
    }
    return false;

}

void FiveInLine::clear()
{
    //默认不开AI 开的话 Black or White
    setCpuColor(None);
    //清空赢法棋子个数统计
    //1. 遍历赢法数组，每个清空
    for(int i=0;i<m_vecWin.size();++i)
        m_vecWin[i].clear();
    //状态位
    m_isOver = true;
    m_blackOrWhite = Black;
    m_moveFlag = false;
    //界面
    for(int i = 0;i < FIL_COLS;i++)
    {
        for(int j = 0;j < FIL_ROWS;j++)
        {
            m_board[i][j] = None;
        }
    }
    //初始将标题置空
    ui->lb_color->setText("");
    ui->lb_timer->setText("");
    ui->lb_winner->setText("");
    m_countTimer.stop();
    m_colorCounter = DEF_TIMER;
}

void FiveInLine::setSelfStatus(int _Status)
{
    m_status = _Status;
}


//初始化所有赢法棋子布局
void FiveInLine::InitAiVector()
{
    // --赢法
    for(int y=0;y<FIL_ROWS;y++)
    {
        for(int x=0;x<FIL_COLS-4;x++) //起点位置
        {
            stru_win w;
            for(int k=0;k<5;k++)
            {
                w.board[x+k][y] = 1;
            }
            m_vecWin.emplace_back(w);
        }
    }
    // |赢法
    for(int x=0;x<FIL_COLS;x++)
    {
        for(int y=0;y<FIL_ROWS-4;y++) //起点位置
        {
            stru_win w;
            for(int k=0;k<5;k++)
            {
                w.board[x][y+k] = 1;
            }
            m_vecWin.emplace_back(w);
        }
    }
    // /赢法
    for(int x=FIL_COLS - 1;x >= 4;--x)
    {
        for(int y=0;y<FIL_ROWS-4;y++) //起点位置
        {
            stru_win w;
            for(int k=0;k<5;k++)
            {
                w.board[x-k][y+k] = 1;
            }
            m_vecWin.emplace_back(w);
        }
    }
    // \赢法
    for(int x=0;x<FIL_COLS-4;x++)
    {
        for(int y=0;y<FIL_ROWS-4;y++) //起点位置
        {
            stru_win w;
            for(int k=0;k<5;k++)
            {
                w.board[x+k][y+k] = 1;
            }
            m_vecWin.emplace_back(w);
        }
    }
}

//电脑落子
void FiveInLine::pieceDownByCpu()
{
    if(m_isOver)
        return ;
    if(getBlackOrWhite() != m_status)
        return;
    int x = 0, y = 0, k = 0;
    int u = 0,v = 0;//最优目标坐标

    int MyScore[FIL_COLS][FIL_ROWS] = {};//记录玩家所有位置的分数
    int CpuScore[FIL_COLS][FIL_ROWS] = {};//记录电脑所有位置的分数
    int maxn = 0; //最高分数

    //估分 找到没有子的点，看每种赢法，对应的棋子个数，进行这个点的分数计算
    for( x = 0; x < FIL_COLS;++x)
    {
        for(y=0;y < FIL_ROWS;++y)
        {
            if(m_board[x][y] == None)
            {
                //遍历所有赢法
                for(k = 0;k < m_vecWin.size();++k)
                {
                    //评估玩家在x,y分数
                    if(m_vecWin[k].board[x][y] == 1)
                    {
                        //根据该赢法，棋子个数
                        switch(m_vecWin[k].playerCount)
                        {
                        case 1:
                            MyScore[x][y] += 200;
                            break;
                        case 2:
                            MyScore[x][y] += 400;
                            break;
                        case 3:
                            MyScore[x][y] += 2000;
                            break;
                        case 4:
                            MyScore[x][y] += 10000;
                            break;
                        }
                    }
                    //评估电脑在x,y分数
                    if(m_vecWin[k].board[x][y] == 1)
                    {
                        //根据该赢法，棋子个数
                        switch(m_vecWin[k].cpuCount)
                        {
                        case 1:
                            CpuScore[x][y] += 220;
                            break;
                        case 2:
                            CpuScore[x][y] += 420;
                            break;
                        case 3:
                            CpuScore[x][y] += 4100;
                            break;
                        case 4:
                            CpuScore[x][y] += 200000;
                            break;
                        }
                    }
                }
            }
        }
    }
    //估分之后找最优点 -- 一定在无子的点
    for( x = 0; x < FIL_COLS;++x)
    {
        for(y = 0;y < FIL_ROWS;++y)
        {
            if(m_board[x][y] == None)
            {

                //先看玩家 MyScore看做是防守 CpuScore看做是进攻
                if(MyScore[x][y] > maxn)
                {
                    maxn = MyScore[x][y];
                    u = x;
                    v = y;
                }
                else if(MyScore[x][y] == maxn)
                {
                    if(CpuScore[x][y] > CpuScore[u][v])
                    {
                        u = x;
                        v = y;
                    }
                }
                //再看电脑
                if(CpuScore[x][y] > maxn)
                {
                    maxn = CpuScore[x][y];
                    u = x;
                    v = y;
                }
                else if(CpuScore[x][y] == maxn)
                {
                    if(MyScore[x][y] > MyScore[u][v])
                    {
                        u = x;
                        v = y;
                    }
                }
            }
        }
    }
    //得到最优目标坐标
    Q_EMIT SIG_pieceDown(getBlackOrWhite(),u,v);

}

//设置电脑颜色
void FiveInLine::setCpuColor(int newCpuColor)
{
    m_cpuColor = newCpuColor;
}
