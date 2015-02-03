#include <QtWidgets/QMainWindow>
#include "ui_edit.h"

class EditDialog : public QDialog
{
	Q_OBJECT

public:
	EditDialog(QString name, QString code, QWidget *parent = 0);
	QString getName();
	~EditDialog();
private:
	Ui::DialogEdit ui;
};