#include "dbmswithform.h"
#include <QMessageBox>


DBMSwithform::DBMSwithform(QWidget *parent)
	: QMainWindow(parent)
{
	
	ui.setupUi(this);
	timerInfo = new QTimer();
	dbPath = "db_elements_of_structures.sqlite";
	DbElementsOfStructure::connect(dbPath);
	typeUser = EXPERT;
	findDialog = NULL;
	m = NULL;
	ui.tabWidget_selectModel->setCurrentIndex(0);
	ui.tabWidget_workFieldInitial->setCurrentIndex(1);
	namesClass = DbElementsOfStructure::getSortClassNames();
	nameCategory = DbElementsOfStructure::getCategoryNames();
	ui.tableViewInitial->setContextMenuPolicy(Qt::CustomContextMenu);
	ui.tableViewConceptual->setContextMenuPolicy(Qt::CustomContextMenu);
	ui.tableViewInfological->setContextMenuPolicy(Qt::CustomContextMenu);
	connect( ui.radioButton_expert, SIGNAL(toggled(bool)), this, SLOT(changedTypeUser(bool)));
	connect( timerInfo, SIGNAL(timeout()), ui.label_infoInitial, SLOT(clear()) );
	connect( timerInfo, SIGNAL(timeout()), ui.label_infoConceptual, SLOT(clear()) );
	connect( timerInfo, SIGNAL(timeout()), ui.label_infoInfological, SLOT(clear()) );
	connect( ui.tabWidget_selectModel, SIGNAL(currentChanged(int)), this, SLOT(showTable(int)) );
	connect( ui.tabWidget_workFieldInitial, SIGNAL(currentChanged(int)), this, SLOT(showTable(int)) );
	connect( ui.tabWidget_workFieldConceptual, SIGNAL(currentChanged(int)), this, SLOT(showTable(int)) );
	connect( ui.tabWidget_workFieldInfological, SIGNAL(currentChanged(int)), this, SLOT(showTable(int)) );
	connect( ui.comboBox_groupInitial, SIGNAL(currentIndexChanged(int)), this, SLOT(changedGroupInitial(int)) );
	connect( ui.tableViewInitial, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(showContextMenu(const QPoint&)));
	connect( ui.radioButton_insertI, SIGNAL(toggled(bool)), this, SLOT(showTableInitialQueries(bool)));
	connect( ui.comboBox_tableConceptual, SIGNAL(currentIndexChanged(int)), this, SLOT(showTableConceptual(int)) );
	connect( ui.comboBox_classConceptual, SIGNAL(currentIndexChanged(int)), this, SLOT(changedClassConceptual(int)) );
	connect( ui.comboBox_groupConceptual, SIGNAL(currentIndexChanged(int)), this, SLOT(changedGroupConceptual(int)) );
	connect( ui.comboBox_codeInGroupConceptual, SIGNAL(currentIndexChanged(QString)), this, SLOT(generateExtraCodeConceptual(QString)) );
	connect( ui.comboBox_parametrConceptual, SIGNAL(currentIndexChanged(QString)), this, SLOT(generateExtraCodeConceptual(QString)) );
	connect( ui.lineEdit_codeInGroupConceptual, SIGNAL(textEdited(QString)), this, SLOT(generateExtraCodeConceptual(QString)) );
	connect( ui.tableViewConceptual, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(showContextMenu(const QPoint&)));
	connect( ui.radioButton_insertC, SIGNAL(toggled(bool)), this, SLOT(showTableConceptualQueries(bool)));
	connect( ui.comboBox_classInfological, SIGNAL(currentIndexChanged(int)), this, SLOT(changedClassInfological(int)) );
	connect( ui.comboBox_tableInfological, SIGNAL(currentIndexChanged(int)), this, SLOT(showTableInfological(int)) );
	connect( ui.tableViewInfological, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(showContextMenu(const QPoint&)));
	connect( ui.radioButton_insertInf, SIGNAL(toggled(bool)), this, SLOT(showTableInfologicalQueries(bool)));

	initializeInitialModel();
	initializeConceptualModel();
	initializeInfologicalModel();
	connect (&tabMenu, SIGNAL(triggered(QAction*)), this, SLOT(selectedTabMenu(QAction *)) );
}

DBMSwithform::~DBMSwithform()
{
	DbElementsOfStructure::disconnect();
}

