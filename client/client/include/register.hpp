
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>

class RegisterWindow : public QWidget {
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setupUi();
        // connectSignalsAndSlots();
    }

signals:
    void registrationCompleted();
    void backToLoginRequested();

private slots:
    void onRegisterClicked() {
        QString username = usernameEdit->text();
        QString email = emailEdit->text();
        QString password = passwordEdit->text();
        QString confirmPassword = confirmPasswordEdit->text();

        if (password != confirmPassword) {
            QMessageBox::warning(this, "Error", "Passwords do not match.");
            return;
        }

        // Here you would typically add code to save the user data to a database.
        QMessageBox::information(this, "Success", "Registration successful!");

        emit registrationCompleted();
    }

    void onBackToLoginClicked() {
        emit backToLoginRequested();
    }

private:
    void setupUi() {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        QLabel *titleLabel = new QLabel("Register", this);
        titleLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(titleLabel);

        QFormLayout *formLayout = new QFormLayout;
        usernameEdit = new QLineEdit(this);
        emailEdit = new QLineEdit(this);
        passwordEdit = new QLineEdit(this);
        confirmPasswordEdit = new QLineEdit(this);
        passwordEdit->setEchoMode(QLineEdit::Password);
        confirmPasswordEdit->setEchoMode(QLineEdit::Password);

        formLayout->addRow("Username:", usernameEdit);
        formLayout->addRow("Email:", emailEdit);
        formLayout->addRow("Password:", passwordEdit);
        formLayout->addRow("Confirm Password:", confirmPasswordEdit);

        QPushButton *registerButton = new QPushButton("Register", this);
        QPushButton *backButton = new QPushButton("Back to Login", this);

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(registerButton);
        buttonLayout->addWidget(backButton);

        mainLayout->addLayout(formLayout);
        mainLayout->addLayout(buttonLayout);

        connect(registerButton, &QPushButton::clicked, this, &RegisterWindow::onRegisterClicked);
        connect(backButton, &QPushButton::clicked, this, &RegisterWindow::onBackToLoginClicked);
    }

    QLineEdit *usernameEdit;
    QLineEdit *emailEdit;
    QLineEdit *passwordEdit;
    QLineEdit *confirmPasswordEdit;
};