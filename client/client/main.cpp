#include "display.h"
#include "client.h"
#include "login.h"
// #include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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
    display window;
    boost::thread ClientThread(&Client::run, window.getclient());
    login l;
    l.show();
    
    return a.exec();
}
