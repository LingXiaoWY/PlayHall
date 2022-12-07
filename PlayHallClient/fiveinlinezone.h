#ifndef FIVEINLINEZONE_H
#define FIVEINLINEZONE_H

#include <QDialog>
#include "roomitem.h"
#include "QGridLayout"
#include <QCloseEvent>

namespace Ui {
class FiveInLineZone;
}

class FiveInLineZone : public QDialog
{
    Q_OBJECT

signals:
    void SIG_close();
    void SIG_joinRoom(int id);
public:
    explicit FiveInLineZone(QWidget *parent = 0);
    ~FiveInLineZone();
    void closeEvent(QCloseEvent *event);
    void setInfo(QString userName);
    std::vector<RoomItem*>& getVecRoomItem();
private:
    Ui::FiveInLineZone *ui;
    QGridLayout* m_layout;

    std::vector<RoomItem*>m_vecRoomItem;
};

#endif // FIVEINLINEZONE_H
