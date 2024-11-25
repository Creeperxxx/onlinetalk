#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) : QWidget(parent),
                                ui(new Ui::login)
{
    ui->setupUi(this);
    window = new display();
}

login::~login()
{
    delete ui;
}

void login::on_login_button_clicked()
{
    this->loginmessage_send();
}

void login::loginmessage_send()
{
    QString account = ui->accountLineEdit->text();
    std::string account1 = account.toStdString();

    QString password = ui->passwordLineEdit->text();
    std::string password1 = password.toStdString();
    //
    std::string message = R"({
        "type": "login",
        "data": {
                "username": )"+account1+R"(,
                "password": )"+password1+R"(
                }
    })";
    window->getclient()->getsendmsg(message);
    window->getclient()->send();
    
}
