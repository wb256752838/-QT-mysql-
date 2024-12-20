#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H
#include <QDialog>

namespace Ui {
class AddUserDialog;
}

class AddUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddUserDialog(QWidget *parent = nullptr);
    ~AddUserDialog();

    QString getId() const;
    QString getName() const;
    QString getClass() const;
    QString getAge() const;
    QString getSex() const;
    QString getPhone() const;

private:
    Ui::AddUserDialog *ui;
};

#endif // ADDUSERDIALOG_H
