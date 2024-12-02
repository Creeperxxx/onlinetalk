// #include "../include/display.h"
// #include "../include/client.h"
// #include "../include/mainwindow.hpp"
// #include "../include/thread.hpp"
// #include "../include/login.h"
// #include "widget.h"

#include <QApplication>
#include <QCoreApplication>
#include <QThread>
#include "../include/client.h"
#include "../include/worker.h"

const char* SERVERIP ="127.0.0.1";
const int PORT = 8060;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Worker* worker = new Worker(SERVERIP,PORT);
    QThread* thread = new QThread;
    worker->moveToThread(thread);

    QObject::connect(thread, &QThread::started, worker, &Worker::doWork);

    // 当应用程序即将退出时，确保线程也退出
    QObject::connect(&a, &QCoreApplication::aboutToQuit, [thread, worker](){
        worker->exit();
        thread->quit();  // 请求线程退出
        thread->wait();  // 等待线程完成
        delete worker;   // 删除worker对象
        delete thread;   // 删除线程对象
    });

    thread->start();  // 启动线程
    



    // display window;
    //w.simple_send_display();
    // Widget w;
    // boost::thread widgetThread(&Widget::show, &w);
    //w.getclient() = new Client("127.0.0.1",8060);
    //w.getclient() = new Client(SERVERIP,PORT);
    //w.init();
    //w.getclient()->run();
    //Client client("127.0.0.1", 8060);
    //client.run();
    //boost::thread myThread(&Client::run,w.getclient());
//    window.simple_send_display();
//    window.show();
//    boost::thread ClientThread(&Client::run, window.getclient());
    // login l;
    // l.show();
    // // Widget w;
    // // boost::thread ClientThread(&Client::run, w.getclient());
    // w.simple_send_display();
    // w.show();
    // display window;
    // window.simple_send_display();
    // window.show();
    // display window;
    // boost::thread ClientThread(&Client::run, window.getclient());
    // login l;
    // l.show();
    // MyThread thread;
    // thread.start();
    // MainWindow w;
    // w.show();
    return a.exec();
}
