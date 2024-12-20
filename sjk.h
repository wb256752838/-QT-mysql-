#ifndef SJK_H
#define SJK_H

#include <QWidget>
#include <QSqlDatabase>
namespace Ui {
class sjk;
}

class sjk : public QWidget
{
    Q_OBJECT

public:
    explicit sjk(QWidget *parent = nullptr);
    ~sjk();
    bool usr_mode=0;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
private slots:
    void on_butregister_clicked();

    void on_butlogin_clicked();

    void on_usr_switch_currentTextChanged(const QString &arg1);

private:
    Ui::sjk *ui;
};

#endif // SJK_H