// изменение типа пользователя
void DBMSwithform::changedTypeUser(bool expert)
{
	if(expert)
	{
		typeUser = EXPERT;
	}
	else
	{
		typeUser = USER;
	}
}
// отобразить видимую таблицу
void DBMSwithform::showTable(int)
{
	if(ui.ViewTablesInitial->isVisible())
	{
		showTableInitial();
	}
	if(ui.ViewTablesConceptual->isVisible())
	{
		showTableConceptual(ui.comboBox_tableConceptual->currentIndex());
	}
	if(ui.ViewTablesInfological->isVisible())
	{
		showTableInfological(ui.comboBox_tableInfological->currentIndex());
	}
	if(ui.QueriesInitial->isVisible())
	{
		showTableInitialQueries(ui.radioButton_insertI->isChecked());
	}
	if(ui.QueriesConceptual->isVisible())
	{
		showTableConceptualQueries(ui.radioButton_insertC->isChecked());
	}
	if(ui.QueriesInfological->isVisible())
	{
		showTableInfologicalQueries(ui.radioButton_insertInf->isChecked());
	}

	if(findDialog)
	{
		findDialog->close();
		findDialog = NULL;
	}
}

// отобразить контекстное меню таблицы
void DBMSwithform::showContextMenu(const QPoint& pos)
{
	QTableView *cur;
	if(ui.tabWidget_selectModel->currentIndex() == 0)
	{
		cur = ui.tableViewInitial;
	}
	if(ui.tabWidget_selectModel->currentIndex() == 1)
	{
		cur = ui.tableViewConceptual;
	}
	if(ui.tabWidget_selectModel->currentIndex() == 2)
	{
		cur = ui.tableViewInfological;
	}
	QPoint globalPos = cur->viewport()->mapToGlobal(pos);
	int selectedRow = cur->indexAt(pos).row();
	selectedId = cur->model()->index(selectedRow, 0).data().toString();
	selectedCode = cur->model()->index(selectedRow, 1).data().toString();
	selectedName = cur->model()->index(selectedRow, 2).data().toString();
	tabMenu.clear();
	tabMenu.addAction(QStringLiteral("Редактировать"));
	if(typeUser == EXPERT)
	{
		tabMenu.addAction(QStringLiteral("Удалить"));
	}
	tabMenu.exec(globalPos);

}

void DBMSwithform::selectedTabMenu(QAction * item)
{
	
	int model = ui.tabWidget_selectModel->currentIndex();
	Category c = ATTRIBUTES;
	if(item->text() == QStringLiteral("Удалить"))
	{
		DbElementsOfStructure* db;
		if(model == 0)
		{
			db = new DbInitialModel();
			db->deleteRecord(selectedId, c);
			showTableInitial();
		}
		if(model == 1)
		{
			db = new DbConceptualModel;
			c = nameCategory[ui.comboBox_tableConceptual->currentText()];
			db->deleteRecord(selectedId, c);
			int t = ui.comboBox_tableConceptual->currentIndex();
			if(t == -1)
			{
				findDialog->pushFind();
			}
			else
			{
				showTableConceptual(t);
			}		
		}
		if(model == 2)
		{
			db = new DbInfologicalModel;
			c = nameCategory[ui.comboBox_tableInfological->currentText()];
			db->deleteRecord(selectedId, c);
			showTableInfological(ui.comboBox_tableInfological->currentIndex());
		}	
	}
	if(item->text() == QStringLiteral("Редактировать"))
	{
		EditDialog dialogEdit(selectedName,selectedCode,this);
		QString newName;
		if(dialogEdit.exec())
		{
			newName = dialogEdit.getName();
		}
		if(dialogEdit.result() == QDialog::Accepted)
		{
			DbElementsOfStructure* db;
			if(model == 0)
			{
				db = new DbInitialModel;
				db->editRecord(typeUser, selectedId, newName);
				showTableInitial();
			}
			if(model == 1)
			{
				db = new DbConceptualModel;
				c = nameCategory[ui.comboBox_tableConceptual->currentText()];
				db->editRecord(typeUser, selectedId, newName, c);
				showTableConceptual(ui.comboBox_tableConceptual->currentIndex());
			}
			if(model == 2)
			{
				db = new DbInfologicalModel;
				c = nameCategory[ui.comboBox_tableInfological->currentText()];
				db->editRecord(typeUser, selectedId, newName, c);
				showTableInfological(ui.comboBox_tableInfological->currentIndex());
			}
		}	
	}
	
}

// нажата кнопка для удаления запроса пользователя
void DBMSwithform::bDelete_clicked()
{
	QString id = QObject::sender()->property("id").toString();
	DbElementsOfStructure::deleteRecord(id);
	showTable(0);
}
// нажата кнопка для подверждения запроса пользователя
void DBMSwithform::bConfirm_clicked()
{
	QString id = QObject::sender()->property("id").toString();
	DbElementsOfStructure::confirmQuery(id);
	showTable(0);
}

