#include "AddBookDialog.h"
#include "ui_addbookdialog.h"

AddBookDialog::AddBookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBookDialog)
{
    ui->setupUi(this);
    setWindowTitle("添加图书");
    // 确认按钮绑定 accept() 槽
      connect(ui->but_ent, &QPushButton::clicked, this, &AddBookDialog::accept);

        // 取消按钮绑定 reject() 槽
      connect(ui->but_cel, &QPushButton::clicked, this, &AddBookDialog::reject);
}

AddBookDialog::~AddBookDialog()
{
    delete ui;
}

QString AddBookDialog::getId() const {
    return ui->lineEditId->text();
}

QString AddBookDialog::getName() const {
    return ui->lineEditName->text();
}

QString AddBookDialog::getCount() const {
    return ui->lineEditCount->text();
}

QString AddBookDialog::getPrice() const {
    return ui->lineEditPrice->text();
}

QString AddBookDialog::getPublisher() const {
    return ui->lineEditPublisher->text();
}

QString AddBookDialog::getAuthor() const {
    return ui->lineEditAuthor->text();
}
