#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setupUi();
        // connectSignalsAndSlots();
    }

signals:
    void loginRequested(const QString &username, const QString &password);
    void registerRequested();

private slots:
    void onLoginClicked() {
        emit loginRequested(usernameEdit->text(), passwordEdit->text());
    }

    void onRegisterClicked() {
        emit registerRequested();
    }

private:
    void setupUi() {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        QLabel *titleLabel = new QLabel("Login", this);
        titleLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(titleLabel);

        QFormLayout *formLayout = new QFormLayout;
        usernameEdit = new QLineEdit(this);
        passwordEdit = new QLineEdit(this);
        passwordEdit->setEchoMode(QLineEdit::Password);

        formLayout->addRow("Username:", usernameEdit);
        formLayout->addRow("Password:", passwordEdit);

        QPushButton *loginButton = new QPushButton("Login", this);
        QPushButton *registerButton = new QPushButton("Register", this);

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(loginButton);
        buttonLayout->addWidget(registerButton);

        mainLayout->addLayout(formLayout);
        mainLayout->addLayout(buttonLayout);

        connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
        connect(registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
    }

    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
};





