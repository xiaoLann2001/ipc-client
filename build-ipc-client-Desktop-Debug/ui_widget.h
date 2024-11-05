/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_VideoPlayer;
    QPushButton *pushButton_videoReply;
    QPushButton *pushButton_event;
    QPushButton *pushButton_setting;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QWidget *widget;
    QLCDNumber *lcdNumber;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_logo;
    QLabel *label_appName;
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_4;
    QRadioButton *radioButton_3;
    QRadioButton *radioButton;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_4;
    QWidget *horizontalLayoutWidget_5;
    QHBoxLayout *horizontalLayout_5;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *pushButton_12;
    QPushButton *pushButton_11;
    QPushButton *pushButton_8;
    QPushButton *pushButton_7;
    QPushButton *pushButton_10;
    QComboBox *comboBox;
    QLabel *label_2;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1024, 600);
        Widget->setMinimumSize(QSize(1024, 600));
        Widget->setMaximumSize(QSize(1024, 600));
        scrollArea = new QScrollArea(Widget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(0, 60, 171, 541));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 169, 539));
        scrollArea->setWidget(scrollAreaWidgetContents);
        horizontalLayoutWidget = new QWidget(Widget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(499, 0, 352, 61));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_VideoPlayer = new QPushButton(horizontalLayoutWidget);
        pushButton_VideoPlayer->setObjectName(QString::fromUtf8("pushButton_VideoPlayer"));

        horizontalLayout->addWidget(pushButton_VideoPlayer);

        pushButton_videoReply = new QPushButton(horizontalLayoutWidget);
        pushButton_videoReply->setObjectName(QString::fromUtf8("pushButton_videoReply"));

        horizontalLayout->addWidget(pushButton_videoReply);

        pushButton_event = new QPushButton(horizontalLayoutWidget);
        pushButton_event->setObjectName(QString::fromUtf8("pushButton_event"));

        horizontalLayout->addWidget(pushButton_event);

        pushButton_setting = new QPushButton(horizontalLayoutWidget);
        pushButton_setting->setObjectName(QString::fromUtf8("pushButton_setting"));

        horizontalLayout->addWidget(pushButton_setting);

        horizontalLayoutWidget_2 = new QWidget(Widget);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(860, 0, 161, 31));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButton_3 = new QPushButton(horizontalLayoutWidget_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        horizontalLayout_2->addWidget(pushButton_3);

        pushButton_2 = new QPushButton(horizontalLayoutWidget_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout_2->addWidget(pushButton_2);

        pushButton = new QPushButton(horizontalLayoutWidget_2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        widget = new QWidget(Widget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(179, 59, 671, 491));
        lcdNumber = new QLCDNumber(Widget);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setGeometry(QRect(860, 30, 161, 31));
        horizontalLayoutWidget_3 = new QWidget(Widget);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(0, 0, 361, 61));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_logo = new QLabel(horizontalLayoutWidget_3);
        label_logo->setObjectName(QString::fromUtf8("label_logo"));

        horizontalLayout_3->addWidget(label_logo);

        label_appName = new QLabel(horizontalLayoutWidget_3);
        label_appName->setObjectName(QString::fromUtf8("label_appName"));

        horizontalLayout_3->addWidget(label_appName);

        horizontalLayoutWidget_4 = new QWidget(Widget);
        horizontalLayoutWidget_4->setObjectName(QString::fromUtf8("horizontalLayoutWidget_4"));
        horizontalLayoutWidget_4->setGeometry(QRect(180, 550, 444, 51));
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget_4);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget_4);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_4->addWidget(label);

        radioButton_2 = new QRadioButton(horizontalLayoutWidget_4);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        horizontalLayout_4->addWidget(radioButton_2);

        radioButton_4 = new QRadioButton(horizontalLayoutWidget_4);
        radioButton_4->setObjectName(QString::fromUtf8("radioButton_4"));

        horizontalLayout_4->addWidget(radioButton_4);

        radioButton_3 = new QRadioButton(horizontalLayoutWidget_4);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));

        horizontalLayout_4->addWidget(radioButton_3);

        radioButton = new QRadioButton(horizontalLayoutWidget_4);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        horizontalLayout_4->addWidget(radioButton);

        verticalLayoutWidget = new QWidget(Widget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(860, 60, 161, 271));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_5 = new QPushButton(verticalLayoutWidget);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));

        verticalLayout->addWidget(pushButton_5);

        pushButton_6 = new QPushButton(verticalLayoutWidget);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));

        verticalLayout->addWidget(pushButton_6);

        pushButton_4 = new QPushButton(verticalLayoutWidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        verticalLayout->addWidget(pushButton_4);

        horizontalLayoutWidget_5 = new QWidget(Widget);
        horizontalLayoutWidget_5->setObjectName(QString::fromUtf8("horizontalLayoutWidget_5"));
        horizontalLayoutWidget_5->setGeometry(QRect(680, 550, 341, 51));
        horizontalLayout_5 = new QHBoxLayout(horizontalLayoutWidget_5);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        gridLayoutWidget = new QWidget(Widget);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(860, 380, 161, 161));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_12 = new QPushButton(gridLayoutWidget);
        pushButton_12->setObjectName(QString::fromUtf8("pushButton_12"));
        pushButton_12->setMaximumSize(QSize(20, 20));

        gridLayout->addWidget(pushButton_12, 2, 1, 1, 1);

        pushButton_11 = new QPushButton(gridLayoutWidget);
        pushButton_11->setObjectName(QString::fromUtf8("pushButton_11"));
        pushButton_11->setMaximumSize(QSize(20, 20));

        gridLayout->addWidget(pushButton_11, 0, 1, 1, 1);

        pushButton_8 = new QPushButton(gridLayoutWidget);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setMaximumSize(QSize(20, 20));

        gridLayout->addWidget(pushButton_8, 1, 1, 1, 1);

        pushButton_7 = new QPushButton(gridLayoutWidget);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setMaximumSize(QSize(20, 20));

        gridLayout->addWidget(pushButton_7, 1, 2, 1, 1);

        pushButton_10 = new QPushButton(gridLayoutWidget);
        pushButton_10->setObjectName(QString::fromUtf8("pushButton_10"));
        pushButton_10->setMaximumSize(QSize(20, 20));

        gridLayout->addWidget(pushButton_10, 1, 0, 1, 1);

        comboBox = new QComboBox(Widget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(910, 340, 111, 31));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(860, 340, 51, 31));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        pushButton_VideoPlayer->setText(QApplication::translate("Widget", "video", nullptr));
        pushButton_videoReply->setText(QApplication::translate("Widget", "reply", nullptr));
        pushButton_event->setText(QApplication::translate("Widget", "event", nullptr));
        pushButton_setting->setText(QApplication::translate("Widget", "setting", nullptr));
        pushButton_3->setText(QApplication::translate("Widget", "-", nullptr));
        pushButton_2->setText(QApplication::translate("Widget", "0", nullptr));
        pushButton->setText(QApplication::translate("Widget", "x", nullptr));
        label_logo->setText(QApplication::translate("Widget", "logo", nullptr));
        label_appName->setText(QApplication::translate("Widget", "app name", nullptr));
        label->setText(QApplication::translate("Widget", "view grid", nullptr));
        radioButton_2->setText(QApplication::translate("Widget", "1", nullptr));
        radioButton_4->setText(QApplication::translate("Widget", "4", nullptr));
        radioButton_3->setText(QApplication::translate("Widget", "9", nullptr));
        radioButton->setText(QApplication::translate("Widget", "16", nullptr));
        pushButton_5->setText(QApplication::translate("Widget", "PushButton", nullptr));
        pushButton_6->setText(QApplication::translate("Widget", "PushButton", nullptr));
        pushButton_4->setText(QApplication::translate("Widget", "PushButton", nullptr));
        pushButton_12->setText(QApplication::translate("Widget", "d", nullptr));
        pushButton_11->setText(QApplication::translate("Widget", "u", nullptr));
        pushButton_8->setText(QString());
        pushButton_7->setText(QApplication::translate("Widget", "r", nullptr));
        pushButton_10->setText(QApplication::translate("Widget", "l", nullptr));
        label_2->setText(QApplication::translate("Widget", "preset", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