//  НАЧАЛЬНАЯ МОДЕЛЬ

void DBMSwithform::initializeInitialModel()
{
	DbInitialModel db;
	QList<QPair<QString, int>> codesGroup = db.getClassifierCodeGroup();
	QPair<QString, int> var;
	foreach(var, codesGroup)
	{
		ui.comboBox_groupInitial->addItem(var.first, var.second);
	}
}


// при изменении кода группы
void DBMSwithform::changedGroupInitial(int indx)
{
	DbInitialModel db;
	QString codeInGroup = QString::number(db.getValidCodeInGroupe(indx));
	ui.lineEdit_codeInGroupInitial->setText(codeInGroup);
}

// генерация кода
void DBMSwithform::on_pButton_GenerateCodeInitial_clicked()
{
	DbInitialModel db;
	QString code = db.generateCode(ui.comboBox_groupInitial->currentIndex(), ui.lineEdit_codeInGroupInitial->text().toInt());
	ui.lineEdit_codeInitial->setText(code);
}

// добавить запись в начальную модель
void DBMSwithform::on_pButton_AddInitial_clicked()
{
	if(ui.lineEdit_nameInitial->text() == "")
	{
		ui.label_infoInitial->setText(QStringLiteral("Заполните наименование"));
		ui.label_infoInitial->setStyleSheet("color: rgb(50, 0, 70)");
		timerInfo->start(5000);
		return;
	}
	DbInitialModel db;
	int res = db.insertRecord(
		typeUser,
		ui.comboBox_groupInitial->currentIndex(),
		ui.lineEdit_codeInGroupInitial->text().toInt(),
		ui.lineEdit_nameInitial->text()
		);
	if(res == -2)
	{
		ui.label_infoInitial->setText(QStringLiteral("Запрос с таким кодом существует"));
		ui.label_infoInitial->setStyleSheet("color: rgb(50, 0, 70)");
		timerInfo->start(5000);
	}
	if(res == 0)
	{
		ui.label_infoInitial->setText(QStringLiteral("Запись добавлена"));
		ui.label_infoInitial->setStyleSheet("color: rgb(0, 50, 70)");
		timerInfo->start(5000);
		ui.lineEdit_nameInitial->clear();
		QString codeInGroup = QString::number(db.getValidCodeInGroupe(ui.comboBox_groupInitial->currentIndex()));
		ui.lineEdit_codeInGroupInitial->setText(codeInGroup);
	}

}

// показать таблицу начального уровня
void DBMSwithform::showTableInitial()
{
	DbInitialModel db;
	if(m != NULL)
	{
		delete m;
		m = NULL;
	}
	m = db.getTable();
	ui.tableViewInitial->setModel(m);
	ui.tableViewInitial->hideColumn(0);
	QHeaderView *header = ui.tableViewInitial->horizontalHeader();
	if(header != NULL)
	{
		header->setSectionResizeMode(1,QHeaderView::ResizeToContents);
		header->setSectionResizeMode(2,QHeaderView::Stretch);
	}
	ui.tableViewInitial->show();
	
}

// таблица запросов
void DBMSwithform::showTableInitialQueries(bool insert)
{
	DbInitialModel db;
	if(m != NULL)
	{
		delete m;
		m = NULL;
	}
	if(insert)
	{
		m = db.getTableQueriesInsert();
	}
	else
	{
		m = db.getTableQueriesEdit();
	}
	ui.tableViewInitial_queries->setModel(m);
	// добавление кнопок к строкам если выбран эксперт
	if (typeUser == EXPERT)
	{
		int count = m->rowCount();
		m->insertColumns(m->columnCount(),2);
		m->setHeaderData(m->columnCount()-1, Qt::Horizontal, (""));
		m->setHeaderData(m->columnCount()-2, Qt::Horizontal, (""));
		while(--count + 1)
		{
			QPushButton *bConfirm = new QPushButton(QStringLiteral("Подтвердить"));
			bConfirm->setProperty("id", m->data(m->index(count, 0))); // запись id записи в кнопку
			connect( bConfirm, SIGNAL(clicked()), this, SLOT(bConfirm_clicked()));
			QPushButton *bDelete = new QPushButton(QStringLiteral("Удалить")); 
			connect( bDelete, SIGNAL(clicked()), this, SLOT(bDelete_clicked()));
			bDelete->setProperty("id", m->data(m->index(count, 0)));
			QModelIndex curIndx = m->index(count,m->columnCount()-2);
			delete ui.tableViewInitial_queries->indexWidget(curIndx);
			ui.tableViewInitial_queries->setIndexWidget(curIndx, bConfirm);
			curIndx = m->index(count,m->columnCount()-1);
			delete ui.tableViewInitial_queries->indexWidget(curIndx);
			ui.tableViewInitial_queries->setIndexWidget(curIndx, bDelete);
		}
	}
	ui.tableViewInitial_queries->hideColumn(0);
	QHeaderView *header = ui.tableViewInitial_queries->horizontalHeader();
	if(header != NULL)
	{
		header->setSectionResizeMode(1,QHeaderView::ResizeToContents);
		header->setSectionResizeMode(2,QHeaderView::Stretch);
	}
	ui.tableViewInitial_queries->show();
}
// Поиск
void DBMSwithform::on_pButton_SearchInitial_clicked()
{
	if (!findDialog) {
		findDialog = new SearchDialog(1,this);
		connect(findDialog, SIGNAL(finded(QAbstractTableModel*)), this, SLOT(showFindTableInitial(QAbstractTableModel*)));
	}
	findDialog->show();
	findDialog->raise();
	findDialog->activateWindow();
}

