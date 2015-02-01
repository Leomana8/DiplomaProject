/********************************************************************************
** Form generated from reading UI file 'edit.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDIT_H
#define UI_EDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_DialogEdit
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label_2;
    QLineEdit *lineEdit_name;
    QLabel *label_code;

    void setupUi(QDialog *DialogEdit)
    {
        if (DialogEdit->objectName().isEmpty())
            DialogEdit->setObjectName(QStringLiteral("DialogEdit"));
        DialogEdit->resize(468, 122);
        buttonBox = new QDialogButtonBox(DialogEdit);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(370, 40, 81, 241));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label_2 = new QLabel(DialogEdit);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 30, 121, 39));
        QFont font;
        font.setPointSize(11);
        label_2->setFont(font);
        lineEdit_name = new QLineEdit(DialogEdit);
        lineEdit_name->setObjectName(QStringLiteral("lineEdit_name"));
        lineEdit_name->setGeometry(QRect(10, 70, 331, 20));
        label_code = new QLabel(DialogEdit);
        label_code->setObjectName(QStringLiteral("label_code"));
        label_code->setGeometry(QRect(10, 0, 121, 39));
        label_code->setFont(font);

        retranslateUi(DialogEdit);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogEdit, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogEdit, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogEdit);
    } // setupUi

    void retranslateUi(QDialog *DialogEdit)
    {
        DialogEdit->setWindowTitle(QApplication::translate("DialogEdit", "\320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265", 0));
        label_2->setText(QApplication::translate("DialogEdit", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265", 0));
        label_code->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DialogEdit: public Ui_DialogEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDIT_H
