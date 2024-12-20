#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QFileDialog>
#include "adduserdialog.h"
#include "modpwd.h"
#include <addbookdialog.h>
namespace Ui {
class admin;
}

class admin : public QWidget
{
    Q_OBJECT

public:
    explicit admin(QWidget *parent = nullptr);\
    explicit admin(QSqlDatabase &d);//构造函数增加一个参数
                                        //是一个 QSqlDatabase引用
    ~admin();
   virtual void closeEvent(QCloseEvent *event);
private slots:
    void switchPage();

    void setupBookModelAndView();

    void on_but_ent_mod_clicked();

    void on_but_search_row_clicked();

    void on_but_add_data_clicked();

    void on_but_echo_data_clicked();

    void on_but_remove_row_usr_clicked();

    void on_but_add_row_usr_clicked();

    void on_but_usr_manage_clicked();

    void on_but_book_manage_clicked();

    void on_but_to_pwd_clicked();

    void on_but_bsch_clicked();

    void on_but_bookcharge_clicked();

    void on_but_book_delete_clicked();

    void on_but_bookadd_clicked();

    void on_but_borrow_manage_clicked();

    void on_but_reflash_clicked();

private:
    Ui::admin *ui;
    QSqlDatabase &db;
    QSqlTableModel *model; // 声明 model
     QSqlTableModel *bookModel;
};

#endif // ADMIN_H
