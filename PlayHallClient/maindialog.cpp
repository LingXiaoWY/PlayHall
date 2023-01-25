#include "maindialog.h"
#include "ui_maindialog.h"
#include "QMessageBox"

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::closeEvent(QCloseEvent *e)
{
    if(QMessageBox::question(this,"退出","是否退出") == QMessageBox::Yes)
    {
        //发信号
        Q_EMIT SIG_close();
        //同意关闭事件
        e->accept();
    }
    else
    {
        //忽略关闭事件
        e->ignore();
    }
}

#include"packdef.h"
void MainDialog::on_pb_fiveinline_clicked()
{
    //发信号
    Q_EMIT SIG_JoinZone(Five_In_Line);
    this->hide();
}
