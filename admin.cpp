#include "admin.h"
#include "ui_admin.h"
#include <QDebug>
#include "bormane.h"

admin::admin(QSqlDatabase &d) :
    ui(new Ui::admin),
    db(d),  // 初始化db引用
    model(new QSqlTableModel(this, db)) //初始化 model
{
    ui->setupUi(this);
    QPalette PAllbackground = this->palette();
    QImage ImgAllbackground(":/1.png");
    QImage fitimgpic=ImgAllbackground.scaled(this->width(),this->height(), Qt::IgnoreAspectRatio);
    PAllbackground.setBrush(QPalette::Window, QBrush(fitimgpic));
    this->setPalette(PAllbackground);


    // 默认显示用户管理页面
      ui->stackedWidget->setCurrentIndex(0);  // 设置为索引 0，即用户管理页面

    model->setTable("student");//表名
    if(!model->select())
       {
           qDebug() << model->lastError().text();
           return;
       }
    // 设置表头
    model->setHeaderData(0, Qt::Horizontal, "学号");
    model->setHeaderData(1, Qt::Horizontal, "姓名");
    model->setHeaderData(2, Qt::Horizontal, "班级");
    model->setHeaderData(3, Qt::Horizontal, "年龄");
    model->setHeaderData(4, Qt::Horizontal, "性别");
    model->setHeaderData(5, Qt::Horizontal, "电话号码");

    // 自适应列宽
    ui->tableView->resizeColumnsToContents();

    // 设置模型
    ui->tableView->setModel(model);

    // 设置编辑策略
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // 禁止编辑
    ui->tableView->setEditTriggers(QTableView::NoEditTriggers);

    // 设置双击可编辑
    ui->tableView->setEditTriggers(QTableView::DoubleClicked);
}

admin::~admin()
{
    delete model;
    delete ui;
}

void admin::closeEvent(QCloseEvent *event)
{
        qDebug() << "closeEvent";
        //显示登录界面
        emit destroyed();//发送信号
        close();
}


void admin::switchPage(){
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if(button==ui->but_usr_manage)
        ui->stackedWidget->setCurrentIndex(0);
    else if(button==ui->but_book_manage)
        ui->stackedWidget->setCurrentIndex(1);


//    else if(button==ui->pushButton_3)
//        ui->stackedWidget->setCurrentIndex(2);

    int i = 0;
    ui->stackedWidget->widget(i);
}
//切换到用户管理 当前界面
void admin::on_but_usr_manage_clicked()
{
 switchPage();
}
// 切换到图书管理界面
void admin::on_but_book_manage_clicked()
{
     switchPage();
     setupBookModelAndView();
//    // 设置当前页面为图书管理页面
//    ui->stackedWidget->setCurrentIndex(1); // 假设图书管理页面是索引为1的页面

//    // 初始化图书管理页面的模型和视图
//    setupBookModelAndView();
}

//密码修改
void admin::on_but_to_pwd_clicked()
{
    modpwd *ped = new modpwd(db);
    ped->show();
    this->hide();
    connect(ped, &modpwd::destroyed, this, &admin::show);
    ped->show();
   // this->close();
}




// 提交修改
void admin::on_but_ent_mod_clicked()
{
    if (!model->submitAll()) {
        QMessageBox::warning(this, "提交失败", "提交修改失败: " + model->lastError().text());
    } else {
        QMessageBox::information(this, "提交成功", "所有更改已成功提交！");
    }
}
//添加用户
void admin::on_but_add_row_usr_clicked()
{
    // 打开自定义对话框
    AddUserDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString id = dialog.getId();
        QString name = dialog.getName();
        QString userClass = dialog.getClass();
        QString age = dialog.getAge();
        QString sex = dialog.getSex();
        QString pno = dialog.getPhone();

        // 校验用户输入
        if (id.isEmpty() || name.isEmpty() || userClass.isEmpty() || age.isEmpty() || sex.isEmpty() || pno.isEmpty()) {
            QMessageBox::warning(this, "输入错误", "所有字段都是必填项，请重新输入！");
            return;
        }

        // 检查用户是否已存在
        QString filter = QString("S_ID = '%1'").arg(id);
        model->setFilter(filter);
        model->select();
        if (model->rowCount() > 0) {
            QMessageBox::warning(this, "添加失败", "用户ID已存在，请检查输入！");
            model->setFilter(""); // 清除筛选
            model->select();
            return;
        }
        model->setFilter(""); // 清除筛选
        model->select();

        // 插入数据
        int row = model->rowCount();
        model->insertRow(row);
        model->setData(model->index(row, 0), id);
        model->setData(model->index(row, 1), name);
        model->setData(model->index(row, 2), userClass);
        model->setData(model->index(row, 3), age);
        model->setData(model->index(row, 4), sex);
        model->setData(model->index(row, 5), pno);

        // 提交更改
        if (!model->submitAll()) {
            QMessageBox::warning(this, "添加失败", "用户添加失败: " + model->lastError().text());
            model->revertAll(); // 回滚未提交的更改
        } else {
            QMessageBox::information(this, "添加成功", "用户已成功添加！");
            model->select(); // 刷新视图
        }
    }
}
// 根据用户名搜索特定行的功能
void admin::on_but_search_row_clicked()
{
    QString usrname = ui->managepage_usr_input->text();
    if (!usrname.isEmpty()) {
        QString filter = QString("S_NAME = '%1'").arg(usrname);
        model->setFilter(filter);
        model->select();
    } else {
        model->setFilter("");
        model->select();
    }
}

