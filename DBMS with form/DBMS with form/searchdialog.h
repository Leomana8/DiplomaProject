#include <QtWidgets/QMainWindow>
#include "ui_search.h"
#include "DbmsElementsOfStructure.h"

struct Item
{
	Category c;
	QVariant codeGroup;
	QVariant codeInGroup;
	SearchType t;
	QString text;
	QList<Category> listClass;
	QList<QChar> listType;
	int level;
};

class SearchDialog : public QDialog
{
	Q_OBJECT

public:
	SearchDialog(int m, QWidget *parent = 0);
	void pushFind();
	~SearchDialog();
private:
	Ui::DialogSearch ui;
	int model;
	// наименования классов
	QStringList namesClass;
	QMap<QString, Category> nameCategory; 

	void initializeInitialModel();
	void initializeConceptualModel();
	void initializeInfologicalModel();

	void findInitial(); 
	void findConceptual(); 
	void findInfological();
	Item getItem();

signals:
	void finded(QAbstractTableModel*);

private slots:
	void on_pButton_find_clicked();
	void changedClass(int indx);
	void changedCodeGroup(int indx);

};