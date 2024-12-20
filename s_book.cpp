#include "s_book.h"
#include "ui_s_book.h"
#include "s_book.h"
#include "ui_s_book.h"
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
#include <QFile>
#include <QFileDialog>
#include "borhispage.h"
s_book::s_book(QSqlDatabase &d,QString IdIn) :
    ui(new Ui::s_book),
    myid(IdIn),
   db(d)
{
    ui->setupUi(this);
    //数据库
    QPalette PAllbackground = this->palette();
    QImage ImgAllbackground(":/1.png");
    QImage fitimgpic=ImgAllbackground.scaled(this->width(),this->height(), Qt::IgnoreAspectRatio);
    PAllbackground.setBrush(QPalette::Window, QBrush(fitimgpic));
    this->setPalette(PAllbackground);
    nowMode=bNameMode;
    d_model=new QSqlTableModel(this,db);
    d_model->setTable("book");
    d_model->setEditStrategy(QSqlTableModel::OnManualSubmit);//设置手动提交修改
//    ui->tableView->setEditTriggers(QTableView::NoEditTriggers);//设置不能编辑表格
    ui->tableView->setEditTriggers(QTableView::DoubleClicked);//设置双击后可以编辑
    getAll();
}

s_book::~s_book()
{
    delete ui;
}


void s_book::closeEvent(QCloseEvent *event)
{
        qDebug() << "closeEvent";
        //显示登录界面
        emit destroyed();//发送信号
        close();
}

void s_book::on_rueBut_clicked()
{
    d_model->submitAll();
    getAll();
}

void s_book::on_addBut_clicked()
{
    // 创建一个对话框来输入用户信息
    QDialog addUserDialog(this);
    addUserDialog.setWindowTitle(tr("图书入库"));

    // 创建输入字段
    QFormLayout *formLayout = new QFormLayout;
    QLineEdit *IDEdit = new QLineEdit;
    QLineEdit *NameEdit = new QLineEdit;
    QLineEdit *CouEdit = new QLineEdit;
    QLineEdit *JgEdit = new QLineEdit;
    QLineEdit *cNameEdit = new QLineEdit;

    // 添加输入字段到布局
    formLayout->addRow(tr("编号:"), IDEdit);
    formLayout->addRow(tr("书名:"), NameEdit);
    formLayout->addRow(tr("库存:"), CouEdit);
    formLayout->addRow(tr("价格:"), JgEdit);
    formLayout->addRow(tr("出版社:"), cNameEdit);

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
        QString ID = IDEdit->text();
        QString Bname = NameEdit->text();
        QString Cou = CouEdit->text();
        QString Jg = JgEdit->text();
        QString Cn = cNameEdit->text();

        // 插入到数据库
        QSqlQuery query(db);
        QString sql=QString("INSERT INTO `book` VALUES ('%1', '%2', '%3','%4','%5');").arg(ID).arg(Bname).arg(Cou).arg(Jg).arg(Cn);
        if (query.exec(sql)) {
            QMessageBox::information(this, tr("成功"), tr("入库成功。"));
        } else {
            QMessageBox::critical(this, tr("错误"), tr("添加失败：") + query.lastError().text());
        }
    }

    getAll(); // 刷新列表
    ui->tableView->scrollToBottom();
}