// 导入数据
// 导入数据
void admin::on_but_add_data_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择数据文件", "", "CSV Files (*.csv);;All Files (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "导入失败", "无法打开文件: " + file.errorString());
        return;
    }

    QTextStream in(&file);
    bool dataImported = false; // 标记是否有数据被导入
    bool firstLineSkipped = false; // 标记是否跳过了第一行

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        // 跳过第一行（列名）
        if (!firstLineSkipped) {
            firstLineSkipped = true;
            continue;
        }

        if (fields.size() == model->columnCount()) { // 确保列数匹配
            QString studentId = fields[0]; // 假设第一个字段是学号
            bool rowValid = true;          // 标记当前行数据是否有效

            if (studentId.toInt() == 0) { // 检查学号是否为有效整数
                QMessageBox::warning(this, "数据错误", "学号字段不是有效的整数，跳过该行。");
                continue;
            }

            // 检查是否已有匹配学号
            bool studentExists = false;
            for (int row = 0; row < model->rowCount(); ++row) {
                if (model->data(model->index(row, 0)).toString() == studentId) {
                    studentExists = true;

                    // 更新已有学号的数据
                    for (int col = 1; col < fields.size(); ++col) { // 从第2列开始更新
                        model->setData(model->index(row, col), fields[col]);
                    }
                    break;
                }
            }

            // 如果不存在，添加新数据
            if (!studentExists) {
                int newRow = model->rowCount();
                model->insertRow(newRow);
                for (int col = 0; col < fields.size(); ++col) {
                    model->setData(model->index(newRow, col), fields[col]);
                }
            }

            dataImported = true;
        } else {
            QMessageBox::warning(this, "数据错误", "行数据列数不匹配，跳过该行。");
        }
    }

    file.close();

    if (dataImported) {
        QMessageBox::information(this, "导入成功", "数据导入成功，请提交更改以保存！");
    } else {
        QMessageBox::information(this, "导入完成", "没有数据被导入。");
    }
}




// 导出全部数据
void admin::on_but_echo_data_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "导出数据", "", "CSV Files (*.csv);;All Files (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "导出失败", "无法打开文件: " + file.errorString());
        return;
    }

    QTextStream out(&file);

    // 写入表头
    QStringList headers;
    for (int i = 0; i < model->columnCount(); ++i) {
        headers << model->headerData(i, Qt::Horizontal).toString();
    }
    out << headers.join(",") << "\n";

    // 写入表数据
    for (int row = 0; row < model->rowCount(); ++row) {
        QStringList rowData;
        for (int col = 0; col < model->columnCount(); ++col) {
            rowData << model->data(model->index(row, col)).toString();
        }
        out << rowData.join(",") << "\n";
    }

    file.close();
    QMessageBox::information(this, "导出成功", "数据已成功导出到文件: " + fileName);
}


// 删除用户的功能，已存在，但需要优化
void admin::on_but_remove_row_usr_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    int row = index.row();

    // 获取学号（假设学号是第一列）
    int studentId = model->data(model->index(row, 0)).toInt();

    // 确认删除
    int response = QMessageBox::warning(this, "删除当前行", "确定删除当前行?", QMessageBox::Yes, QMessageBox::No);
    if (response == QMessageBox::No) {
        return;  // 如果选择了No，取消删除
    }

    QSqlQuery query(db);

    // 删除 login 表中的相关记录
    query.prepare("DELETE FROM login WHERE s_id = :id");
    query.bindValue(":id", studentId);  // 假设 login 表中使用 s_id 来关联学生
    if (!query.exec()) {
        qDebug() << "删除登录记录失败:" << query.lastError().text();
        QMessageBox::warning(this, "删除失败", "删除登录记录失败！");
        return;
    }

    // 删除 student 表中的记录
    query.prepare("DELETE FROM student WHERE s_id = :id");
    query.bindValue(":id", studentId);
    if (!query.exec()) {
        qDebug() << "删除学生记录失败:" << query.lastError().text();
        QMessageBox::warning(this, "删除失败", "删除学生记录失败！");
        return;
    }

    // 从模型中删除该行
    model->removeRow(row);
    model->submitAll();

    // 更新表格
    model->select();
    ui->tableView->setModel(model);

    // 提示用户删除成功
    QMessageBox::information(this, "删除成功", "用户已成功删除！");
}



