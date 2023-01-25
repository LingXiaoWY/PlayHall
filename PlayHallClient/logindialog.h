#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    void setUI();
    void clearRegister();

signals:
    void SIG_LoginCommit(QString tel,QString password);
    void SIG_RegisterCommit(QString tel,QString password,QString name);
    void SIG_close();

private slots:
    void on_pb_register_clicked();

    void on_pb_clear_register_clicked();

    void on_pb_login_clicked();

    void on_pb_clear_clicked();

    void closeEvent(QCloseEvent *e);
private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
