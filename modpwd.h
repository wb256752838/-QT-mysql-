#ifndef MODPWD_H
#define MODPWD_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
namespace Ui {
class modpwd;
}

class modpwd : public QWidget
{
    Q_OBJECT

public:
    explicit modpwd(QWidget *parent = nullptr);
    explicit modpwd(QSqlDatabase &d);//构造函数增加一个参数
                                        //是一个 QSqlDatabase引用
    ~modpwd();
     virtual void closeEvent(QCloseEvent *event);
private slots:
    void on_but_search_row_clicked();

    void on_but_add_row_usr_clicked();

    void on_but_remove_row_usr_clicked();

    void on_but_ent_mod_clicked();

    void on_but_to_pwd_clicked();

private:
    Ui::modpwd *ui;
    QSqlDatabase &db;
    QSqlTableModel *model; // 声明 model
};

#endif // MODPWD_H
