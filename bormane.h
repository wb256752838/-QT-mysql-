#ifndef BORMANE_H
#define BORMANE_H

#include <QWidget>
#include<QStandardItemModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
namespace Ui {
class borMane;
}

class borMane : public QWidget
{
    Q_OBJECT

public:
    explicit borMane(QSqlDatabase &d);
    ~borMane();
    virtual void closeEvent(QCloseEvent *event);
private slots:
    void on_rueBut_clicked();

    void on_delBut_clicked();

    void on_addBut_clicked();

    void on_findBut_clicked();


private:
    Ui::borMane *ui;
    QSqlDatabase db;
    QStandardItemModel m_model;
    QVector<QStringList> getUsers(QString strCondition="");
    void getAll();
    void AddUser(QVector<QStringList>);
    void delUser(QString strID);
    QSqlTableModel * d_model;
};

#endif // BORMANE_H