void DBMSwithform::showFindTableInitial(QAbstractTableModel* mF)
{
	if(m != NULL)
	{
		delete m;
		m = NULL;
	}
	m = mF;
	ui.tableViewInitial->setModel(m);
	ui.tableViewInitial->hideColumn(0);
	QHeaderView *header = ui.tableViewInitial->horizontalHeader();
	if(header != NULL)
	{
		header->setSectionResizeMode(1,QHeaderView::ResizeToContents);
		header->setSectionResizeMode(2,QHeaderView::Stretch);
	}
	ui.tableViewInitial->show();
}



// КОНЦЕПТУАЛЬНАЯ МОДЕЛЬ

void DBMSwithform::initializeConceptualModel()
{
	DbConceptualModel db;
	ui.comboBox_tableConceptual->addItems(namesClass);
	ui.comboBox_classConceptual->addItems(namesClass);
	ui.comboBox_codeInGroupConceptual->setVisible(false);
}

// отобразить таблицу
void DBMSwithform::showTableConceptual(int indx)
{
	DbConceptualModel db;
	if(m != NULL)
	{
		delete m;
		m = NULL;
	}
	if(indx >= 0)
	{
		m = db.getTable(nameCategory[ui.comboBox_tableConceptual->itemText(indx)]);	
		ui.tableViewConceptual->setModel(m);
		ui.tableViewConceptual->hideColumn(0);
		QHeaderView *header = ui.tableViewConceptual->horizontalHeader();
		if(header != NULL)
		{
			header->setSectionResizeMode(1,QHeaderView::ResizeToContents);
			header->setSectionResizeMode(2,QHeaderView::Stretch);
		}
		ui.tableViewConceptual->show();
	}
}

void DBMSwithform::changedClassConceptual(int indx)
{
	Category c = nameCategory[ui.comboBox_classConceptual->itemText(indx)];
	DbConceptualModel db;
	ui.lineEdit_codeInGroupConceptual->setVisible(true);
	ui.comboBox_codeInGroupConceptual->setVisible(false);
	ui.groupBox_codes->setEnabled(true);
	ui.groupBox_parametr->setEnabled(false);
	ui.comboBox_groupConceptual->clear();
	ui.comboBox_codeInGroupConceptual->clear();
	if (c == VALUES)
	{
		;
	}
	if(c == ATTRIBUTES)
	{
		ui.groupBox_codes->setEnabled(false);
		ui.groupBox_parametr->setEnabled(true);
		ui.comboBox_parametrConceptual->clear();
		QMap<QString, int> parametrs = db.getParametrs();
		foreach(QString var, parametrs.keys())
		{
			ui.comboBox_parametrConceptual->addItem(var, parametrs[var]);
		}
	}
	else
	{
		QList<QPair<QString, int>> codesGroup = db.getClassifierCodeGroup(c);
		QPair<QString, int> var;
		foreach(var, codesGroup)
		{
			ui.comboBox_groupConceptual->addItem(var.first, var.second);
		}
		if(c == COMPONENTS)
		{
			ui.lineEdit_codeInGroupConceptual->setVisible(false);
			ui.comboBox_codeInGroupConceptual->setVisible(true);
			QList<QPair<QString, int>> codesInGroup = db.getClassifierCodeInGroupComponent();
			QPair<QString, int> var;
			foreach(var, codesInGroup)
			{
				ui.comboBox_codeInGroupConceptual->addItem(var.first, var.second);
			}
			
		}
		else
		{
			QString codeInGroup = QString::number(db.getValidCodeInGroupe(getConceptualItem().codeGroup, c));
			ui.lineEdit_codeInGroupConceptual->setText(codeInGroup);
			generateExtraCodeConceptual("");
		}
	}
}
// 
void DBMSwithform::changedGroupConceptual(int indx)
{
	Category c = nameCategory[ui.comboBox_classConceptual->currentText()];
	if(c != ATTRIBUTES && c != COMPONENTS)
	{
		DbConceptualModel db;
		QString codeInGroup = QString::number(db.getValidCodeInGroupe(getConceptualItem().codeGroup, c));
		ui.lineEdit_codeInGroupConceptual->setText(codeInGroup);
	}

}
ConceptualItem DBMSwithform::getConceptualItem()
{
	ConceptualItem item;
	item.extraCode = ui.lineEdit_extraCodeConceptual->text().toInt();
	item.c = nameCategory[ui.comboBox_classConceptual->currentText()];
	if(item.c == ATTRIBUTES)
	{
		QString j=ui.comboBox_parametrConceptual->currentText();
		item.idParametr = ui.comboBox_parametrConceptual->currentData().toInt();
	}
	else
	{
		item.codeGroup = ui.comboBox_groupConceptual->currentData().toInt();
		if(item.c == COMPONENTS)
		{
			item.codeInGroup = ui.comboBox_codeInGroupConceptual->currentData().toInt();
		}
		else
		{
			item.codeInGroup = ui.lineEdit_codeInGroupConceptual->text().toInt();
		}
	}
	item.name = ui.lineEdit_nameConceptual->text();
	return item;
}