// 初始化图书管理页面的模型和视图
void admin::setupBookModelAndView()
{
    // 创建模型
    bookModel = new QSqlTableModel(this, db);
    bookModel->setTable("book");
    bookModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // 设置表头
    bookModel->setHeaderData(0, Qt::Horizontal, "ID");
    bookModel->setHeaderData(1, Qt::Horizontal, "书名");
    bookModel->setHeaderData(2, Qt::Horizontal, "数量");
    bookModel->setHeaderData(3, Qt::Horizontal, "价格");
    bookModel->setHeaderData(4, Qt::Horizontal, "作者");

    // 加载数据
    if(!bookModel->select())
    {
        qDebug() << bookModel->lastError().text();
    }

    // 设置模型到视图
    ui->book_tableView->setModel(bookModel);
    for (int column = 0; column < bookModel->columnCount(); ++column) {
        ui->book_tableView->setColumnWidth(column, 150);
    }
}
//图书搜索
void admin::on_but_bsch_clicked()
{
    // 获取用户输入的搜索关键词
    QString searchQuery = ui->lineEdit_book_search->text().trimmed(); // 去除多余空格

    if (!searchQuery.isEmpty()) {
        // 设置筛选条件，支持部分匹配（模糊查询）
        QString filter = QString("B_NAME LIKE '%%1%'  OR B_ID = '%1'").arg(searchQuery);
        bookModel->setFilter(filter); // 设置筛选条件
        bookModel->select(); // 刷新模型

        // 如果没有搜索结果，提示用户
        if (bookModel->rowCount() == 0) {
            QMessageBox::information(this, "搜索结果", "未找到符合条件的图书！");
        }
    } else {
        // 如果输入为空，显示所有数据
        bookModel->setFilter("");
        bookModel->select();
    }
}



// 图书修改
void admin::on_but_bookcharge_clicked()
{
    // 启用编辑
    ui->book_tableView->setEditTriggers(QTableView::AllEditTriggers);

    // 提交更改
    if(bookModel->submitAll())
    {
        QMessageBox::information(this, "成功", "图书信息更新成功！");
    }
    else
    {
        QMessageBox::critical(this, "错误", "更新失败：" + bookModel->lastError().text());
    }
}

// 图书删除
void admin::on_but_book_delete_clicked()
{
    // 获取选中的行
    int row = ui->book_tableView->currentIndex().row();
    if(row < 0)
    {
        QMessageBox::warning(this, "警告", "请先选择要删除的图书！");
        return;
    }

    // 删除行
    bookModel->removeRow(row);
    if(bookModel->submitAll())
    {
        QMessageBox::information(this, "成功", "图书删除成功！");
    }
    else
    {
        QMessageBox::critical(this, "错误", "删除失败：" + bookModel->lastError().text());
    }
}
//图书增加
void admin::on_but_bookadd_clicked()
{
    AddBookDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString id = dialog.getId();
        QString name = dialog.getName();
        QString count = dialog.getCount();
        QString price = dialog.getPrice();
        QString publisher = dialog.getPublisher();
        QString author = dialog.getAuthor();

        if (id.isEmpty() || name.isEmpty() || count.isEmpty() || price.isEmpty() || publisher.isEmpty() || author.isEmpty()) {
            QMessageBox::warning(this, "输入错误", "所有字段都是必填项，请重新输入！");
            return;
        }

        // 插入数据到模型
        int row = bookModel->rowCount();
        bookModel->insertRow(row);
        bookModel->setData(bookModel->index(row, 0), id);
        bookModel->setData(bookModel->index(row, 1), name);
        bookModel->setData(bookModel->index(row, 2), count);
        bookModel->setData(bookModel->index(row, 3), price);
        bookModel->setData(bookModel->index(row, 4), publisher);
        bookModel->setData(bookModel->index(row, 5), author);

        // 提交更改
        if (!bookModel->submitAll()) {
            QMessageBox::warning(this, "添加失败", "图书添加失败: " + bookModel->lastError().text());
            bookModel->revertAll();
        } else {
            QMessageBox::information(this, "添加成功", "图书已成功添加！");
            bookModel->select(); // 刷新视图
        }
    }
}




void admin::on_but_borrow_manage_clicked()
{
    borMane *bor = new borMane(db);
    bor->show();
    this->hide();
    connect(bor, &admin::destroyed, this, &admin::show);
    bor->show();
    //this->close();
}
//手动刷新
void admin::on_but_reflash_clicked()
{
    if (!model->select()) {
        // 如果select失败，可以在这里处理错误
        QMessageBox::critical(this, "错误", "刷新数据失败: " + model->lastError().text());
        return;
    }
    ui->tableView->setModel(model);

}
