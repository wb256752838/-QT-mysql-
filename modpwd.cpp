#include "modpwd.h"
#include "ui_modpwd.h"

modpwd::modpwd(QSqlDatabase &d) :
    ui(new Ui::modpwd),
    db(d),  // 初始化db引用
    model(new QSqlTableModel(this, db)) //初始化 model
{
    ui->setupUi(this);
    QPalette PAllbackground = this->palette();
    QImage ImgAllbackground(":/1.png");
    QImage fitimgpic=ImgAllbackground.scaled(this->width(),this->height(), Qt::IgnoreAspectRatio);
    PAllbackground.setBrush(QPalette::Window, QBrush(fitimgpic));
    this->setPalette(PAllbackground);
    model->setTable("login");//表名
    if(!model->select())
       {
           qDebug() << model->lastError().text();
           return;
       }
    ui->tableView->resizeColumnsToContents();

    // 设置模型
    ui->tableView->setModel(model);

}
void modpwd::closeEvent(QCloseEvent *event)
{
        qDebug() << "closeEvent";
        //显示登录界面
        emit destroyed();//发送信号
        close();
}


modpwd::~modpwd()
{
    delete ui;
}

//搜索用户名
void modpwd::on_but_search_row_clicked()
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

//添加用户
void modpwd::on_but_add_row_usr_clicked()
{
    QString sno1 = ui->managepage_sno_input->text();  // 获取学生学号
    QString username = ui->managepage_usr_input->text();  // 获取用户名
    QString password = ui->managepage_usr_pwd->text();  // 获取密码
    QString confirmPassword = ui->managepage_usr_pwd_confirm->text();  // 获取确认密码

    if (sno1.isEmpty() || username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请填写完整的用户信息！");
        return;
    }

    // 验证密码和确认密码是否一致
    if (password != confirmPassword) {
        QMessageBox::warning(this, "密码不匹配", "密码和确认密码不一致！");
        return;
    }

    // 验证密码强度（例如，密码长度为至少6个字符）
    if (password.length() < 3) {
        QMessageBox::warning(this, "密码过短", "密码至少需要3个字符！");
        return;
    }

    QSqlQuery query(db);

    // 构建 SQL 语句（插入到 student 表）
    QString sql1 = QString("INSERT INTO student (S_ID, S_NAME) VALUES ('%1', '%2');").arg(sno1).arg(username);
    if (!query.exec(sql1)) {
        qDebug() << "插入学生记录失败:" << query.lastError().text();
        return;
    }

    // 构建 SQL 语句（插入到 login 表）
    QString sql2 = QString("INSERT INTO login (S_ID,S_NAME,S_PASSWD) VALUES ('%1', '%2', '%3');").arg(sno1).arg(username).arg(password);
    if (!query.exec(sql2)) {
        qDebug() << "插入登录记录失败:" << query.lastError().text();
        return;
    }

    // 提示用户添加成功
    QMessageBox::information(this, "添加成功", "用户已成功添加！");
    // 刷新表格数据
       model->select();
       ui->tableView->setModel(model);  // 更新视图
}
//删除

void modpwd::on_but_remove_row_usr_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    int row = index.row();

    // 获取学号或用户名
    QString sno = model->data(model->index(row, 0)).toString();  // 假设学号在第一列

    if (sno.isEmpty()) {
        QMessageBox::warning(this, "删除失败", "未选中有效用户！");
        return;
    }

    // 确认删除
    int response = QMessageBox::warning(this, "删除当前行", "确定删除当前用户吗?", QMessageBox::Yes, QMessageBox::No);
    if (response == QMessageBox::No) {
        return;  // 如果选择了No，取消删除
    }

    QSqlQuery query(db);

    // 删除 student 表中的记录
    query.prepare("DELETE FROM student WHERE S_ID = :id");
    query.bindValue(":id", sno);
    if (!query.exec()) {
        qDebug() << "删除学生记录失败:" << query.lastError().text();
        QMessageBox::warning(this, "删除失败", "删除学生记录失败！");
        return;
    }

    // 删除 login 表中的记录
    query.prepare("DELETE FROM login WHERE s_id = :id");
    query.bindValue(":id", sno);
    if (!query.exec()) {
        qDebug() << "删除登录记录失败:" << query.lastError().text();
        QMessageBox::warning(this, "删除失败", "删除登录记录失败！");
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
    // 刷新表格数据
       model->select();
       ui->tableView->setModel(model);  // 更新视图
}


//提交修改
void modpwd::on_but_ent_mod_clicked()
{
    if (!model->submitAll()) {
        QMessageBox::warning(this, "提交失败", "提交修改失败: " + model->lastError().text());
    } else {
        QMessageBox::information(this, "提交成功", "所有更改已成功提交！");
    }
}
//密码修改
void modpwd::on_but_to_pwd_clicked()
{
    QString sno1 = ui->managepage_sno_input->text();  // 获取学号
    QString newPassword = ui->managepage_usr_pwd->text();  // 获取新密码
    QString confirmPassword = ui->managepage_usr_pwd_confirm->text();  // 获取确认密码

    // 校验新密码和确认密码是否一致
    if (newPassword.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "新密码和确认密码不能为空！");
        return;
    }

    if (newPassword != confirmPassword) {
        QMessageBox::warning(this, "密码不匹配", "新密码和确认密码不一致！");
        return;
    }

    // 密码修改操作
    QSqlQuery query(db);
    query.prepare("UPDATE login SET S_passwd = :newPassword WHERE S_ID = :sno1");
    query.bindValue(":newPassword", newPassword);
    query.bindValue(":sno1", sno1);

    if (!query.exec()) {
        qDebug() << "修改密码失败:" << query.lastError().text();
        return;
    }

    // 提示成功
    QMessageBox::information(this, "成功", "密码修改成功！");

    // 清空输入框
    ui->managepage_usr_pwd->clear();
    ui->managepage_usr_pwd_confirm->clear();
    // 刷新表格数据
       model->select();
       ui->tableView->setModel(model);  // 更新视图
}


