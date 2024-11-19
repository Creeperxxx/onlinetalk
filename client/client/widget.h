#pragma once
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


//QT_BEGIN_NAMESPACE
//namespace Ui { class Widget; }
//QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    Client* getclient(){return client;}
    void simple_send_display();
    // void init();

private slots:
    void onSubmitClicked();

private:
//    Ui::Widget *ui;
    QTextEdit *textEdit;
    Client *client;
};
