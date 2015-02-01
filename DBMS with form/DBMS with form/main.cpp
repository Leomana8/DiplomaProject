#include "dbmswithform.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QLocale::setDefault(QLocale(QLocale::Russian));
	DBMSwithform w;
	QComboBox h;
	w.show();
	return a.exec();
}