void s_book::on_delBut_clicked()
{
    // 获取选中的行
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, tr("警告"), tr("请选择要删除的行。"));
        return;
    }

    // 获取选中行的主键值
    QModelIndex index = selectedIndexes.first();
     id = d_model->data(d_model->index(index.row(), 0)).toInt();

    // 确认删除
    if (QMessageBox::question(this, tr("确认"), tr("确定要删除选中的图书吗？"),
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    // 执行删除操作
    if (d_model->removeRow(index.row())) {
        QMessageBox::information(this, tr("成功"), tr("删除成功。"));
    } else {
        QMessageBox::critical(this, tr("错误"), tr("删除失败。"));
    }
    d_model->submitAll();
    getAll();
}

void s_book::on_findBut_clicked()
{
    QString search = ui->searchEdit->text();

    // 根据条件查询
    QSqlQueryModel *queryModel = new QSqlQueryModel(this);
    if(nowMode==bNoMode)
        queryModel->setQuery(QString("SELECT * FROM book WHERE B_Id LIKE '%1';")
                         .arg(search), db);
    if(nowMode==bNameMode)
        queryModel->setQuery(QString("SELECT * FROM book WHERE B_Name LIKE '%1';")
                         .arg(search), db);
    if(nowMode==cNameMode)
        queryModel->setQuery(QString("SELECT * FROM book WHERE C_Name LIKE '%1';")
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
void s_book::getAll()
{
    if(!d_model->select()){
        qDebug()<<"连接错误";
    }
    ui->tableView->setModel(d_model);
    // 设置中文表头
        QStringList chineseHeaders;
        chineseHeaders << tr("图书编号") << tr("书名") << tr("库存")<< tr("价格")<< tr("出版社"); // 根据你的列数和内容添加
        for (int i = 0; i < chineseHeaders.size(); ++i) {
            d_model->setHeaderData(i, Qt::Horizontal, chineseHeaders[i]);
        }

        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}
void s_book::on_borBut_clicked()
{
    // 获取选中的行
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, tr("警告"), tr("请选择要删除的行。"));
        return;
    }

    // 获取选中行的主键值
    QModelIndex index = selectedIndexes.first();
    int id = d_model->data(d_model->index(index.row(), 0)).toInt();

    // 创建一个对话框来输入用户信息
    QDialog addUserDialog(this);
    addUserDialog.setWindowTitle(tr("图书入库"));

    // 创建输入字段
    QFormLayout *formLayout = new QFormLayout;
    QLineEdit *SIdEdit = new QLineEdit;
    QLineEdit *NumEdit = new QLineEdit;
    QLineEdit *emailEdit = new QLineEdit;

    // 添加输入字段到布局
    formLayout->addRow(tr("借阅人:"), SIdEdit);
    formLayout->addRow(tr("数量:"), NumEdit);
    formLayout->addRow(tr("联系方式:"), emailEdit);

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
        QString SID = SIdEdit->text();
        QString Num = NumEdit->text();
        QString email = emailEdit->text();

        // 插入到数据库
        QSqlQuery query(db);
        qDebug()<<BID;
        QString sql=QString("call `bor_book`('%1','%2',%3,'%4');").arg(SID).arg(BID).arg(Num).arg(email);
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

void s_book::on_comboBox_currentTextChanged(const QString &arg1)
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

void s_book::on_imBut_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("导入书籍"), "", tr("CSV Files (*.csv)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, tr("错误"), tr("无法打开文件：") + file.errorString());
            return;
        }

        QTextStream in(&file);
        QSqlQuery query(db);
        query.prepare("INSERT INTO `book` (B_Id, B_Name, B_Cou, B_Jg, C_Name) VALUES (:id, :name, :cou, :jg, :CName)");

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(',');
            if (fields.size() < 5) continue; // 确保每行有足够的字段

            // 检查主键是否已存在
            QSqlQuery checkQuery(db);
            checkQuery.prepare("SELECT COUNT(*) FROM `book` WHERE B_Id = :id");
            checkQuery.bindValue(":id", fields[0]);
            if (!checkQuery.exec() || checkQuery.next() && checkQuery.value(0).toInt() > 0) {
                // 主键已存在，跳过此记录
                continue;
            }

            query.bindValue(":id", fields[0]);
            query.bindValue(":name", fields[1]);
            query.bindValue(":cou", fields[2]);
            query.bindValue(":jg", fields[3]);
            query.bindValue(":CName", fields[4]);
            if (!query.exec()) {
                QMessageBox::critical(this, tr("错误"), tr("导入失败：") + query.lastError().text());
                break;
            }
        }

        file.close();
        QMessageBox::information(this, tr("成功"), tr("图书数据导入成功。"));
        getAll(); // 刷新列表
    }
}

void s_book::on_outBut_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("导出书库"), "", tr("CSV Files (*.csv)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(this, tr("错误"), tr("无法打开文件：") + file.errorString());
            return;
        }

        QTextStream out(&file);
        QSqlQueryModel model;
        model.setQuery("SELECT * FROM book", db);

        for (int row = 0; row < model.rowCount(); ++row) {
            for (int col = 0; col < model.columnCount(); ++col) {
                out << model.data(model.index(row, col)).toString();
                if (col < model.columnCount() - 1)
                    out << ",";
            }
            out << "\n";
        }

        file.close();
        QMessageBox::information(this, tr("成功"), tr("图书数据导出成功。"));
    }
}
void s_book::on_pushButton_clicked()
{

    borhispage *borh = new borhispage(db,myid);
    borh->show();
    this->hide();
    connect(borh, &s_book::destroyed, this, &s_book::show);
    borh->show();
}