void DBMSwithform::generateExtraCodeConceptual(QString)
{
	DbConceptualModel db;
	QString extraCode;
	ConceptualItem item = getConceptualItem();
	if(item.c == ATTRIBUTES)
	{
		extraCode = QString::number(db.getValidExtraCode(item.idParametr));
		QStringList name = ui.comboBox_parametrConceptual->currentText().split(" ");
		ui.lineEdit_nameConceptual->setText(name.at(name.size()-1));
	}
	else
	{
		extraCode = QString::number(db.getValidExtraCode(item.c, item.codeGroup, item.codeInGroup));
	}
	ui.lineEdit_extraCodeConceptual->setText(extraCode);	
}

void DBMSwithform::on_pButton_GenerateCodeConceptual_clicked()
{
	DbConceptualModel db;
	QString code;
	ConceptualItem item = getConceptualItem();
	if(item.c == ATTRIBUTES)
	{
		code = db.generateCode(item.idParametr, item.extraCode);
	}
	else
	{
		code = db.generateCode(item.c, item.codeGroup, item.codeInGroup, item.extraCode);
	}
	ui.lineEdit_codeConceptual->setText(code);
}

void DBMSwithform::on_pButton_AddConceptual_clicked()
{
	ConceptualItem item = getConceptualItem();
	if(item.name == "")
	{
		ui.label_infoConceptual->setText(QStringLiteral("Заполните наименование"));
		ui.label_infoConceptual->setStyleSheet("color: rgb(50, 0, 70)");
		timerInfo->start(5000);
		return;
	}
	DbConceptualModel db;
	int res;
	if(item.c == ATTRIBUTES)
	{
		res = db.insertRecord(typeUser, item.idParametr, item.extraCode, item.name);
		if(res == -2)
		{
			ui.label_infoConceptual->setText(QStringLiteral("Параметра не существует")); 
			ui.label_infoConceptual->setStyleSheet("color: rgb(50, 0, 70)");
		}
	}
	else
	{
		res = db.insertRecord(
			typeUser,
			item.c,
			item.codeGroup,
			item.codeInGroup,
			item.extraCode,
			item.name
			);

		if(res == -2) 
		{
			ui.label_infoConceptual->setText(QStringLiteral("Недопустимый код группы")); 
			ui.label_infoConceptual->setStyleSheet("color: rgb(50, 0, 70)");
		}
		if(res == -3)
		{
			ui.label_infoConceptual->setText(QStringLiteral("Недопустимый код в группе")); 
			ui.label_infoConceptual->setStyleSheet("color: rgb(50, 0, 70)");
		}
	}
	switch (res)
	{
	case 0:
		{
			ui.label_infoConceptual->setText(QStringLiteral("Запись добавлена"));
			ui.label_infoConceptual->setStyleSheet("color: rgb(0, 50, 70)");
			break;
		}
	case -4:
		{
			ui.label_infoConceptual->setText(QStringLiteral("Недопустимый дополнительный код")); 
			ui.label_infoConceptual->setStyleSheet("color: rgb(50, 0, 70)");
			break;
		}
	case -5:
		{
			ui.label_infoConceptual->setText(QStringLiteral("Пустое имя")); 
			ui.label_infoConceptual->setStyleSheet("color: rgb(50, 0, 70)");
			break;
		}
	case -6:
		{
			ui.label_infoConceptual->setText(QStringLiteral("Запрос с таким кодом существует")); 
			ui.label_infoConceptual->setStyleSheet("color: rgb(50, 0, 70)");
			break;
		}
	}
	timerInfo->start(5000);	
	generateExtraCodeConceptual("");
}

