/********************************************************************************
** Form generated from reading UI file 'search.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCH_H
#define UI_SEARCH_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogSearch
{
public:
    QLineEdit *lineEdit_text;
    QGroupBox *groupBox_3;
    QRadioButton *radioButton_fullSearch;
    QRadioButton *radioButton_partSearch;
    QRadioButton *radioButton_wildcard;
    QGroupBox *groupBox_5;
    QRadioButton *radioButton_name;
    QRadioButton *radioButton_code;
    QGroupBox *groupBox_4;
    QWidget *widget_class2;
    QLabel *label_4;
    QListWidget *listWidget_class;
    QWidget *widget;
    QRadioButton *radioButton_class1;
    QRadioButton *radioButton_class2;
    QWidget *widget_class1;
    QComboBox *comboBox_codeInGroup;
    QWidget *widgetGroup;
    QLabel *label_3;
    QComboBox *comboBox_group;
    QLabel *label_11;
    QWidget *widgetClass;
    QLabel *label_5;
    QComboBox *comboBox_class;
    QWidget *widget_type;
    QLabel *label_6;
    QListWidget *listWidget_type;
    QLabel *label_7;
    QLineEdit *lineEdit_level;
    QLabel *label_2;
    QPushButton *pButton_find;
    QPushButton *pButton_close;

    void setupUi(QDialog *DialogSearch)
    {
        if (DialogSearch->objectName().isEmpty())
            DialogSearch->setObjectName(QStringLiteral("DialogSearch"));
        DialogSearch->resize(604, 593);
        lineEdit_text = new QLineEdit(DialogSearch);
        lineEdit_text->setObjectName(QStringLiteral("lineEdit_text"));
        lineEdit_text->setGeometry(QRect(70, 20, 411, 20));
        QFont font;
        font.setFamily(QStringLiteral("Segoe UI"));
        lineEdit_text->setFont(font);
        groupBox_3 = new QGroupBox(DialogSearch);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(20, 120, 461, 51));
        groupBox_3->setFont(font);
        radioButton_fullSearch = new QRadioButton(groupBox_3);
        radioButton_fullSearch->setObjectName(QStringLiteral("radioButton_fullSearch"));
        radioButton_fullSearch->setGeometry(QRect(20, 20, 151, 17));
        radioButton_fullSearch->setFont(font);
        radioButton_fullSearch->setCheckable(true);
        radioButton_fullSearch->setChecked(true);
        radioButton_partSearch = new QRadioButton(groupBox_3);
        radioButton_partSearch->setObjectName(QStringLiteral("radioButton_partSearch"));
        radioButton_partSearch->setGeometry(QRect(160, 20, 151, 17));
        radioButton_partSearch->setFont(font);
        radioButton_wildcard = new QRadioButton(groupBox_3);
        radioButton_wildcard->setObjectName(QStringLiteral("radioButton_wildcard"));
        radioButton_wildcard->setGeometry(QRect(320, 20, 101, 17));
        radioButton_wildcard->setFont(font);
        groupBox_5 = new QGroupBox(DialogSearch);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(20, 60, 461, 51));
        groupBox_5->setFont(font);
        radioButton_name = new QRadioButton(groupBox_5);
        radioButton_name->setObjectName(QStringLiteral("radioButton_name"));
        radioButton_name->setGeometry(QRect(20, 20, 141, 17));
        radioButton_name->setFont(font);
        radioButton_name->setChecked(true);
        radioButton_code = new QRadioButton(groupBox_5);
        radioButton_code->setObjectName(QStringLiteral("radioButton_code"));
        radioButton_code->setGeometry(QRect(190, 20, 141, 17));
        radioButton_code->setFont(font);
        groupBox_4 = new QGroupBox(DialogSearch);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setEnabled(true);
        groupBox_4->setGeometry(QRect(20, 180, 461, 401));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_4->sizePolicy().hasHeightForWidth());
        groupBox_4->setSizePolicy(sizePolicy);
        groupBox_4->setFont(font);
        groupBox_4->setFlat(false);
        groupBox_4->setCheckable(false);
        widget_class2 = new QWidget(groupBox_4);
        widget_class2->setObjectName(QStringLiteral("widget_class2"));
        widget_class2->setEnabled(false);
        widget_class2->setGeometry(QRect(30, 140, 431, 141));
        label_4 = new QLabel(widget_class2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 2, 41, 15));
        QFont font1;
        font1.setFamily(QStringLiteral("Segoe UI"));
        font1.setPointSize(9);
        label_4->setFont(font1);
        listWidget_class = new QListWidget(widget_class2);
        listWidget_class->setObjectName(QStringLiteral("listWidget_class"));
        listWidget_class->setGeometry(QRect(90, 2, 256, 131));
        listWidget_class->setAlternatingRowColors(true);
        listWidget_class->setSelectionMode(QAbstractItemView::MultiSelection);
        widget = new QWidget(groupBox_4);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(0, 20, 31, 141));
        radioButton_class1 = new QRadioButton(widget);
        radioButton_class1->setObjectName(QStringLiteral("radioButton_class1"));
        radioButton_class1->setGeometry(QRect(10, 0, 31, 17));
        radioButton_class1->setChecked(true);
        radioButton_class2 = new QRadioButton(widget);
        radioButton_class2->setObjectName(QStringLiteral("radioButton_class2"));
        radioButton_class2->setGeometry(QRect(10, 120, 31, 20));
        widget_class1 = new QWidget(groupBox_4);
        widget_class1->setObjectName(QStringLiteral("widget_class1"));
        widget_class1->setEnabled(true);
        widget_class1->setGeometry(QRect(30, 10, 431, 131));
        comboBox_codeInGroup = new QComboBox(widget_class1);
        comboBox_codeInGroup->setObjectName(QStringLiteral("comboBox_codeInGroup"));
        comboBox_codeInGroup->setGeometry(QRect(90, 90, 331, 21));
        comboBox_codeInGroup->setEditable(true);
        widgetGroup = new QWidget(widget_class1);
        widgetGroup->setObjectName(QStringLiteral("widgetGroup"));
        widgetGroup->setGeometry(QRect(0, 40, 431, 41));
        label_3 = new QLabel(widgetGroup);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 12, 41, 15));
        label_3->setFont(font1);
        comboBox_group = new QComboBox(widgetGroup);
        comboBox_group->setObjectName(QStringLiteral("comboBox_group"));
        comboBox_group->setGeometry(QRect(90, 10, 331, 20));
        comboBox_group->setEditable(true);
        label_11 = new QLabel(widget_class1);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(10, 90, 81, 20));
        QFont font2;
        font2.setPointSize(9);
        label_11->setFont(font2);
        widgetClass = new QWidget(widget_class1);
        widgetClass->setObjectName(QStringLiteral("widgetClass"));
        widgetClass->setGeometry(QRect(0, 0, 431, 41));
        label_5 = new QLabel(widgetClass);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 12, 41, 15));
        label_5->setFont(font1);
        comboBox_class = new QComboBox(widgetClass);
        comboBox_class->setObjectName(QStringLiteral("comboBox_class"));
        comboBox_class->setGeometry(QRect(90, 10, 331, 20));
        comboBox_class->setEditable(true);
        widget_type = new QWidget(groupBox_4);
        widget_type->setObjectName(QStringLiteral("widget_type"));
        widget_type->setEnabled(true);
        widget_type->setGeometry(QRect(30, 280, 431, 121));
        label_6 = new QLabel(widget_type);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 2, 41, 15));
        label_6->setFont(font1);
        listWidget_type = new QListWidget(widget_type);
        listWidget_type->setObjectName(QStringLiteral("listWidget_type"));
        listWidget_type->setGeometry(QRect(90, 2, 256, 61));
        listWidget_type->setAlternatingRowColors(false);
        listWidget_type->setSelectionMode(QAbstractItemView::MultiSelection);
        label_7 = new QLabel(widget_type);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 80, 51, 16));
        label_7->setFont(font1);
        lineEdit_level = new QLineEdit(widget_type);
        lineEdit_level->setObjectName(QStringLiteral("lineEdit_level"));
        lineEdit_level->setGeometry(QRect(90, 80, 101, 20));
        lineEdit_level->setFont(font);
        lineEdit_level->setInputMethodHints(Qt::ImhDigitsOnly);
        label_2 = new QLabel(DialogSearch);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 10, 51, 39));
        QFont font3;
        font3.setFamily(QStringLiteral("Segoe UI"));
        font3.setPointSize(11);
        label_2->setFont(font3);
        pButton_find = new QPushButton(DialogSearch);
        pButton_find->setObjectName(QStringLiteral("pButton_find"));
        pButton_find->setGeometry(QRect(500, 30, 71, 31));
        pButton_find->setDefault(true);
        pButton_close = new QPushButton(DialogSearch);
        pButton_close->setObjectName(QStringLiteral("pButton_close"));
        pButton_close->setGeometry(QRect(500, 80, 71, 31));
        pButton_close->setAutoDefault(false);

        retranslateUi(DialogSearch);
        QObject::connect(radioButton_class1, SIGNAL(toggled(bool)), widget_class1, SLOT(setEnabled(bool)));
        QObject::connect(radioButton_class2, SIGNAL(toggled(bool)), widget_class2, SLOT(setEnabled(bool)));
        QObject::connect(radioButton_name, SIGNAL(toggled(bool)), groupBox_4, SLOT(setEnabled(bool)));
        QObject::connect(pButton_close, SIGNAL(clicked()), DialogSearch, SLOT(close()));

        QMetaObject::connectSlotsByName(DialogSearch);
    } // setupUi

    void retranslateUi(QDialog *DialogSearch)
    {
        DialogSearch->setWindowTitle(QApplication::translate("DialogSearch", "\320\237\320\276\320\270\321\201\320\272", 0));
        groupBox_3->setTitle(QApplication::translate("DialogSearch", "\320\234\320\265\321\202\320\276\320\264 \320\277\320\276\320\270\321\201\320\272\320\260", 0));
        radioButton_fullSearch->setText(QApplication::translate("DialogSearch", "\320\277\320\276\320\273\320\275\320\276\320\265 \321\201\320\276\320\276\321\202\320\262\320\265\321\201\321\202\320\262\320\270\320\265", 0));
        radioButton_partSearch->setText(QApplication::translate("DialogSearch", "\320\275\320\265\320\277\320\276\320\273\320\275\320\276\320\265 \321\201\320\276\320\276\321\202\320\262\320\265\321\202\321\201\321\202\320\262\320\270\320\265", 0));
        radioButton_wildcard->setText(QApplication::translate("DialogSearch", "\320\277\320\276 \321\210\320\260\320\261\320\273\320\276\320\275\321\203", 0));
        groupBox_5->setTitle(QApplication::translate("DialogSearch", "\320\240\320\265\320\266\320\270\320\274 \320\277\320\276\320\270\321\201\320\272\320\260", 0));
        radioButton_name->setText(QApplication::translate("DialogSearch", "\320\277\320\276 \320\275\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\321\216", 0));
        radioButton_code->setText(QApplication::translate("DialogSearch", "\320\277\320\276 \320\272\320\276\320\264\321\203", 0));
        groupBox_4->setTitle(QApplication::translate("DialogSearch", "\320\232\321\200\320\270\321\202\320\265\321\200\320\270\320\270", 0));
        label_4->setText(QApplication::translate("DialogSearch", "\320\232\320\273\320\260\321\201\321\201:", 0));
        radioButton_class1->setText(QString());
        radioButton_class2->setText(QString());
        comboBox_codeInGroup->setCurrentText(QApplication::translate("DialogSearch", "\320\235\320\265 \320\262\320\260\320\266\320\275\320\276", 0));
        label_3->setText(QApplication::translate("DialogSearch", "\320\223\321\200\321\203\320\277\320\277\320\260:", 0));
        comboBox_group->setCurrentText(QApplication::translate("DialogSearch", "\320\235\320\265 \320\262\320\260\320\266\320\275\320\276", 0));
        label_11->setText(QApplication::translate("DialogSearch", "\320\232\320\276\320\264 \320\262 \320\263\321\200\321\203\320\277\320\277\320\265:", 0));
        label_5->setText(QApplication::translate("DialogSearch", "\320\232\320\273\320\260\321\201\321\201", 0));
        comboBox_class->setCurrentText(QString());
        label_6->setText(QApplication::translate("DialogSearch", "\320\242\320\270\320\277:", 0));
        label_7->setText(QApplication::translate("DialogSearch", "\320\243\321\200\320\276\320\262\320\265\320\275\321\214:", 0));
        label_2->setText(QApplication::translate("DialogSearch", "\320\242\320\265\320\272\321\201\321\202:", 0));
        pButton_find->setText(QApplication::translate("DialogSearch", "\320\237\320\276\320\270\321\201\320\272", 0));
        pButton_close->setText(QApplication::translate("DialogSearch", "\320\236\321\202\320\274\320\265\320\275\320\260", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogSearch: public Ui_DialogSearch {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCH_H
