#ifndef DBMSWITHFORM_H
#define DBMSWITHFORM_H

#include <QtWidgets/QMainWindow>
#include "ui_dbmswithform.h"
#include "DbmsElementsOfStructure.h"
#include "searchdialog.h"
#include "editdialog.h"

struct ConceptualItem
{
	Category c;
	int codeGroup;
	int codeInGroup;
	int idParametr;
	int extraCode;
	QString name;
};
struct InfologicalItem
{
	Category c;
	QString code_1;
	QString code_2;
	int idCategory;
	QChar type;
	int level;
	QString name;
};

class DBMSwithform : public QMainWindow
{
	Q_OBJECT

public:
	DBMSwithform(QWidget *parent = 0);
	~DBMSwithform();

private:
	Ui::MainWindow ui;
	QAbstractTableModel *m; // модель для таблиц
	UserType typeUser;
	QString dbPath;
	QTimer* timerInfo;
	QStringList namesClass; // наименования классов в нужном порядке
	QMap<QString, Category> nameCategory; // наименование класса - класс
	SearchDialog *findDialog;
	QString selectedId; // индекс выбранной строки при вызове контекстного меню
	QString selectedCode;
	QString selectedName;
	QMenu tabMenu; // контекстное меню таблиц
	void initializeInitialModel();
	void initializeConceptualModel();
	void initializeInfologicalModel();
private slots: 

	void changedTypeUser(bool);
	void showTableInitial();
	void showTable(int);
	void changedGroupInitial(int);
	void on_pButton_GenerateCodeInitial_clicked();
	void on_pButton_AddInitial_clicked();
	void on_pButton_SearchInitial_clicked();
	void showFindTableInitial(QAbstractTableModel*);
	void showContextMenu(const QPoint&);
	void showTableInitialQueries(bool);

	void showTableConceptual(int);
	void changedClassConceptual(int);
	void changedGroupConceptual(int);
	void generateExtraCodeConceptual(QString);
	void on_pButton_GenerateCodeConceptual_clicked();
	void on_pButton_AddConceptual_clicked();
	ConceptualItem getConceptualItem();
	void showFindTableConceptual(QAbstractTableModel*);
	void on_pButton_SearchConceptual_clicked();
	void showTableConceptualQueries(bool);

	void changedClassInfological(int);
	void on_pButton_GenerateCodeInfological_clicked();
	InfologicalItem getInfologicalItem();
	void showTableInfological(int);
	void on_pButton_AddInfological_clicked();
	void showFindTableInfological(QAbstractTableModel*);
	void on_pButton_SearchInfological_clicked();
	void showTableInfologicalQueries(bool);
	void on_radioButton_new_clicked(bool);
	void selectedTabMenu(QAction *);
	void bDelete_clicked();
	void bConfirm_clicked();
};

#endif // DBMSWITHFORM_H