void DBMSwithform::on_pButton_SearchConceptual_clicked()
{
	ui.comboBox_tableConceptual->setCurrentIndex(-1);
	if (!findDialog) {
		findDialog = new SearchDialog(2, this);
		connect(findDialog, SIGNAL(finded(QAbstractTableModel*)), this, SLOT(showFindTableConceptual(QAbstractTableModel*)));
	}
	findDialog->show();
	findDialog->raise();
	findDialog->activateWindow();
}

void DBMSwithform::showFindTableConceptual(QAbstractTableModel* mF)
{
	if(m != NULL)
	{
		delete m;
		m = NULL;
	}
	m = mF;
	ui.tableViewConceptual->setModel(m);
	ui.tableViewConceptual->hideColumn(0);
	QHeaderView *header = ui.tableViewConceptual->horizontalHeader();
	if(header != NULL)
	{
		header->setSectionResizeMode(1,QHeaderView::ResizeToContents);
		header->setSectionResizeMode(2,QHeaderView::Stretch);
	}
	ui.tableViewConceptual->show();
}
void DBMSwithform::showTableConceptualQueries(bool insert)
{
	DbConceptualModel db;
	if(m != NULL)
	{
		delete m;
		m = NULL;
	}
	if(insert)
	{
		m = db.getTableQueriesInsert();
	}
	else
	{
		m = db.getTableQueriesEdit();
	}
	ui.tableViewConceptual_queries->setModel(m);
	QHeaderView *header = ui.tableViewConceptual_queries->horizontalHeader();
	// добавление кнопок к строкам если выбран эксперт
	if (typeUser == EXPERT)
	{
		int count = m->rowCount();
		m->insertColumns(m->columnCount(),2);
		m->setHeaderData(m->columnCount()-1, Qt::Horizontal, (""));
		m->setHeaderData(m->columnCount()-2, Qt::Horizontal, (""));
		
		while(--count + 1)
		{
			QPushButton *bConfirm = new QPushButton(QStringLiteral("Подтвердить"));
			bConfirm->setProperty("id", m->data(m->index(count, 0))); // запись id записи в кнопку
			connect( bConfirm, SIGNAL(clicked()), this, SLOT(bConfirm_clicked()));
			QPushButton *bDelete = new QPushButton(QStringLiteral("Удалить")); 
			connect( bDelete, SIGNAL(clicked()), this, SLOT(bDelete_clicked()));
			bDelete->setProperty("id", m->data(m->index(count, 0)));
			QModelIndex curIndx = m->index(count,m->columnCount()-2);
			delete ui.tableViewConceptual_queries->indexWidget(curIndx);
			ui.tableViewConceptual_queries->setIndexWidget(curIndx, bConfirm);
			curIndx = m->index(count,m->columnCount()-1);
			delete ui.tableViewConceptual_queries->indexWidget(curIndx);
			ui.tableViewConceptual_queries->setIndexWidget(curIndx, bDelete);
		}
	}
	ui.tableViewConceptual_queries->hideColumn(0);
	
	if(header != NULL)
	{
		header->resizeSections(QHeaderView::ResizeToContents);
		header->setSectionResizeMode(2,QHeaderView::Stretch);
		header->setSectionResizeMode(3,QHeaderView::Stretch);
	}
	ui.tableViewConceptual_queries->show();
}

// ИНФОЛОГИЧЕСКАЯ МОДЕЛЬ


void DBMSwithform::initializeInfologicalModel()
{
	DbInfologicalModel db;
	ui.comboBox_tableInfological->addItems(namesClass);
	ui.comboBox_classInfological->addItems(namesClass);
	QMap<QString, QChar> types = db.getTypes();
	foreach(QString var, types.keys())
	{
		ui.comboBox_typeInfological->addItem(var, types[var]);
	}
}

