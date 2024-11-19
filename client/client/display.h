#ifndef DISPLAY_H
#define DISPLAY_H

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QApplication>
#include <iostream>
#include <sstream>
#include "client.h"
#include <boost/thread.hpp>

extern const char* SERVERIP;
extern const int PORT;

namespace Ui {
class display;
}

class display : public QWidget
{
    Q_OBJECT

public:
    explicit display(QWidget *parent = nullptr);
    ~display();
    Client* getclient(){return client;}
    void simple_send_display();
private slots:
    void onSubmitClicked();

private:
    Ui::display *ui;

    QTextEdit *textEdit;
    Client *client;
};

#endif // DISPLAY_H
