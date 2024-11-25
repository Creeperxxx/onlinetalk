#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "display.h"

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

    void loginmessage_send();
private slots:
    
    void on_login_button_clicked();


private:
    display *window;
    Ui::login *ui;
};

#endif // LOGIN_H
