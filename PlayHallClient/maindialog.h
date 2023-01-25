#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include<QCloseEvent>

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT
signals:
    void SIG_close();
    void SIG_JoinZone(int zoneid);
public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

    void closeEvent(QCloseEvent *e);
private slots:
    void on_pb_fiveinline_clicked();

private:
    Ui::MainDialog *ui;
};

#endif // MAINDIALOG_H
