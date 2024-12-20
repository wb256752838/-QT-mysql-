#include "adduserdialog.h"
#include "ui_adduserdialog.h"


AddUserDialog::AddUserDialog(QWidget *parent) :
    QDialog(parent),  // 继承 QDialog
    ui(new Ui::AddUserDialog)
{
    ui->setupUi(this);
    setWindowTitle("添加用户");
    // 确认按钮绑定 accept() 槽
      connect(ui->but_ent, &QPushButton::clicked, this, &AddUserDialog::accept);

        // 取消按钮绑定 reject() 槽
      connect(ui->but_cancel, &QPushButton::clicked, this, &AddUserDialog::reject);
}

AddUserDialog::~AddUserDialog()
{
    delete ui;
}

QString AddUserDialog::getId() const
{
    return ui->lineEditId->text();
}

QString AddUserDialog::getName() const
{
    return ui->lineEditName->text();
}

QString AddUserDialog::getClass() const
{
    return ui->lineEditClass->text();
}

QString AddUserDialog::getAge() const
{
    return ui->lineEditAge->text();
}

QString AddUserDialog::getSex() const
{
    return ui->lineEditSex->text();
}

QString AddUserDialog::getPhone() const
{
    return ui->lineEditPhone->text();
}
