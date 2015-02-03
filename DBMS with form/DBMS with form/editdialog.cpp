#include "editdialog.h"


EditDialog::EditDialog(QString name, QString code, QWidget *parent) : QDialog(parent)
{
	this->setLocale(QLocale(QLocale::Russian));
	ui.setupUi(this);
	ui.lineEdit_name->setText(name);
	ui.label_code->setText(code);
}

EditDialog::~EditDialog()
{

}
QString EditDialog::getName()
{
	return ui.lineEdit_name->text();
}
