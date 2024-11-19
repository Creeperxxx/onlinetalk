#include "widget.h"
//#include "ui_widget.h"

const char* SERVERIP = "127.0.0.1";
const int PORT = 8060;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
//    ui = new Ui::Widget;
//     ui->setupUi(this);*/

//    setWindowTitle("Qt 窗口示例");
//            resize(400, 300);

//            // 创建布局
//            QVBoxLayout *layout = new QVBoxLayout(this);

//            // 创建按钮
//            QPushButton *button = new QPushButton("点击我", this);
//            layout->addWidget(button);

//            // 设置主窗口的布局
//            setLayout(layout);
    client = new Client(SERVERIP,PORT);
    setWindowTitle("main window");
    resize(400, 300);
//    client = new Client(SERVERIP,PORT);
}

Widget::~Widget()
{
//    delete ui;
    delete client;
}

void Widget::onSubmitClicked()
{
    QString inputText = textEdit->toPlainText();
    std::cout << "输入的内容: " << inputText.toStdString() << std::endl;

    // 发送内容给客户端
    client->getsendmsg(inputText.toStdString());
    client->send();
    // // 将内容写入标准输入缓冲区（这里使用 cout 模拟）
    // std::istringstream iss(inputText.toStdString());
    // std::cin.rdbuf(iss.rdbuf());

    // 清空输入框
    textEdit->clear();
}

// void Widget::init()
// {
//     client = new Client(SERVERIP,PORT);
// }

void Widget::simple_send_display()
{
        // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 创建标签
    QLabel *label = new QLabel("请输入内容:", this);
    layout->addWidget(label);

    // 创建 QTextEdit 控件
    textEdit = new QTextEdit(this);
    layout->addWidget(textEdit);

    // 创建按钮
    QPushButton *submitButton = new QPushButton("提交", this);
    layout->addWidget(submitButton);

    // 连接按钮点击信号到槽函数
    connect(submitButton, &QPushButton::clicked, this, &Widget::onSubmitClicked);

    // 设置布局居中对齐
    layout->setAlignment(Qt::AlignCenter);
    // this->show();
}

