#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include "register.hpp"
#include "login.hpp"
#include "client.h"

extern const char* SERVERIP;
extern const int PORT;

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr) : QWidget(parent), loginWindow(new LoginWindow(this)), registerWindow(new RegisterWindow(this)), m_client(new Client(SERVERIP,PORT))  {
        setWindowTitle("Login and Registration System");
        initializeWindows();
        connectSignalsAndSlots();
        showLoginWindow();
    }

private slots:
    void handleLoginRequest(const QString &username, const QString &password) {
        if (username == "admin" && password == "password") {
            QMessageBox::information(this, "Success", "Login successful!");
        } else {
            QMessageBox::warning(this, "Error", "Invalid username or password.");
        }
    }

    void showRegisterWindow() {
        loginWindow->hide();
        registerWindow->show();
    }

    void showLoginWindow() {
        registerWindow->hide();
        loginWindow->show();
    }

private:
    void initializeWindows() {
        // loginWindow->setupUi();
        // loginWindow->connectSignalsAndSlots();
        // registerWindow->setupUi();
        // registerWindow->connectSignalsAndSlots();
    }

    void connectSignalsAndSlots() {
        connect(loginWindow, &LoginWindow::loginRequested, this, &MainWindow::handleLoginRequest);
        connect(loginWindow, &LoginWindow::registerRequested, this, &MainWindow::showRegisterWindow);
        connect(registerWindow, &RegisterWindow::registrationCompleted, this, &MainWindow::showLoginWindow);
        connect(registerWindow, &RegisterWindow::backToLoginRequested, this, &MainWindow::showLoginWindow);
    }

    LoginWindow *loginWindow;
    RegisterWindow *registerWindow;
    Client* m_client;
};