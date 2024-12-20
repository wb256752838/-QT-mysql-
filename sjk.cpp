#include "sjk.h"
#include "ui_sjk.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include "s_book.h"
#include "admin.h"

sjk::sjk(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sjk)
{
    ui->setupUi(this);
    QPalette PAllbackground = this->palette();
    QImage ImgAllbackground(":/1.png");
    QImage fitimgpic=ImgAllbackground.scaled(this->width(),this->height(), Qt::IgnoreAspectRatio);
    PAllbackground.setBrush(QPalette::Window, QBrush(fitimgpic));
    this->setPalette(PAllbackground);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("root");
    db.setDatabaseName("bms");//设置数据库名字，设置好之后，后面写数据表的名字就不再需要加上数据库的名字了
    if(db.open())
    {
        qDebug() << "数据库连接成功";
    }
    else {
        qDebug() << "数据库连接失败：" << db.lastError();
        return; // 如果数据库连接失败，则不继续执行
    }

}

sjk::~sjk()
{
    delete ui;
}

//注册按键
void sjk::on_butregister_clicked()
{

    QSqlQuery query(db);
    // 检查用户输入
    bool register_flag=true;
    QString sno1 = ui->sno->text();
    QString username = ui->username->text();
    QString password = ui->password->text();
    if (sno1.isEmpty() || username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "学号、姓名或密码不能为空！");
        return;
    }
    // 检查学号和姓名是否匹配
    QString checkSql = QString("SELECT * FROM student WHERE S_ID = '%1' AND S_NAME = '%2';").arg(sno1).arg(username);
    if (!query.exec(checkSql) || !query.next()) {
        QMessageBox::information(this, "注册失败", "学号和姓名不匹配！");
        qDebug() << "学号和姓名不匹配：" << query.lastError();
        register_flag=0;
        return;
    }

    // 检查用户是否已存在
    QString checkExistSql = QString("SELECT * FROM logindata WHERE S_ID = '%1' AND S_NAME = '%2';").arg(sno1).arg(username);
    if (query.exec(checkExistSql) && query.next()) {
        QMessageBox::warning(this, "注册失败", "用户已存在");
        qDebug() << "用户已存在";
        register_flag=0;
        return;
    }

    // 插入新用户
    if(register_flag==true)
  {
    QString sql2 = QString("INSERT INTO login (S_ID,S_NAME,S_PASSWD) VALUES ('%1', '%2', '%3');").arg(sno1).arg(username).arg(password);

    if (query.exec(sql2)) {
        QMessageBox::information(this, "注册成功", "欢迎！");
        qDebug() << "登录数据插入成功";
    } else {
        qDebug() << "登录数据插入失败：" << query.lastError();
        QMessageBox::critical(this, "注册失败", "用户已存在!!!。");
    }
  }
}


// 登录按钮点击
void sjk::on_butlogin_clicked()
{

    QSqlQuery query(db);
    QString sql = "SELECT * FROM login;";
    if(query.exec(sql))
    {
        qDebug() << "查询成功";
    }
    else {
        qDebug() << "查询失败：" << query.lastError();
        return; // 如果查询失败，则不继续执行
    }

    QString sno = ui->sno->text();
    QString username = ui->username->text();
    QString password = ui->password->text();

    bool loginSuccess = false;
    bool adminlogin=false;
    if(usr_mode==0){
        if (sno.isEmpty() || username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "登录失败", "学号、姓名或密码不能为空！");
            return;
        }
    while(query.next())
    {
        QString lo_sno = query.value(0).toString();
        QString lo_name = query.value(1).toString();
        QString lo_password = query.value(2).toString();

        if(sno == lo_sno && username == lo_name && password == lo_password)
        {
            loginSuccess = true;

            break;
        }
    }
    if(loginSuccess)
    {
        qDebug() << "登录成功";
        QMessageBox::information(this, "登录成功", "欢迎登录！");
//        this->close();//关
//        sec *pic=new sec();
//        pic->show();
        s_book * b = new s_book(db,sno);
                b->show();//显示新界面
                this->hide();//隐藏当前界面
        connect(b,&s_book::destroyed,this,&sjk::show);
                b->show();
                //this->hide();
                this->close();

    }
    else
    {
        qDebug() << "登录失败，请注册或检查用户名和密码";
        QMessageBox::warning(this, "登录失败", "用户名或密码错误，请重新输入或注册！");
    }
}

    else if (usr_mode == 1) {
    QString sqlAdmin = "SELECT * FROM admin;";
    if (!query.exec(sqlAdmin)) {
        qDebug() << "管理员查询失败：" << query.lastError();
        return;
    }

    while (query.next()) {
        QString A_ID = query.value(0).toString();
        QString A_ZH = query.value(1).toString();
        QString adminName = query.value(2).toString();
        QString adminPwd = query.value(3).toString();
        QString A_PNO = query.value(4).toString();


        if (username == adminName && password == adminPwd) {
            adminlogin = true;
            break;
        }
    }

    if (adminlogin) {
        qDebug() << "管理员登录成功";
        QMessageBox::information(this, "管理员登录成功", "欢迎登录！");
        admin *ad = new admin(db);
        ad->show();
        this->hide();
        connect(ad, &admin::destroyed, this, &sjk::show);
        ad->show();
        this->close();
    } else {
        qDebug() << "管理员登录失败，请检查用户名和密码";
        QMessageBox::warning(this, "登录失败", "用户名或密码错误，请重新输入或注册！");
    }
}
}


void sjk::on_usr_switch_currentTextChanged(const QString &arg1)
{
    if(arg1=="管理员"){
        ui->sno->setReadOnly(true);
        ui->sno->clear();
        ui->butregister->setEnabled(0);
        usr_mode=true;
    }
    if(arg1=="学生"){
        ui->sno->setReadOnly(false);
        ui->butregister->setEnabled(1);
        usr_mode=false;
    }
}
