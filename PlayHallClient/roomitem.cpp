#include "roomitem.h"
#include "ui_roomitem.h"
#include <QDebug>

RoomItem::RoomItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomItem),m_roomid(0)
{
    ui->setupUi(this);
}

RoomItem::~RoomItem()
{
    delete ui;
}



void RoomItem::setInfo(int roomid)
{
    m_roomid = roomid;
    QString txt = QString("五子棋-%1房").arg(roomid);
    ui->lb_title->setText(txt);
}

void RoomItem::setRoomItem(int num)
{
    QPixmap ready1 = QPixmap(":/icon/avatar_0.png").copy(0,0,230,280);
    QPixmap ready2 = QPixmap(":/icon/avatar_2.png").copy(0,0,230,280);
    QPixmap wait = QPixmap(":/icon/slotwait.png");
    switch(num)
    {
    case 0:
        ui->lb_player1->setPixmap(wait);
        ui->lb_player2->setPixmap(wait);
        break;
    case 1:
        ui->lb_player1->setPixmap(ready1);
        ui->lb_player2->setPixmap(wait);
        break;
    case 2:
        ui->lb_player1->setPixmap(ready1);
        ui->lb_player2->setPixmap(ready2);
        break;
    }
}

void RoomItem::on_pb_join_clicked()
{
    Q_EMIT SIG_JoinRoom(m_roomid);
}