void DBMSwithform::changedClassInfological(int indx)
{
	DbInfologicalModel db;
	Category c = nameCategory[ui.comboBox_classInfological->itemText(indx)];
	QMap<QString, int> categories = db.getCategories(c);
	ui.comboBox_categoryInfological->clear();
	foreach(QString var, categories.keys())
	{
		ui.comboBox_categoryInfological->addItem(var, categories[var]);
	}

	if(ui.radioButton_new->isChecked())
	{
		ui.lineEdit_codeInfological_1->setText(QString(c));
		ui.lineEdit_codeInfological_2->setText(QString(c));
	}
}
void DBMSwithform::on_radioButton_new_clicked(bool)
{
	Category c = nameCategory[ui.comboBox_classInfological->currentText()];
	ui.lineEdit_codeInfological_1->setText(QString(c));
	ui.lineEdit_codeInfological_2->setText(QString(c));
}

InfologicalItem DBMSwithform::getInfologicalItem()
{
	InfologicalItem item;
	item.type = ui.comboBox_typeInfological->currentData().toChar();
	item.level = ui.lineEdit_levelInfological->text().toInt();
	item.c = nameCategory[ui.comboBox_classInfological->currentText()];
	item.name = ui.lineEdit_nameInfological->text();
	item.code_1 = ui.lineEdit_codeInfological_1->text();
	item.code_2 = ui.lineEdit_codeInfological_2->text();
	item.idCategory = ui.comboBox_categoryInfological->currentData().toInt();
	return item;
}


void DBMSwithform::on_pButton_GenerateCodeInfological_clicked()
{

	QString code;
	DbInfologicalModel db;
	InfologicalItem item = getInfologicalItem();

	if (item.level <= 0)
	{
		ui.label_infoInfological->setText(QStringLiteral("Неверный уровень сложности"));
		ui.label_infoInfological->setStyleSheet("color: rgb(50, 0, 70)");
		timerInfo->start(5000);
	}
	else
	{
		if (ui.radioButton_Category->isChecked())
		{
			code = db.generateCode(item.type, item.level, item.c, item.idCategory);
		}
		else
		{
			if (item.code_1.isEmpty())
			{
				ui.label_infoInfological->setText(QStringLiteral("Заполните код 1"));
				ui.label_infoInfological->setStyleSheet("color: rgb(50, 0, 70)");
				return;
			}
			if (item.code_2.isEmpty())
			{
				ui.label_infoInfological->setText(QStringLiteral("Заполните код 2"));
				ui.label_infoInfological->setStyleSheet("color: rgb(50, 0, 70)");
				return;
			}
			code = db.generateCode(item.type,item.level, item.code_1, item.code_2);
		}
	}
	ui.lineEdit_codeInfological->setText(code);

}

void DBMSwithform::showTableInfological(int indx)
{
	DbInfologicalModel db;
	if(m != NULL)
	{
		delete m;
		m = NULL;
	}
	if(indx >= 0)
	{
		m = db.getTable(nameCategory[ui.comboBox_tableInfological->itemText(indx)]);	
		ui.tableViewInfological->setModel(m);
		ui.tableViewInfological->hideColumn(0);
		QHeaderView *header = ui.tableViewInfological->horizontalHeader();
		if(header != NULL)
		{
			header->setSectionResizeMode(1,QHeaderView::ResizeToContents);
			header->setSectionResizeMode(2,QHeaderView::Stretch);
		}
		ui.tableViewInfological->show();
	}
}

