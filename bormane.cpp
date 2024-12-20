#include "bormane.h"
#include "ui_bormane.h"
#include<QDebug>
#include<QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSqlQuery>

borMane::borMane(QSqlDatabase &d) :
//    QWidget(parent),
    ui(new Ui::borMane),
  db(d)
{
    ui->setupUi(this);
    QPalette PAllbackground = this->palette();
    QImage ImgAllbackground(":/1.png");
    QImage fitimgpic=ImgAllbackground.scaled(this->width(),this->height(), Qt::IgnoreAspectRatio);
    PAllbackground.setBrush(QPalette::Window, QBrush(fitimgpic));
    this->setPalette(PAllbackground);
    d_model=new QSqlTableModel(this,db);
    d_model->setTable("borrow");
    d_model->setEditStrategy(QSqlTableModel::OnManualSubmit);//设置手动提交修改
//    ui->tableView->setEditTriggers(QTableView::NoEditTriggers);//设置不能编辑表格
    ui->tableView->setEditTriggers(QTableView::DoubleClicked);//设置双击后可以编辑
    getAll();
}

borMane::~borMane()
{
    delete ui;
}
void borMane::closeEvent(QCloseEvent *event)
{
        qDebug() << "closeEvent";
        //显示登录界面
        emit destroyed();//发送信号
        close();
}


void borMane::on_rueBut_clicked()
{
    d_model->submitAll();
    getAll();
}

void borMane::on_addBut_clicked()
{
    // 创建一个对话框来输入用户信息
    QDialog addUserDialog(this);
    addUserDialog.setWindowTitle(tr("添加用户"));

    // 创建输入字段
    QFormLayout *formLayout = new QFormLayout;
    QLineEdit *usernameEdit = new QLineEdit;
    QLineEdit *passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password); // 密码输入框
    QLineEdit *idEdit = new QLineEdit;

    // 添加输入字段到布局
    formLayout->addRow(tr("用户名:"), usernameEdit);
    formLayout->addRow(tr("账号:"), idEdit);
    formLayout->addRow(tr("密码:"), passwordEdit);

    // 创建按钮和布局
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, &addUserDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &addUserDialog, &QDialog::reject);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);
    addUserDialog.setLayout(layout);

    // 显示对话框并检查结果
    if (addUserDialog.exec() == QDialog::Accepted) {
        QString username = usernameEdit->text();
        QString password = passwordEdit->text();
        QString userid = idEdit->text();

        // 插入新用户到数据库
        QSqlQuery query(db);
        QString sql=QString("INSERT INTO `login` VALUES ('%1', '%2', '%3');").arg(userid).arg(username).arg(password);
        if (query.exec(sql)) {
            QMessageBox::information(this, tr("成功"), tr("用户添加成功。"));
        } else {
            QMessageBox::critical(this, tr("错误"), tr("用户添加失败：") + query.lastError().text());
        }
    }

    getAll(); // 刷新用户列表
    ui->tableView->scrollToBottom();
}

void borMane::on_delBut_clicked()
{
    // 获取选中的行
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, tr("请现在"), tr("请选择要归还的记录。"));
        return;
    }

    // 获取选中行的主键值
    QModelIndex index = selectedIndexes.first();
    int id = d_model->data(d_model->index(index.row(), 0)).toInt();

    // 确认删除
    if (QMessageBox::question(this, tr("确认"), tr("确定归还书籍？"),
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    // 执行操作

    QSqlQuery query(d_model->database());
    qDebug()<<id;
        QString sql = QString("DELETE FROM borrow WHERE J_Id = %1").arg(id);
        if (query.exec(sql)) {
            QMessageBox::information(this, tr("成功"), tr("删除成功。"));
        } else {
            QMessageBox::critical(this, tr("错误"), tr("删除失败：") + query.lastError().text());
        }
    d_model->submitAll();
    getAll();
}

void borMane::on_findBut_clicked()
{
    QString search = ui->searchEdit->text();

    // 根据条件查询用户
    QSqlQueryModel *queryModel = new QSqlQueryModel(this);
    queryModel->setQuery(QString("SELECT * FROM login WHERE S_Name LIKE '%1' OR S_Id LIKE '%1'")
                         .arg(search), db);

    if (queryModel->lastError().type() != QSqlError::NoError) {
        qDebug() << "查询错误:" << queryModel->lastError();
        QMessageBox::critical(this, tr("错误"), tr("查询失败：") + queryModel->lastError().text());
        return;
    }

    // 显示查询结果
    ui->tableView->setModel(queryModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


void borMane::getAll()
{
    if(!d_model->select()){
        qDebug()<<"连接错误";
    }
    ui->tableView->setModel(d_model);
    // 设置中文表头
        QStringList chineseHeaders;
        chineseHeaders << tr("借阅编号") << tr("联系方式") << tr("学号")<< tr("图书编号")<< tr("数量")<< tr("借阅日期")<< tr("归还日期");
        for (int i = 0; i < chineseHeaders.size(); ++i) {
            d_model->setHeaderData(i, Qt::Horizontal, chineseHeaders[i]);
        }

        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
