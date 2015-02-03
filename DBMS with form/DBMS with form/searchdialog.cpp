#include "searchdialog.h"
#include <QSizePolicy>

SearchDialog::SearchDialog(int m, QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);
	model = m;
	namesClass = DbElementsOfStructure::getSortClassNames();
	nameCategory = DbElementsOfStructure::getCategoryNames();
	switch (m)
	{
	case 1: initializeInitialModel(); break;
	case 2: initializeConceptualModel(); break;
	case 3: initializeInfologicalModel(); break;
	}
}

SearchDialog::~SearchDialog()
{
}
// инициировать нажатие кнопки Поиск
void SearchDialog::pushFind()
{
	on_pButton_find_clicked();
}

void SearchDialog::initializeInitialModel()
{
	ui.widget_class2->setVisible(false);
	ui.widgetClass->setVisible(false);
	ui.widget->setVisible(false);
	ui.widgetGroup->move(0,0);
	ui.groupBox_4->resize(ui.groupBox_4->size().width(), 68);
	this->resize(this->size().width(), 270);
	DbInitialModel db;
	ui.comboBox_group->addItem(QStringLiteral("не важно"));
	QList<QPair<QString, QVariant>> codesGroup = db.getGroupCodes();
	QPair<QString, QVariant> var;
	foreach(var, codesGroup)
	{
		ui.comboBox_group->addItem(var.first, var.second);
	}
}

void SearchDialog::initializeConceptualModel()
{
	ui.widget_type->setVisible(false);
	ui.listWidget_class->addItems(namesClass);
	ui.comboBox_class->addItems(namesClass);
	ui.groupBox_4->resize(ui.groupBox_4->size().width(), 290);
	this->resize(this->size().width(), 500);
	connect( ui.comboBox_class, SIGNAL(currentIndexChanged(int)), this, SLOT(changedClass(int)) );
	connect( ui.comboBox_group, SIGNAL(currentIndexChanged(int)), this, SLOT(changedCodeGroup(int)) );
}

void SearchDialog::initializeInfologicalModel()
{
	DbInfologicalModel db;
	ui.listWidget_class->addItems(namesClass);
	ui.comboBox_class->addItems(namesClass);
	connect( ui.comboBox_class, SIGNAL(currentIndexChanged(int)), this, SLOT(changedClass(int)) );
	connect( ui.comboBox_group, SIGNAL(currentIndexChanged(int)), this, SLOT(changedCodeGroup(int)) );
	QMap<QString, QChar> types = db.getTypes();
	ui.listWidget_type->clear();
	foreach(QString var, types.keys())
	{
		QListWidgetItem* a = new QListWidgetItem;
		a->setData(Qt::UserRole, types[var]);
		a->setText(var);
		ui.listWidget_type->addItem(a);
	}
}

void SearchDialog::on_pButton_find_clicked()
{
	switch (model)
	{
	case 1: findInitial(); break;
	case 2: findConceptual(); break;
	case 3: findInfological(); break;
	}
}

Item SearchDialog::getItem()
{
	Item item;
	item.text = ui.lineEdit_text->text();
	item.c = nameCategory[ui.comboBox_class->currentText()];
	if (ui.radioButton_fullSearch->isChecked())
	{
		item.t = FULL;
	}
	else if(ui.radioButton_partSearch->isChecked())
	{
		item.t = PARTIAL;
	}
	else if(ui.radioButton_wildcard->isChecked())
	{
		item.t = TEMPLATE;
	}
	item.codeGroup = -1;
	item.codeInGroup = -1;
	if(ui.comboBox_group->currentIndex() != 0)
	{
		item.codeGroup = ui.comboBox_group->currentData();
	}
	if(ui.comboBox_codeInGroup->currentIndex() != 0)
	{
		item.codeInGroup = ui.comboBox_codeInGroup->currentData();
	}
	foreach (QListWidgetItem* var, ui.listWidget_class->selectedItems())
	{
		item.listClass.append(nameCategory[var->text()]);
	}
	foreach (QListWidgetItem* var, ui.listWidget_type->selectedItems())
	{
		item.listType.append(var->data(Qt::UserRole).toChar());
	}
	item.level = -1;
	if(!ui.lineEdit_level->text().isEmpty())
	{
		item.level = ui.lineEdit_level->text().toInt();
	}
	return item;
}

void SearchDialog::findInitial()
{
	DbInitialModel db;
	QAbstractTableModel *find = NULL;
	Item item = getItem();
	if(ui.radioButton_name->isChecked())
	{
		// поиск по наименованию
		
		if(ui.comboBox_group->currentIndex() == 0)
		{
			find = db.findText(item.text,item.t);
		}
		else
		{
			find = db.findText(item.text, item.t, item.codeGroup.toInt());
		}
	}
	else
	{
		// поиск по коду
		find = db.findCode(item.text, item.t);
	}

	emit finded(find);
}

void SearchDialog::findConceptual()
{
	DbConceptualModel db;
	QAbstractTableModel *find = NULL;
	Item item = getItem();
	if(ui.radioButton_name->isChecked())
	{
		// поиск по наименованию
		if(ui.radioButton_class1->isChecked())
		{
			// по классификаторам
			find = db.findText(item.text, item.t, item.c, item.codeGroup.toInt(), item.codeInGroup.toInt());		
		}
		else
		{
			// по нескольким классам
			find = db.findText(item.text, item.t, item.listClass);
			
		}
	}
	else
	{
		// поиск по коду
		find = db.findCode(item.text, item.t);
	}

	emit finded(find);
}

void SearchDialog::findInfological()
{
	DbInfologicalModel db;
	QAbstractTableModel *find = NULL;
	Item item = getItem();
	if(ui.radioButton_name->isChecked())
	{
		// поиск по наименованию
		if(ui.radioButton_class1->isChecked())
		{
			// по классификаторам
			find = db.findText(item.text, item.t, item.c, item.listType, item.codeGroup.toString(), item.codeInGroup.toString(), item.level);	
		}
		else
		{
			// по нескольким классам
			find = db.findText(item.text, item.t, item.listClass, item.listType, item.level);
		}
	}
	else
	{
		// поиск по коду
		find = db.findCode(item.text, item.t);
	}

	emit finded(find);
}

void SearchDialog::changedClass(int indx)
{
	ui.comboBox_group->clear();
	Category c = nameCategory[ui.comboBox_class->itemText(indx)];
	DbElementsOfStructure* db;
	if(model == 2)
	{
		db = new DbConceptualModel();
	}
	if(model == 3)
	{
		db = new DbInfologicalModel();
	}
	ui.comboBox_group->addItem(QStringLiteral("не важно"));
	QList<QPair<QString, QVariant>> codesGroup = db->getGroupCodes(c);
	QPair<QString, QVariant> var;
	foreach(var, codesGroup)	
	{
		ui.comboBox_group->addItem(var.first, var.second);
	}
	delete db;
}

void SearchDialog::changedCodeGroup(int indx)
{
	ui.comboBox_codeInGroup->clear();
	Category c = nameCategory[ui.comboBox_class->currentText()];
	DbElementsOfStructure* db;
	if(model == 2)
	{
		db = new DbConceptualModel();
	}
	if(model == 3)
	{
		db = new DbInfologicalModel();
	}
	ui.comboBox_codeInGroup->addItem(QStringLiteral("не важно"));
	QList<QVariant> codes = db->getInGroupCodes(ui.comboBox_group->currentData(), c);
	foreach(QVariant var, codes)	
	{
		ui.comboBox_codeInGroup->addItem(var.toString(), var);
	}
	delete db;
}