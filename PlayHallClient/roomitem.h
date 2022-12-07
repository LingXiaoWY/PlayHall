#ifndef ROOMITEM_H
#define ROOMITEM_H

#include <QDialog>

namespace Ui {
class RoomItem;
}

class RoomItem : public QDialog
{
    Q_OBJECT
signals:
    void SIG_JoinRoom(int id);
public:
    explicit RoomItem(QWidget *parent = 0);
    ~RoomItem();

    void setInfo(int roomid);
    void setRoomItem(int num);
private slots:
    void on_pb_join_clicked();

private:
    Ui::RoomItem *ui;
    int m_roomid;

    friend class Ckernel;
};

#endif // ROOMITEM_H
