/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_expression;
    QLabel *label_display;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QGridLayout *gridLayout_10;
    QWidget *widget_2;
    QGridLayout *gridLayout_11;
    QPushButton *btn_7;
    QPushButton *btn_8;
    QPushButton *btn_9;
    QPushButton *btn_lparen;
    QPushButton *btn_4;
    QPushButton *btn_5;
    QPushButton *btn_6;
    QPushButton *btn_rparen;
    QPushButton *btn_1;
    QPushButton *btn_2;
    QPushButton *btn_3;
    QPushButton *btn_add;
    QPushButton *btn_0;
    QPushButton *btn_dot;
    QPushButton *btn_eq;
    QPushButton *btn_sub;
    QPushButton *btn_back;
    QPushButton *btn_clear;
    QPushButton *btn_mul;
    QPushButton *btn_div;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        MainWindow->setStyleSheet(QString::fromUtf8("QLabel#label_expression{\n"
"    background-color:#f8f8f8;\n"
"    border:1px solid #cccccc;\n"
"    border-radius:8px;\n"
"    padding:12px;\n"
"}\n"
"QLabel#label_display{\n"
"    background-color:#f0f0f0;\n"
"    border:1px solid #bbbbbb;\n"
"    border-radius:8px;\n"
"    padding:12px;\n"
"}\n"
"QPushButton[objectName^=\"btn_\"]{\n"
"    min-height:45px;\n"
"    min-width:45px;\n"
"    font-size:16px;\n"
"    border-radius:8px;\n"
"    border:1px solid #dddddd;\n"
"    background-color:#ffffff;\n"
"}\n"
"QPushButton#btn_add,#btn_sub,#btn_mul,#btn_div,#btn_eq{\n"
"    background-color:#ffb86c;\n"
"    color:white;\n"
"}\n"
"QPushButton#btn_clear,#btn_back{\n"
"    background-color:#ff5555;\n"
"    color:white;\n"
"}\n"
"QPushButton[objectName^=\"btn_\"]:hover{\n"
"    background-color:#dddddd;\n"
"}\n"
"QPushButton#btn_add:hover,#btn_sub:hover,#btn_mul:hover,#btn_div:hover,#btn_eq:hover{\n"
"    background-color:#ffa040;\n"
"}\n"
"QPushButton#btn_clear:hover,#btn_back:hover{\n"
"    background-color:#ee3333"
                        ";\n"
"}\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        label_expression = new QLabel(centralwidget);
        label_expression->setObjectName("label_expression");
        label_expression->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        verticalLayout_2->addWidget(label_expression);

        label_display = new QLabel(centralwidget);
        label_display->setObjectName("label_display");
        label_display->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        verticalLayout_2->addWidget(label_display);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        gridLayout_10 = new QGridLayout(widget);
        gridLayout_10->setObjectName("gridLayout_10");
        widget_2 = new QWidget(widget);
        widget_2->setObjectName("widget_2");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy);
        widget_2->setMinimumSize(QSize(80, 40));
        gridLayout_11 = new QGridLayout(widget_2);
        gridLayout_11->setObjectName("gridLayout_11");
        btn_7 = new QPushButton(widget_2);
        btn_7->setObjectName("btn_7");

        gridLayout_11->addWidget(btn_7, 0, 0, 1, 1);

        btn_8 = new QPushButton(widget_2);
        btn_8->setObjectName("btn_8");

        gridLayout_11->addWidget(btn_8, 0, 1, 1, 1);

        btn_9 = new QPushButton(widget_2);
        btn_9->setObjectName("btn_9");

        gridLayout_11->addWidget(btn_9, 0, 2, 1, 1);

        btn_lparen = new QPushButton(widget_2);
        btn_lparen->setObjectName("btn_lparen");

        gridLayout_11->addWidget(btn_lparen, 0, 3, 1, 1);

        btn_4 = new QPushButton(widget_2);
        btn_4->setObjectName("btn_4");

        gridLayout_11->addWidget(btn_4, 1, 0, 1, 1);

        btn_5 = new QPushButton(widget_2);
        btn_5->setObjectName("btn_5");

        gridLayout_11->addWidget(btn_5, 1, 1, 1, 1);

        btn_6 = new QPushButton(widget_2);
        btn_6->setObjectName("btn_6");

        gridLayout_11->addWidget(btn_6, 1, 2, 1, 1);

        btn_rparen = new QPushButton(widget_2);
        btn_rparen->setObjectName("btn_rparen");

        gridLayout_11->addWidget(btn_rparen, 1, 3, 1, 1);

        btn_1 = new QPushButton(widget_2);
        btn_1->setObjectName("btn_1");

        gridLayout_11->addWidget(btn_1, 2, 0, 1, 1);

        btn_2 = new QPushButton(widget_2);
        btn_2->setObjectName("btn_2");

        gridLayout_11->addWidget(btn_2, 2, 1, 1, 1);

        btn_3 = new QPushButton(widget_2);
        btn_3->setObjectName("btn_3");

        gridLayout_11->addWidget(btn_3, 2, 2, 1, 1);

        btn_add = new QPushButton(widget_2);
        btn_add->setObjectName("btn_add");

        gridLayout_11->addWidget(btn_add, 2, 3, 1, 1);

        btn_0 = new QPushButton(widget_2);
        btn_0->setObjectName("btn_0");

        gridLayout_11->addWidget(btn_0, 3, 0, 1, 1);

        btn_dot = new QPushButton(widget_2);
        btn_dot->setObjectName("btn_dot");

        gridLayout_11->addWidget(btn_dot, 3, 1, 1, 1);

        btn_eq = new QPushButton(widget_2);
        btn_eq->setObjectName("btn_eq");

        gridLayout_11->addWidget(btn_eq, 3, 2, 1, 1);

        btn_sub = new QPushButton(widget_2);
        btn_sub->setObjectName("btn_sub");

        gridLayout_11->addWidget(btn_sub, 3, 3, 1, 1);

        btn_back = new QPushButton(widget_2);
        btn_back->setObjectName("btn_back");

        gridLayout_11->addWidget(btn_back, 4, 0, 1, 1);

        btn_clear = new QPushButton(widget_2);
        btn_clear->setObjectName("btn_clear");

        gridLayout_11->addWidget(btn_clear, 4, 1, 1, 1);

        btn_mul = new QPushButton(widget_2);
        btn_mul->setObjectName("btn_mul");

        gridLayout_11->addWidget(btn_mul, 4, 2, 1, 1);

        btn_div = new QPushButton(widget_2);
        btn_div->setObjectName("btn_div");

        gridLayout_11->addWidget(btn_div, 4, 3, 1, 1);


        gridLayout_10->addWidget(widget_2, 0, 0, 1, 1);


        verticalLayout->addWidget(widget);


        verticalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 18));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_expression->setText(QString());
        label_display->setText(QString());
        btn_7->setText(QCoreApplication::translate("MainWindow", "7", nullptr));
        btn_8->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        btn_9->setText(QCoreApplication::translate("MainWindow", "9", nullptr));
        btn_lparen->setText(QCoreApplication::translate("MainWindow", "(", nullptr));
        btn_4->setText(QCoreApplication::translate("MainWindow", "4", nullptr));
        btn_5->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        btn_6->setText(QCoreApplication::translate("MainWindow", "6", nullptr));
        btn_rparen->setText(QCoreApplication::translate("MainWindow", ")", nullptr));
        btn_1->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        btn_2->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        btn_3->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        btn_add->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        btn_0->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        btn_dot->setText(QCoreApplication::translate("MainWindow", ".", nullptr));
        btn_eq->setText(QCoreApplication::translate("MainWindow", "=", nullptr));
        btn_sub->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        btn_back->setText(QCoreApplication::translate("MainWindow", "\342\206\220", nullptr));
        btn_clear->setText(QCoreApplication::translate("MainWindow", "C", nullptr));
        btn_mul->setText(QCoreApplication::translate("MainWindow", "*", nullptr));
        btn_div->setText(QCoreApplication::translate("MainWindow", "/", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
