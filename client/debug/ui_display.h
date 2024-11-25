/********************************************************************************
** Form generated from reading UI file 'display.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISPLAY_H
#define UI_DISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_display
{
public:
    QAction *actiontest;
    QPushButton *pushButton;

    void setupUi(QWidget *display)
    {
        if (display->objectName().isEmpty())
            display->setObjectName(QString::fromUtf8("display"));
        display->resize(640, 480);
        actiontest = new QAction(display);
        actiontest->setObjectName(QString::fromUtf8("actiontest"));
        pushButton = new QPushButton(display);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(130, 220, 80, 24));

        retranslateUi(display);

        QMetaObject::connectSlotsByName(display);
    } // setupUi

    void retranslateUi(QWidget *display)
    {
        display->setWindowTitle(QCoreApplication::translate("display", "Form", nullptr));
        actiontest->setText(QCoreApplication::translate("display", "test", nullptr));
#if QT_CONFIG(shortcut)
        actiontest->setShortcut(QCoreApplication::translate("display", "Ctrl+C", nullptr));
#endif // QT_CONFIG(shortcut)
        pushButton->setText(QCoreApplication::translate("display", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class display: public Ui_display {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAY_H
