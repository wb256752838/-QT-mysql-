#ifndef BORHISPAGE_H
#define BORHISPAGE_H
#include <QWidget>
#include "ui_borhispage.h" // 确保这是正确的 UI 头文件名
#include <QWidget>
#include<QStandardItemModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
namespace Ui {
class borhispage;
}

class borhispage : public QWidget
{
    Q_OBJECT

public:
    enum Mode {
        bNameMode = 0,
        cNameMode = 1,
        bNoMode = 2
    };
    explicit borhispage(QSqlDatabase &d,QString IdIn);
    ~borhispage();
    virtual void closeEvent(QCloseEvent *event);
private slots:
    void on_rueBut_clicked();
    void on_findBut_clicked();
    void on_borBut_clicked();
    void on_comboBox_currentTextChanged(const QString &arg1);
private:
    Ui::borhispage *ui;
    QSqlDatabase db;
    QString myid;
    QStandardItemModel m_model;
    Mode nowMode;
    void getAll();
    QSqlTableModel * d_model;
};

#endif // BORHISPAGE_H