void DBMSwithform::on_pButton_AddInfological_clicked()
{
	InfologicalItem item = getInfologicalItem();
	if (item.level <= 0)
	{
		ui.label_infoInfological->setText(QStringLiteral("Неверный уровень сложности"));
		ui.label_infoInfological->setStyleSheet("color: rgb(50, 0, 70)");
	}
	DbInfologicalModel db;
	int res;
	if(ui.radioButton_Category->isChecked())
	{
		res = db.insertRecord(typeUser, item.type, item.level, item.c, item.idCategory);
		if(res == -4)
		{
			ui.label_infoConceptual->setText(QStringLiteral("Категории не существует")); 
			ui.label_infoConceptual->setStyleSheet("color: rgb(50, 0, 70)");
		}
	}
	else
	{
		if(item.name == "")
		{
			ui.label_infoInfological->setText(QStringLiteral("Заполните наименование"));
			ui.label_infoInfological->setStyleSheet("color: rgb(50, 0, 70)");
			timerInfo->start(5000);
			return;
		}
		if (item.code_1.isEmpty())
		{
			ui.label_infoInfological->setText(QStringLiteral("Заполните код 1"));
			ui.label_infoInfological->setStyleSheet("color: rgb(50, 0, 70)");
			return;
		}
		if (item.code_2.isEmpty())
		{
			ui.label_infoInfological->setText(QStringLiteral("Заполните код 2"));
			ui.label_infoInfological->setStyleSheet("color: rgb(50, 0, 70)");
			return;
		}
		res = db.insertRecord(
			typeUser,
			item.type,
			item.level,
			item.c,
			item.code_1,
			item.code_2,
			item.name
			);

		if(res == -41) 
		{
			ui.label_infoInfological->setText(QStringLiteral("Недопустимый код 1")); 
			ui.label_infoInfological->setStyleSheet("color: rgb(50, 0, 70)");
		}
		if(res == -42)
		{
			ui.label_infoInfological->setText(QStringLiteral("Недопустимый код 2")); 
			ui.label_infoInfological->setStyleSheet("color: rgb(50, 0, 70)");
		}
	}
	switch (res)
	{
	case 0:
		{
			ui.label_infoInfological->setText(QStringLiteral("Запись добавлена"));
			ui.label_infoInfological->setStyleSheet("color: rgb(0, 50, 70)");
			break;
		}
	case -2:
		{
			ui.label_infoInfological->setText(QStringLiteral("Недопустимый тип")); 
			ui.label_infoInfological->setStyleSheet("color: rgb(50, 0, 70)");
			break;
		}
	case -3:
		{
			ui.label_infoInfological->setText(QStringLiteral("Недопустимый уровень")); 
			ui.label_infoInfological->setStyleSheet("color: rgb(50, 0, 70)");
			break;
		}
	case -6:
		{
			ui.label_infoInfological->setText(QStringLiteral("Запрос с таким кодом существует")); 
			ui.label_infoInfological->setStyleSheet("color: rgb(50, 0, 70)");
			break;
		}
	}
	timerInfo->start(5000);
}

void DBMSwithform::showFindTableInfological(QAbstractTableModel* mF)
{
	if(m != NULL)
	{
		delete m;
		m = NULL;
	}
	m = mF;
	ui.tableViewInfological->setModel(m);
	ui.tableViewInfological->hideColumn(0);
	QHeaderView *header = ui.tableViewInfological->horizontalHeader();
	if(header != NULL)
	{
		header->setSectionResizeMode(1,QHeaderView::ResizeToContents);
		header->setSectionResizeMode(2,QHeaderView::Stretch);
	}
	ui.tableViewInfological->show();
}

void DBMSwithform::on_pButton_SearchInfological_clicked()
{
	ui.comboBox_tableInfological->setCurrentIndex(-1);
	if (!findDialog) {
		findDialog = new SearchDialog(3, this);
		connect(findDialog, SIGNAL(finded(QAbstractTableModel*)), this, SLOT(showFindTableInfological(QAbstractTableModel*)));
	}
	findDialog->show();
	findDialog->raise();
	findDialog->activateWindow();
}

void DBMSwithform::showTableInfologicalQueries(bool insert)
{
	DbInfologicalModel db;
	if(m != NULL)
	{
		delete m;
		m = NULL;
	}
	if(insert)
	{
		m = db.getTableQueriesInsert();
	}
	else
	{
		m = db.getTableQueriesEdit();
	}
	ui.tableViewInfological_queries->setModel(m);
	// добавление кнопок к строкам если выбран эксперт
	if (typeUser == EXPERT)
	{
		int count = m->rowCount();
		m->insertColumns(m->columnCount(),2);
		m->setHeaderData(m->columnCount()-1, Qt::Horizontal, (""));
		m->setHeaderData(m->columnCount()-2, Qt::Horizontal, (""));
		while(--count + 1)
		{
			QPushButton *bConfirm = new QPushButton(QStringLiteral("Подтвердить"));
			bConfirm->setProperty("id", m->data(m->index(count, 0))); // запись id записи в кнопку
			connect( bConfirm, SIGNAL(clicked()), this, SLOT(bConfirm_clicked()));
			QPushButton *bDelete = new QPushButton(QStringLiteral("Удалить")); 
			connect( bDelete, SIGNAL(clicked()), this, SLOT(bDelete_clicked()));
			bDelete->setProperty("id", m->data(m->index(count, 0)));
			QModelIndex curIndx = m->index(count,m->columnCount()-2);
			delete ui.tableViewInfological_queries->indexWidget(curIndx);
			ui.tableViewInfological_queries->setIndexWidget(curIndx, bConfirm);
			curIndx = m->index(count,m->columnCount()-1);
			delete ui.tableViewInfological_queries->indexWidget(curIndx);
			ui.tableViewInfological_queries->setIndexWidget(curIndx, bDelete);
		}
	}
	ui.tableViewInfological_queries->hideColumn(0);
	QHeaderView *header = ui.tableViewInfological_queries->horizontalHeader();
	if(header != NULL)
	{
		
	}
	ui.tableViewInfological_queries->show();
}
