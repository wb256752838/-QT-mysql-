#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include <QDialog>

namespace Ui {
class AddBookDialog;
}

class AddBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBookDialog(QWidget *parent = nullptr);
    ~AddBookDialog();

    // 获取用户输入
    QString getId() const;
    QString getName() const;
    QString getCount() const;
    QString getPrice() const;
    QString getPublisher() const;
    QString getAuthor() const;

private:
    Ui::AddBookDialog *ui;
};

#endif // ADDBOOKDIALOG_H
