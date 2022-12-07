#include "fiveinlinezone.h"
#include "ui_fiveinlinezone.h"
#include <QMessageBox>
#include "packdef.h"
FiveInLineZone::FiveInLineZone(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FiveInLineZone)
{
    ui->setupUi(this);
    this->setWindowTitle("五子棋专区");
    m_layout = new QGridLayout;

    m_layout->setContentsMargins(0,0,0,0);
    ui->wdg_roomGrid->setLayout(m_layout);
    m_vecRoomItem.emplace_back(nullptr);
    for(int i=0;i<120;i++){
        RoomItem *item = new RoomItem;
        item->setInfo(i+1);
        m_vecRoomItem.emplace_back(item);
        connect(item,SIGNAL(SIG_JoinRoom(int)),this,SIGNAL(SIG_joinRoom(int)));
        m_layout->addWidget(item,i/2,i%2);
    }
}

FiveInLineZone::~FiveInLineZone()
{
    delete ui;
}

void FiveInLineZone::closeEvent(QCloseEvent * event)
{
    if(QMessageBox::question(this,"退出提示","是否退出专区") == QMessageBox::Yes)
    {
        Q_EMIT SIG_close();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void FiveInLineZone::setInfo(QString userName)
{
    ui->lb_name->setText(userName);
}

std::vector<RoomItem *> &FiveInLineZone::getVecRoomItem()
{
    return m_vecRoomItem;
}
