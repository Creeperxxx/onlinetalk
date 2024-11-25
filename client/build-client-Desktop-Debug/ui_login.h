/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    QFormLayout *formLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QFormLayout *formLayout;
    QLabel *errorLabel;
    QLineEdit *accountLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QPushButton *login_button;
    QPushButton *register_button;

    void setupUi(QWidget *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName(QString::fromUtf8("login"));
        login->resize(205, 128);
        formLayout_2 = new QFormLayout(login);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(login);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        errorLabel = new QLabel(login);
        errorLabel->setObjectName(QString::fromUtf8("errorLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, errorLabel);

        accountLineEdit = new QLineEdit(login);
        accountLineEdit->setObjectName(QString::fromUtf8("accountLineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, accountLineEdit);

        passwordLabel = new QLabel(login);
        passwordLabel->setObjectName(QString::fromUtf8("passwordLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, passwordLabel);

        passwordLineEdit = new QLineEdit(login);
        passwordLineEdit->setObjectName(QString::fromUtf8("passwordLineEdit"));
        passwordLineEdit->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(1, QFormLayout::FieldRole, passwordLineEdit);

        login_button = new QPushButton(login);
        login_button->setObjectName(QString::fromUtf8("login_button"));

        formLayout->setWidget(2, QFormLayout::LabelRole, login_button);

        register_button = new QPushButton(login);
        register_button->setObjectName(QString::fromUtf8("register_button"));

        formLayout->setWidget(2, QFormLayout::FieldRole, register_button);


        verticalLayout->addLayout(formLayout);


        formLayout_2->setLayout(0, QFormLayout::LabelRole, verticalLayout);


        retranslateUi(login);

        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QWidget *login)
    {
        login->setWindowTitle(QCoreApplication::translate("login", "Form", nullptr));
        label->setText(QCoreApplication::translate("login", "error:", nullptr));
        errorLabel->setText(QCoreApplication::translate("login", "account:", nullptr));
        accountLineEdit->setText(QString());
        passwordLabel->setText(QCoreApplication::translate("login", "password:", nullptr));
        passwordLineEdit->setText(QString());
        login_button->setText(QCoreApplication::translate("login", "Login", nullptr));
        register_button->setText(QCoreApplication::translate("login", "Register", nullptr));
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
