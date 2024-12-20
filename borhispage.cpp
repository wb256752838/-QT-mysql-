#include "borhispage.h"
#include "ui_borhispage.h"
#include<QDebug>
#include<QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSqlQuery>
#include<QTableView>

borhispage::borhispage(QSqlDatabase &d,QString IdIn) :
    db(d),
    myid(IdIn),
    ui(new Ui::borhispage)
{
    ui->setupUi(this);
    nowMode=bNameMode;
    d_model=new QSqlTableModel(this,db);
    d_model->setTable("borrow");
    // 设置中文表头
    d_model->setHeaderData(0, Qt::Horizontal, "借阅编号");
    d_model->setHeaderData(1, Qt::Horizontal, "借阅人手机号码");
    d_model->setHeaderData(2, Qt::Horizontal, "借阅人学号");
    d_model->setHeaderData(3, Qt::Horizontal, "借阅数量");
    d_model->setHeaderData(4, Qt::Horizontal, "借阅日期");
    d_model->setHeaderData(5, Qt::Horizontal, "归还日期");

    // 选择模型，以便表头数据生效
    d_model->select();

    // 设置自适应列宽应该在模型设置到视图之后调用
    ui->tableView->setModel(d_model);
    ui->tableView->resizeColumnsToContents();
    d_model->setEditStrategy(QSqlTableModel::OnManualSubmit);//设置手动提交修改
//    ui->tableView->setEditTriggers(QTableView::NoEditTriggers);//设置不能编辑表格
    ui->tableView->setEditTriggers(QTableView::DoubleClicked);//设置双击后可以编辑
    // 设置中文表头


    getAll();

}

borhispage::~borhispage()
{
    delete ui;
}
void borhispage::closeEvent(QCloseEvent *event)
{
        qDebug() << "closeEvent";
        //显示登录界面
        emit destroyed();//发送信号
        close();
}

void borhispage::on_rueBut_clicked()
{
    d_model->submitAll();
    getAll();
}

void borhispage::on_findBut_clicked()
{
    QString search = ui->searchEdit->text();

    // 根据条件查询
    QSqlQueryModel *queryModel = new QSqlQueryModel(this);
    if(nowMode==bNoMode)
        queryModel->setQuery(QString("select bo.* from book b right join borrow bo on b.B_Id = bo.J_Book WHERE B_Id LIKE '%1';")
                         .arg(search), db);
    if(nowMode==bNameMode)
        queryModel->setQuery(QString("select bo.* from book b right join borrow bo on b.B_Id = bo.J_Book WHERE B_Name LIKE '%1';")
                         .arg(search), db);
    if(nowMode==cNameMode)
        queryModel->setQuery(QString("select bo.* from book b right join borrow bo on b.B_Id = bo.J_Book WHERE C_Name LIKE '%1';")
                         .arg(search), db);
    if (queryModel->lastError().type() != QSqlError::NoError) {
        qDebug() << "查询错误:" << queryModel->lastError();
        QMessageBox::critical(this, tr("错误"), tr("查询失败：") + queryModel->lastError().text());
        return;
    }
    queryModel->setHeaderData(0, Qt::Horizontal, "借阅编号");
    queryModel->setHeaderData(1, Qt::Horizontal, "借阅人手机号码");
    queryModel->setHeaderData(2, Qt::Horizontal, "借阅人学号");
    queryModel->setHeaderData(3, Qt::Horizontal, "借阅数量");
    queryModel->setHeaderData(4, Qt::Horizontal, "借阅日期");
    queryModel->setHeaderData(5, Qt::Horizontal, "归还日期");

    // 设置自适应列宽应该在模型设置到视图之后调用
    ui->tableView->setModel(queryModel);
    ui->tableView->resizeColumnsToContents();

    // 显示查询结果
    ui->tableView->setModel(queryModel);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
void borhispage::getAll()
{

    QSqlQueryModel *queryModel = new QSqlQueryModel(this);
        queryModel->setQuery(QString("SELECT * FROM borrow WHERE J_Xh = '%1';")
                         .arg(myid), db);
    if (queryModel->lastError().type() != QSqlError::NoError) {
        qDebug() << "查询错误:" << queryModel->lastError();
        QMessageBox::critical(this, tr("错误"), tr("查询失败：") + queryModel->lastError().text());
        return;
    }

    // 显示查询结果
    ui->tableView->setModel(queryModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
void borhispage::on_borBut_clicked()
{
    // 获取选中的行
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, tr("警告"), tr("请选择要续借的图书。"));
        return;
    }

    // 获取选中行的主键值
    QModelIndex index = selectedIndexes.first();
    int id = d_model->data(d_model->index(index.row(), 0)).toInt();

    // 创建一个对话框来输入用户信息
    QDialog addUserDialog(this);
    addUserDialog.setWindowTitle(tr("续借"));

    // 创建输入字段
    QFormLayout *formLayout = new QFormLayout;
    QLineEdit *NumEdit = new QLineEdit;

    // 添加输入字段到布局
    formLayout->addRow(tr("续借天数:"), NumEdit);

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
        QString BID = QString::number(id);
        QString Num = NumEdit->text();

        //
        QSqlQuery query(db);
        qDebug()<<BID;
        QString sql=QString("UPDATE  borrow SET J_Yh = J_Yh + INTERVAL %1 DAY WHERE J_Id = %2;").arg(Num).arg(id);
        qDebug()<<sql;
        if (query.exec(sql)) {
            QMessageBox::information(this, tr("成功"), tr("借阅成功。"));
        } else {
            QMessageBox::critical(this, tr("错误"), tr("借阅失败：") + query.lastError().text());
        }
    }

    getAll(); // 刷新列表
    ui->tableView->scrollToBottom();
}

void borhispage::on_comboBox_currentTextChanged(const QString &arg1)
{
    if(arg1=="编号"){
        nowMode = bNoMode;
    }
    else if(arg1=="书名"){
        nowMode = bNameMode;
    }
    else if(arg1=="出版社"){
        nowMode = cNameMode;
    }
}
