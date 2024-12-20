#ifndef S_BOOK_H
#define S_BOOK_H

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
#include <QStandardItemModel>
namespace Ui {
class s_book;
}

class s_book : public QWidget
{
    Q_OBJECT

public:
    int id;

    enum Mode {
        bNameMode = 0,
        cNameMode = 1,
        bNoMode = 2
    };
    explicit s_book(QWidget *parent = nullptr);
    explicit s_book(QSqlDatabase &d);//构造函数增加一个参数
    explicit s_book(QSqlDatabase &d,QString IdIn);
    ~s_book();
     virtual void closeEvent(QCloseEvent *event);
private slots:
    void on_rueBut_clicked();

    void on_delBut_clicked();

    void on_addBut_clicked();

    void on_findBut_clicked();
    void on_borBut_clicked();
    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_imBut_clicked();

    void on_outBut_clicked();

    void on_pushButton_clicked();


private:
    Ui::s_book *ui;
    QSqlDatabase db;
    QString myid;
    QStandardItemModel m_model;
    Mode nowMode;
    QVector<QStringList> getUsers(QString strCondition="");
    void getAll();
    void AddUser(QVector<QStringList>);
    void delUser(QString strID);
    QSqlTableModel * d_model;
};


#endif // S_BOOK_H
