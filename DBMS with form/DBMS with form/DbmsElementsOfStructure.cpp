#include "DbmsElementsOfStructure.h"

QSqlDatabase DbElementsOfStructure::_sdb = QSqlDatabase::addDatabase("QSQLITE");
QMap<Category, QString> DbElementsOfStructure::nameTable;
DbElementsOfStructure::DbElementsOfStructure()
{
	nameTable.clear();
	if(nameTable.isEmpty())
	{
		nameTable.insert(CYCLES, "Cycles");
		nameTable.insert(PROCESSES, "Processes");
		nameTable.insert(TASKS, "Tasks");
		nameTable.insert(COMPONENTS, "Components");
		nameTable.insert(OBJECTS, "Objects");
		nameTable.insert(ATTRIBUTES, "Attributes");
		nameTable.insert(VALUES, "NumValues");
	}
}

int DbElementsOfStructure::connect(QString path)
{
	if(!QFile::exists(path))
	{
		return -1;
	}

	_sdb.setDatabaseName(path);
	if (!_sdb.open())
	{
		return -2;
	}
	return 0;
}

void DbElementsOfStructure::disconnect()
{
	_sdb.close();
}


DbElementsOfStructure::~DbElementsOfStructure()
{
}

bool DbElementsOfStructure::isConnect()
{
	if (!_sdb.open())
	{
		return false;
	}
	return true;
}
// наименования классов в правильном порядке
QStringList DbElementsOfStructure::getSortClassNames()
{
	QStringList namesClass;
	namesClass.push_back(QStringLiteral("Цикл"));
	namesClass.push_back(QStringLiteral("Процесс"));
	namesClass.push_back(QStringLiteral("Задача"));
	namesClass.push_back(QStringLiteral("Компонент"));
	namesClass.push_back(QStringLiteral("Объект"));
	namesClass.push_back(QStringLiteral("Признак"));
	namesClass.push_back(QStringLiteral("Значение"));
	return namesClass;
}
// набор соответствий наимнований классов к категориям
QMap<QString, Category> DbElementsOfStructure::getCategoryNames()
{
	QStringList namesClass = getSortClassNames();
	QMap<QString, Category> nameCat;
	nameCat.insert(namesClass[0], CYCLES);
	nameCat.insert(namesClass[1], PROCESSES);
	nameCat.insert(namesClass[2], TASKS);
	nameCat.insert(namesClass[3], COMPONENTS);
	nameCat.insert(namesClass[4], OBJECTS);
	nameCat.insert(namesClass[5], ATTRIBUTES);
	nameCat.insert(namesClass[6], VALUES);
	return nameCat;
}

// получить таблицу типов формат возвращаемых данных: code[QChar], name[string]
QMap<QString, QChar> DbElementsOfStructure::getTypes()
{
	QMap<QString, QChar> result;
	if(!isConnect())
	{
		return result;
	}
	QSqlQuery types;
	QString str = "SELECT code || ' - ' || name, code FROM Types";
	if(!types.exec(str))
	{
		result.insert(types.lastError().text(), -1);
	}
	while(types.next())
	{
		result.insert(types.value(0).toString(), types.value(1).toString().at(0));
	}
	return result;
}

// получить справочник классификатора категорий, формат возвращаемых данных: полное наименование[QString], код[int]
QList<QPair<QString, int>> DbElementsOfStructure::getClassifierCodeGroup(Category table)
{
	QList<QPair<QString, int>> result;
	if(!isConnect())
	{
		return result;
	}
	// корректировка класса или запроса
	if(table == OBJECTS)
	{
		return getClassifierCodeInGroupComponent();
	}
	if(table == COMPONENTS)
	{
		table = PROCESSES;
	}
	if(table == VALUES)
	{
		table = ATTRIBUTES;
	}
	QSqlQuery codes;
	QString str = "SELECT code, code || '-' || name FROM ClassifierCategories WHERE class = '" + QString(table) + 
		"' ORDER BY code";
	if (table == TASKS)
	{
		str = "SELECT DISTINCT code_in_group, code_in_group FROM Processes WHERE code_in_group BETWEEN 0 AND 99 AND confirm = " + QString::number(EXPERT) +
			" ORDER BY code_in_group";
	}
	if(!codes.exec(str))
	{
		result.push_back(QPair<QString, int>(codes.lastError().text(),-1));
	}
	while(codes.next())
	{
		result.push_back(QPair<QString, int>(codes.value(1).toString(), codes.value(0).toInt()));
	}
	return result;
}

// получить классификацию кодов в группе для категории "Компоненты", формат возвращаемых данных: полное наименование[QString], код[int]
QList<QPair<QString, int>> DbElementsOfStructure::getClassifierCodeInGroupComponent()
{
	QList<QPair<QString, int>>  result;
	if(!isConnect())
	{
		return result;
	}

	QSqlQuery codes;
	QString str = "SELECT code, code || '-' || name FROM ClassifierCategories WHERE class = '" 
		+ QString(COMPONENTS) + "' ORDER BY code";
	if(!codes.exec(str))
	{
		result.push_back(QPair<QString, int>(codes.lastError().text(),-1));
	}
	while(codes.next())
	{
		result.push_back(QPair<QString, int>(codes.value(1).toString(), codes.value(0).toInt()));
	}
	return result;
}

// Получить последниый допустимый код в группе (недопустимо для COMPONENTS) 
int DbElementsOfStructure::getValidCodeInGroupe(int codeGroup, Category table)
{
	if(!isConnect())
	{
		return -1;
	}
	if(table == COMPONENTS)
	{
		return -1;
	}
	QSqlQuery maxCode;
	maxCode.exec("SELECT max(code_in_group) FROM " + nameTable[table] + 
		" WHERE code_group = " + QString::number(codeGroup) +
		" AND confirm = " + QString::number(EXPERT));
	if(!maxCode.next())
	{
		return 1;// первый код
	}
	return maxCode.value(0).toInt() + 1;
}

int DbElementsOfStructure::deleteRecord(QString id, Category table )
{
	if(!isConnect())
	{
		return -1;
	}
	QChar c = id[0];
	QList<Category> tablesForFind = nameTable.keys();
	QSqlQueryModel *model = new QSqlQueryModel();
	// поиск к какому классу(таблице) принадлежит код
	foreach(Category var, tablesForFind)
	{
		if (c == var)
		{
			table = var;
			id = id.mid(1);
			break;
		}
	}
	QSqlQuery query;
	QString str = "DELETE FROM " + nameTable[table] + " WHERE id = " + id;
	if(!query.exec(str))
	{
		QString er = query.lastError().text();
		return -2;
	}
	return 0;
}

//Редактирование, id - id редактируемой записи
int DbElementsOfStructure::editRecord(UserType user, QString id, QString name, Category table)
{
	if(!isConnect())
	{
		return -1;
	}
	QChar c = id[0];
	QList<Category> tablesForFind = nameTable.keys();
	QSqlQueryModel *model = new QSqlQueryModel();
	// поиск к какому классу(таблице) принадлежит код
	foreach(Category var, tablesForFind)
	{
		if (c == var)
		{
			table = var;
			id = id.mid(1);
			break;
		}
	}
	QSqlQuery query;
	QString str;
	QString err = query.lastError().text();
	name = name.toLower().simplified();
	if(user == USER)
	{
		str = "INSERT INTO " + nameTable[table] + " (name, confirm, type, level, code_group, code_in_group, extra_code)" 
			" SELECT '" + name + "', " + (QString::number(USER + 1 + id.toInt())) + ", type, level, code_group, code_in_group, extra_code FROM " +
			nameTable[table] + " WHERE id = " + id;
	}
	else
	{
		str = "UPDATE " + nameTable[table] + " SET name = '" + name + "' WHERE id = " + id;
	}
	if(!query.exec(str))
	{
		QString er = query.lastError().text();
		return -3;
	}
	return 0;
}

// подтвердить запрос пользователя
int DbElementsOfStructure::confirmQuery(QString id, Category table)
{
	if(!isConnect())
	{
		return -1;
	}
	QSqlQuery query;
	QString str;
	QChar c = id[0];
	QList<Category> tablesForFind = nameTable.keys();
	QSqlQueryModel *model = new QSqlQueryModel();
	// поиск к какому классу(таблице) принадлежит код
	foreach(Category var, tablesForFind)
	{
		if (c == var)
		{
			table = var;
			id = id.mid(1);
			break;
		}
	}
	str = "SELECT confirm, name FROM " + nameTable[table] + " WHERE id = " + id;
	QString er;
	if(!query.exec(str))
	{
		er = query.lastError().text();
		return -2;
	}
	query.next();
	int conf = query.value(0).toInt();
	if(conf == USER)
	{
		// подвердтить добавление
		str = "UPDATE " + nameTable[table] + " SET confirm = " + QString::number(EXPERT) +
			" WHERE id = " + id;
	}
	if (conf > USER)
	{
		// подвердить редактирование
		int idEdit = conf - USER - 1;
		str = "UPDATE " + nameTable[table] + " SET name = '" + query.value(1).toString() + 
			"' WHERE id = " + QString::number(idEdit);
		deleteRecord(id, table);
	}
	if(!query.exec(str))
	{
		er = query.lastError().text();
		return -3;
	}
	return 0;
}

// НАЧАЛЬНЫЙ УРОВЕНЬ

DbInitialModel::DbInitialModel() : DbElementsOfStructure(){}

// получить таблицу параметров начального уровня
// формат возвращаемых данных getTable: id[string], code[string], name[string]
QAbstractTableModel* DbInitialModel::getTable(Category table)
{
	if(!isConnect())
	{
		return NULL;
	}
	QSqlQueryModel *model = new QSqlQueryModel();
	model->setQuery("SELECT '" + QString(ATTRIBUTES) + "'||id," 
		"('A' || code_group || '.' || code_in_group) AS code,"
		"name"
		" FROM " + nameTable[ATTRIBUTES] +
		" WHERE confirm = " + QString::number(EXPERT) + " AND extra_code = ''" + " AND code_group BETWEEN 0 AND 99");
	QString er = model->lastError().text();
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));	
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Наименование"));
	return model;
}


QAbstractTableModel* DbInitialModel::getTableQueriesEdit()
{
	if(!isConnect())
	{
		return NULL;
	}
	QSqlQueryModel *model = new QSqlQueryModel();
	QString str = "SELECT '" + QString(ATTRIBUTES) + "'||id,"  
		"('A' || code_group || '.' || code_in_group) AS code,"
		"name"  ", (SELECT name FROM " + nameTable[ATTRIBUTES] + " WHERE id = b.confirm - 1 - " + QString::number(USER) + ")"
		" FROM " + nameTable[ATTRIBUTES] + " AS b"
		" WHERE confirm > " + QString::number(USER) + " AND extra_code = ''" + " AND code_group BETWEEN 0 AND 99";
	model->setQuery(str);
	QString er = model->lastError().text();
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));	
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Новое наименование"));
	model->setHeaderData(3, Qt::Horizontal, QStringLiteral("Старое наименование"));
	return model;
}
QAbstractTableModel* DbInitialModel::getTableQueriesInsert()
{
	if(!isConnect())
	{
		return NULL;
	}
	QSqlQueryModel *model = new QSqlQueryModel();
	model->setQuery("SELECT '" + QString(ATTRIBUTES) + "'||id," 
		"('A' || code_group || '.' || code_in_group) AS code,"
		"name"
		" FROM " + nameTable[ATTRIBUTES] +
		" WHERE confirm = " + QString::number(USER) + " AND extra_code = ''" + " AND code_group BETWEEN 0 AND 99");
	QString er = model->lastError().text();
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));	
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Наименование"));
	return model;
}

// Генерирование кодировки. Если кодировка не допустима, возвращается пустая строка
QString DbInitialModel::generateCode(int codeGroup, int codeInGroup)
{
	if(!isConnect())
	{
		return NULL;
	}
	QSqlQuery codes;
	// отбираем допустимые коды группы
	codes.exec("SELECT code FROM ClassifierCategories WHERE class = 'R' AND code = '" + QString::number(codeGroup) + "'");
	if(!codes.next())
	{
		return QStringLiteral("Неверный код группы");
	}
	if(codeInGroup <= 0)
	{
		return QStringLiteral("Неверный код в группе");
	}
	codes.exec("SELECT code_in_group, code_group FROM " + nameTable[ATTRIBUTES] 
	+ " WHERE code_in_group = '" + QString::number(codeInGroup) + "' AND code_group = '" + QString::number(codeGroup) + 
		"' AND confirm = " + QString::number(EXPERT));
	if(codes.next())
	{
		return QStringLiteral("Такой код существует в таблице");
	}
	return "A" + QString::number(codeGroup) + "." + QString::number(codeInGroup);
}

// вставить параметр начального уровня
int DbInitialModel::insertRecord(UserType user, int codeGroup, int codeInGroup, QString name)
{
	if(!isConnect())
	{
		return -1;
	}

	QSqlQuery query;
	query.exec("SELECT code_in_group, code_group FROM " + nameTable[ATTRIBUTES] + 
		" WHERE code_in_group = " + QString::number(codeInGroup) + 
		" AND code_group = " + QString::number(codeGroup) + 
		" AND confirm = " + QString::number(EXPERT));
	QString err = query.lastError().text();
	if(query.next())
	{
		// запись с таким кодом существует
		return -2;
	}
	name = name.toLower().simplified();
	QString str = "INSERT INTO " + nameTable[ATTRIBUTES] + " (code_group, code_in_group, name, confirm, extra_code) VALUES(" + 
		QString::number(codeGroup) + ", " + 
		QString::number(codeInGroup) + ", '" + 
		name + "', " + QString::number(user) + ",'')";
	if(!query.exec(str))
	{
		QString er = query.lastError().text();
		return -3;
	}
	return 0;
}
// получить коды групп созданных единиц
QList<QPair<QString, QVariant>> DbInitialModel::getGroupCodes(Category table)
{
	QList<QPair<QString, QVariant>> result;
	if(!isConnect())
	{
		return result;
	}
	QList<QPair<QString, int>> groupCodes = getClassifierCodeGroup(ATTRIBUTES);
	QString str;
	QSqlQuery codes;
	str = "SELECT DISTINCT code_group FROM " + nameTable[ATTRIBUTES] + 
		" WHERE code_group BETWEEN 0 AND 99 AND extra_code = '' "
		" AND confirm = " + QString::number(EXPERT);
	codes.exec(str);
	if(!codes.exec(str))
	{
		QString er = codes.lastError().text();
	}

	while(codes.next())
	{
		QPair<QString, int> var;
		foreach (var, groupCodes)
		{
			
			if(var.second == codes.value(0))
			{
				result.push_back(var);
			}
		}
	}
	return result;
}


// получить коды в группе созданных единиц
QList<QVariant> DbInitialModel::getInGroupCodes(QVariant codeGroup, Category table)
{
	QList<QVariant> resault;
	if(!isConnect())
	{
		return resault;
	} 
	QString str;
	QSqlQuery codes;
	str = "SELECT DISTINCT code_in_group FROM " + nameTable[ATTRIBUTES] + 
		" WHERE code_group BETWEEN 0 AND 99 AND  extra_code = '' AND code_group = " + codeGroup.toString() +
		" AND confirm = " + QString::number(EXPERT) + " ORDER BY code_in_group";
	codes.exec(str);
	if(!codes.exec(str))
	{
		QString er = codes.lastError().text();
	}

	while(codes.next())
	{
		resault.push_back(codes.value(0).toInt());
	}
	return resault;
}

// Поиск по тексту с выбранным типом поиска (st) 
QAbstractTableModel* DbInitialModel::findText(QString string, SearchType st)
{
	if(!isConnect())
	{
		return NULL;
	}
	QSqlQueryModel *model = new QSqlQueryModel();
	QString str;
	string = string.toLower().simplified();
	str = "SELECT '" + QString(ATTRIBUTES) + "'||id," 
		"('A' || code_group || '.' || code_in_group) AS code,"
		"name"
		" FROM " + nameTable[ATTRIBUTES] +
		" WHERE confirm = " + QString::number(EXPERT) + " AND extra_code = ''";
	if (st == FULL)
	{
		str += " AND (name = '" + string + "')";

	} 
	else
	{
		if(st == PARTIAL)
		{
			string = "%" + string + "%";
		}
		str += " AND (name LIKE '" + string + "')";
	}
	model->setQuery(str);
	QString er = model->lastError().text();
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));	
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Наименование"));
	model->setHeaderData(3, Qt::Horizontal, QStringLiteral("Единица измерения"));
	return model;

}

// Поиск с выбранным типом поиска (st) по классификатору  
QAbstractTableModel* DbInitialModel::findText(QString string, SearchType st, int code)
{
	if(!isConnect())
	{
		return NULL;
	}
	QSqlQueryModel *model = new QSqlQueryModel();
	QString str;
	string = string.toLower().simplified();
	str = "SELECT '" + QString(ATTRIBUTES) + "'||id," 
		"('A' || code_group || '.' || code_in_group) AS code,"
		"name"
		" FROM " + nameTable[ATTRIBUTES] +
		" WHERE confirm = " + QString::number(EXPERT) + " AND extra_code = '' AND code_group = " + QString::number(code);
	if (st == FULL)
	{
		str += " AND (name = '" + string + "')";

	} 
	else
	{
		if(st == PARTIAL)
		{
			string = "%" + string + "%";
		}
		str += " AND (name LIKE '" + string + "')";
	}
	model->setQuery(str);
	QString er = model->lastError().text();
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));	
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Наименование"));
	return model;
}
// Поиск по коду
QAbstractTableModel* DbInitialModel::findCode(QString code,  SearchType st)
{
	if(!isConnect())
	{
		return NULL;
	}
	QSqlQueryModel *model = new QSqlQueryModel();
	QString str;
	code = code.toUpper().simplified();
	str = "SELECT '" + QString(ATTRIBUTES) + "'||id," 
		"('A' || code_group || '.' || code_in_group) AS code,"
		"name"
		" FROM " + nameTable[ATTRIBUTES] +
		" WHERE confirm = " + QString::number(EXPERT) + " AND extra_code = ''";
	if (st == FULL)
	{
		str += " AND (code = '" + code + "')";

	} 
	else
	{
		if(st == PARTIAL)
		{
			code = "%" + code + "%";
		}
		str += " AND (code LIKE '" + code + "')";
	}
	model->setQuery(str);
	QString er = model->lastError().text();
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));	
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Наименование"));
	return model;
}


// КОНЦЕПТУАЛЬНЫЙ УРОВЕНЬ

DbConceptualModel::DbConceptualModel() : DbElementsOfStructure(){}

// получить таблицу параметров концептуального уровня
// формат возвращаемых данных getTable: id[string], code[string], name[string]
QAbstractTableModel* DbConceptualModel::getTable(Category table)
{
	if(!isConnect())
	{
		return NULL;
	}
	QSqlQueryModel *model = new QSqlQueryModel();
	QString str = "SELECT '" + QString(table) + "'||id," 
		"('" + QString(table) + "' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '') ) AS code,"
		"name " +
		" FROM " + nameTable[table] +
		" WHERE confirm = " + QString::number(EXPERT) + " AND code_group BETWEEN 0 AND 99 GROUP BY code";
	model->setQuery(str);
	QString er = model->lastError().text();
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));	
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Наименование"));
	if (table == VALUES)
	{
		model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Единица измерения"));
	}
	return model;
}

// получить таблицу с запросами на изменения данных от обычных пользователей
QAbstractTableModel* DbConceptualModel::getTableQueriesEdit()
{
	if(!isConnect())
	{
		return NULL;
	}
	QList<Category> tables= nameTable.keys();
	QString str;
	QSqlQueryModel *model = new QSqlQueryModel();
	// поиск к какому классу(таблице) принадлежит код
	int i = 0;
	foreach(Category var, tables)
	{
		i++;
		str += "SELECT '" + QString(var) + "'||id,"  
			"('" + QString(var) + "' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '') ) AS code," +
			"name, (SELECT name FROM " + nameTable[var] + " WHERE id = b.confirm - 1 - " + QString::number(USER) + ")"
			" FROM " + nameTable[var] + " AS b"
			" WHERE confirm > " + QString::number(USER) + " AND code_group BETWEEN 0 AND 99";
		if (i < tables.count())
		{
			str += " UNION ";
		} 		
	}
	
	model->setQuery(str);
	QString er = model->lastError().text();
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));	
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Новое наименование"));
	model->setHeaderData(3, Qt::Horizontal, QStringLiteral("Старое наименование"));
	return model;
}
QAbstractTableModel* DbConceptualModel::getTableQueriesInsert()
{
	if(!isConnect())
	{
		return NULL;
	}
	QSqlQueryModel *model = new QSqlQueryModel();
	QString str;
	int i = 0;
	QList<Category> tables= nameTable.keys();
	foreach(Category var, tables)
	{
		i++;
		str += "SELECT '" + QString(var) + "'||id,"  
			"('" + QString(var) + "' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '') ) AS code," 
		"name"
		" FROM " + nameTable[var] +
		" WHERE confirm = " + QString::number(USER) + " AND code_group BETWEEN 0 AND 99";
		if (i < tables.count())
		{
			str += " UNION ";
		} 
	}
	model->setQuery(str);
	QString er = model->lastError().text();
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));	
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Наименование"));
	return model;
}

QMap<QString, int> DbConceptualModel::getParametrs()
{
	QMap<QString, int> result;
	if(!isConnect())
	{
		return result;
	}
	QString str;
	QSqlQuery query;
	str = "SELECT 'A' || code_group || '.' || code_in_group || ' ' || name, id"
		" FROM " + nameTable[ATTRIBUTES] +
		" WHERE confirm = " + QString::number(EXPERT) + " AND code_group BETWEEN 0 AND 99 AND extra_code = ''";
	if(!query.exec(str))
	{
		result.insert(query.lastError().text(), -1);
	}
	while(query.next())
	{
		result.insert(query.value(0).toString(), query.value(1).toInt());
	}
	return result;
}

// Получить последний допустимый дополнительный код 
int DbConceptualModel::getValidExtraCode(Category table, int codeGroup, int codeInGroup)
{
	if(!isConnect())
	{
		return 0;
	}
	QSqlQuery maxCode;
	maxCode.exec("SELECT max(extra_code) FROM " + nameTable[table] + 
		" WHERE code_group = " + QString::number(codeGroup) +
		" AND code_in_group = " + QString::number(codeInGroup) +
		" AND confirm = " + QString::number(EXPERT));
	if(!maxCode.next())
	{
		return 1; // первый код
	}
	return maxCode.value(0).toInt() + 1;
}

int DbConceptualModel::getValidExtraCode(int idParametr)
{
	if(!isConnect())
	{
		return 0;
	}
	QSqlQuery maxCode;
	QString str;
	str = "SELECT max(a.extra_code) FROM "+ nameTable[ATTRIBUTES] + " AS a, " + nameTable[ATTRIBUTES] + " AS b"		
		" WHERE a.code_group = b.code_group AND a.code_in_group = b.code_in_group AND b.id = " + QString::number(idParametr) +
		" AND a.extra_code <> '' AND a.confirm = " + QString::number(EXPERT) ;
	maxCode.exec(str);
	QString er = maxCode.lastError().text();
	if(!maxCode.next())
	{
		return 1; // первая запись
	}
	int y = maxCode.value(0).toInt();
	return maxCode.value(0).toInt() + 1;
}

// получить коды групп созданных единиц
QList<QPair<QString, QVariant>> DbConceptualModel::getGroupCodes(Category table)
{
	QList<QPair<QString, QVariant>> result;
	if(!isConnect())
	{
		return result;
	}
	QList<QPair<QString, int>> groupCodes = getClassifierCodeGroup(table);
	QString str;
	QSqlQuery codes;
	str = "SELECT DISTINCT code_group FROM " + nameTable[table] + 
		" WHERE code_group BETWEEN 0 AND 99 "
		" AND confirm = " + QString::number(EXPERT);
	codes.exec(str);
	if(!codes.exec(str))
	{
		QString er = codes.lastError().text();
	}

	while(codes.next())
	{
		QPair<QString, int> var;
		foreach (var, groupCodes)
		{
			if(var.second == codes.value(0))
			{
				result.push_back(var);
			}
		}
	}
	return result;
}
// получить коды в группе созданных единиц
QList<QVariant> DbConceptualModel::getInGroupCodes(QVariant codeGroup, Category table)
{
	QList<QVariant> resault;
	if(!isConnect())
	{
		return resault;
	} 
	QString str;
	QSqlQuery codes;
	str = "SELECT DISTINCT code_in_group FROM " + nameTable[table] + 
		" WHERE code_group BETWEEN 0 AND 99 AND code_group = " + codeGroup.toString() +
		" AND confirm = " + QString::number(EXPERT) + " ORDER BY code_in_group";
	codes.exec(str);
	if(!codes.exec(str))
	{
		QString er = codes.lastError().text();
	}

	while(codes.next())
	{
		resault.push_back(codes.value(0).toInt());
	}
	return resault;
}

// Проверить данные на правильность
int DbConceptualModel::checkData(Category table, int codeGroup, int codeInGroup, int extraCode, QString name)
{

	QList<QPair<QString, int>> listCodes = getClassifierCodeGroup(table);
	QList<int> codes;
	QPair<QString, int> var;
	foreach (var, listCodes)
	{
		codes.push_back(var.second);
	}
	if(!codes.contains(codeGroup))
	{
		return 1; // codeGroup недопустимый
	}
	if(table == COMPONENTS)
	{
		listCodes = getClassifierCodeInGroupComponent();
		foreach (var, listCodes)
		{
			codes.push_back(var.second);
		}
		if(!codes.contains(codeInGroup))
		{
			return 2; //  codeInGroup недопустимый
		}
	}
	if(extraCode < 0)
	{
		return 3; // extraCode недопустиый
	}
	if(name.isEmpty())
	{
		return 4; // name пустое
	}
	return 0;
}

// Генерирование кодировки. Если кодировка не допустима, возвращается пустая строка
QString DbConceptualModel::generateCode(Category table, int codeGroup, int codeInGroup, int extraCode)
{
	if(!isConnect())
	{
		return NULL;
	}
	QSqlQuery query;
	// проверка на допустимость кода 
	int err = checkData(table, codeGroup, codeInGroup, extraCode, "name");
	switch (err)
	{
	case 0:
		break;
	case 1: 
		return QStringLiteral("Неверный код группы");
	case 2:
		return QStringLiteral("Неверный код в группе");
	case 3:
		return QStringLiteral("Неверный дополнительный код");
	}
	QString ec = QString::number(extraCode);
	if(extraCode == 0)
	{
		// без дополнительного кода
		ec = "";
	}
	query.exec("SELECT code_in_group, code_group, extra_code FROM " + nameTable[table] +
		" WHERE code_in_group = '" + QString::number(codeInGroup) + 
		"' AND code_group = '" + QString::number(codeGroup) + 
		"' AND extra_code = '" + ec + 
		"' AND confirm = " + QString::number(EXPERT));
	if(query.next())
	{
		return QStringLiteral("Такой код существует в таблице");
	}
	ec = "." + ec;
	return QString(table) + QString::number(codeGroup) + "." + QString::number(codeInGroup) + ec;
}

QString DbConceptualModel::generateCode(int idParametr, int extraCode)
{
	if(!isConnect())
	{
		return NULL;
	}
	QSqlQuery query;
	QString str;
	str = "SELECT code_group || '.' || code_in_group FROM "+ nameTable[ATTRIBUTES] + " WHERE id = " + QString::number(idParametr) +
		" AND confirm = " + QString::number(EXPERT);
	query.exec(str);
	if(!query.next())
	{
		return QStringLiteral("Параметра с данным id не существует");
	}
	if(extraCode <= 0)
	{
		return QStringLiteral("Неверный дополнительный код");
	}
	return QString(ATTRIBUTES) + query.value(0).toString() + "." + QString::number(extraCode);

}

// вставить параметр концептуального уровня
int DbConceptualModel::insertRecord(UserType user, Category table, int codeGroup, int codeInGroup, int extraCode, QString name)
{
	if(!isConnect())
	{
		return -1;
	}
	// проверка на допустимость кода 
	int err = checkData(table, codeGroup, codeInGroup, extraCode, name);
	switch (err)
	{
	case 0:
		break;
	case 1: 
		return -2;
	case 2:
		return -3;
	case 3:
		return -4;
	case 4:
		return -5;
	}
	QString ec = QString::number(extraCode);
	if(extraCode == 0)
	{
		// без дополнительного кода
		ec = "";
	}
	QSqlQuery query;
	QString str;
	str = "SELECT code_group, code_in_group, extra_code FROM " + nameTable[table] +
		" WHERE code_in_group = '" + QString::number(codeInGroup) + 
		"' AND code_group = '" + QString::number(codeGroup) + 
		"' AND extra_code = '" + ec + 
		"' AND confirm = " + QString::number(EXPERT);
	query.exec(str);
	if(query.next())
	{
		// запись с таким кодом существует
		return -6;
	}
	name = name.toLower().simplified();
	str = "INSERT INTO " + nameTable[table] + " (code_group, code_in_group, extra_code, name, confirm) VALUES(" + 
		QString::number(codeGroup) + ", " + 
		QString::number(codeInGroup) + ", '" + 
		ec + "', '" + 
		name + "', " + QString::number(user) + ")";
	if(!query.exec(str))
	{
		QString er = query.lastError().text();
		return -7;
	}
	return 0;
}

int DbConceptualModel::insertRecord(UserType user, int idParametr, int extraCode, QString name)
{
	if(!isConnect())
	{
		return -1;
	}
	QSqlQuery query;
	QSqlQuery record;
	QString str;
	if(extraCode <= 0)
	{
		return -4;
	}
	str = "SELECT code_group, code_in_group, name FROM "+ nameTable[ATTRIBUTES] + " WHERE id = " + QString::number(idParametr) +
		" AND confirm = " + QString::number(EXPERT);
	record.exec(str);
	QString err = record.lastError().text();
	if(!record.next())
	{
		// параметра с данным id не существует
		return -2;
	}
	str = "SELECT code_in_group, code_group, extra_code FROM " + nameTable[ATTRIBUTES] +
		" WHERE code_group = '" + record.value(0).toString() + 
		"' AND code_in_group = '" + record.value(1).toString() + 
		"' AND extra_code = '" + QString::number(extraCode) + 
		"' AND confirm = " + QString::number(EXPERT);
	query.exec(str);
	if(query.next())
	{
		// запись с таким кодом существует
		return -6;
	}
	name = name.toLower().simplified();
	if(name.isEmpty())
	{
		name = record.value(2).toString();
	}
	str = "INSERT INTO " + nameTable[ATTRIBUTES] + " (code_group, code_in_group, extra_code, name, confirm) VALUES(" +
		record.value(0).toString() + ", " + 
		record.value(1).toString() + ", " + 
		QString::number(extraCode) + ", '" + 
		name + "', " + 
		QString::number(user) + ")";
	if(!query.exec(str))
	{
		QString er = query.lastError().text();
		return -7;
	}
	return 0;
}

// Поиск с выбранным типом поиска (st) по категориям 
QAbstractTableModel* DbConceptualModel::findText(QString string, SearchType st, QList<Category> categories)
{
	if(!isConnect() || categories.isEmpty())
	{
		return NULL;
	}
	QSqlQueryModel *model = new QSqlQueryModel();
	QString str = "";
	string = string.toLower().simplified();
	int i = 0;
	foreach(Category var, categories)
	{
		i++;
		QString strT = string;
		str += "SELECT '" + QString(var) + "'||id," 
			"('" + QString(var) + "' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '') ) AS code," +
			"name " +
			" FROM " + nameTable[var] +
			" WHERE confirm = " + QString::number(EXPERT) + " AND code_group BETWEEN 0 AND 99";
		if (st == FULL)
		{
			str += " AND (name = '" + strT + "')";
		} 
		else
		{			
			if(st == PARTIAL)
			{
				strT = "%" + strT + "%";
			}
			str += " AND (name LIKE '" + strT + "')";
		}
		if (i < categories.count())
		{
			str += " UNION ";
		} 
	}
	model->setQuery(str);
	QString er = model->lastError().text();
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));	
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Наименование"));
	return model;
}

// Поиск с выбранным типом поиска (st) по классификатору категорий 
QAbstractTableModel* DbConceptualModel::findText(QString string, SearchType st, Category category, int codeGroup, int codeInGroup)
{
	if(!isConnect())
	{
		return NULL;
	}
	QSqlQueryModel *model = new QSqlQueryModel();
	QString str = "";
	string = string.toLower().simplified();
	QString strT = string;
	str += "SELECT '" + QString(category) + "'||id," 
		"('" + QString(category) + "' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '') ) AS code," +
		"name " +
		" FROM " + nameTable[category] +
		" WHERE confirm = " + QString::number(EXPERT) + " AND code_group BETWEEN 0 AND 99";
	if(codeGroup != -1)
	{
		str += " AND code_group = " + QString::number(codeGroup);
	}
	if(codeInGroup != -1)
	{
		str += " AND code_in_group = " + QString::number(codeInGroup);
	}
	if (st == FULL)
	{
		str += " AND (name = '" + strT + "')";
	} 
	else
	{			
		if(st == PARTIAL)
		{
			strT = "%" + strT + "%";
		}
		str += " AND (name LIKE '" + strT + "')";
	}
	model->setQuery(str);
	QString er = model->lastError().text();
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));	
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Наименование"));
	return model;
}

// Поиск по коду
QAbstractTableModel* DbConceptualModel::findCode(QString code,  SearchType st)
{
	if(!isConnect())
	{
		return NULL;
	}
	code = code.toUpper().simplified();
	QChar c = code[0];
	int table = -1;
	QList<Category> tablesForFind = nameTable.keys();
	QString str;
	QSqlQueryModel *model = new QSqlQueryModel();
	// поиск к какому классу(таблице) принадлежит код
	foreach(Category var, tablesForFind)
	{
		if (c == var)
		{
			table = var;
			break;
		}
	}
	if(table != -1)
	{
		// код таблицы найден/ Значит поиск производится в одной таблице
		tablesForFind.clear();
		tablesForFind.append((Category)table);
	}
	int i = 0;
	foreach(Category var, tablesForFind)
	{
		i++;
		QString codeF = code;
		str += "SELECT '" + QString(var) + "'||id," 
			"('" + QString(var) + "' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '') ) AS code," +
			"name " + 
			" FROM " + nameTable[var] +
			" WHERE confirm = " + QString::number(EXPERT) + " AND code_group BETWEEN 0 AND 99";
		if (st == FULL)
		{
			str += " AND (code = '" + codeF + "')";
		} 
		else
		{			
			if(st == PARTIAL)
			{
				codeF = "%" + codeF + "%";
			}
			str += " AND (code LIKE '" + codeF + "')";
		}
		if (i < tablesForFind.count())
		{
			str += " UNION ";
		} 
	}
	model->setQuery(str);
	QString er = model->lastError().text();
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));	
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Наименование"));
	return model;	
}


// ИНФОЛОГИЧЕСКАЯ МОДЕЛЬ
DbInfologicalModel::DbInfologicalModel(): DbElementsOfStructure(){}

// формат возвращаемых данных getTable: id[string], code[string], name[string]
QAbstractTableModel* DbInfologicalModel::getTable(Category table)
{
	if(!isConnect())
	{
		return NULL;
	}
	QSqlQueryModel *model = new QSqlQueryModel();
	QString str = "SELECT '" + QString(table) + "'||id," 
		"(type || level || '." + QString(table) + "' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '') ) AS code,"
		"name " +
		" FROM " + nameTable[table] +
		" WHERE confirm = " + QString::number(EXPERT) + " AND type IS NOT NULL AND code_group BETWEEN 0 AND 99 " +
		"UNION SELECT '" + QString(table) + "'||id," +
		"(type || level || '.' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '')) AS code,"
		"name " + 
		" FROM " + nameTable[table] +
		" WHERE confirm = " + QString::number(EXPERT) + " AND type IS NOT NULL AND code_group NOT BETWEEN 0 AND 99 ";
	model->setQuery(str);
	QString er = model->lastError().text();
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));	
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Наименование"));
	if (table == VALUES)
	{
		model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Единица измерения"));
	}
	return model;
}

// получить набор категорий <код с наименованием, индекс>
QMap<QString, int>  DbInfologicalModel::getCategories(Category table)
{
	QMap<QString, int> result;
	if(!isConnect())
	{
		return result;
	}
	QString str;
	QSqlQuery query;
	
	str = "SELECT DISTINCT ('" + QString(table) + "' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '') || ' ' || name) AS c, id"
		" FROM " + nameTable[table] +
		" WHERE confirm = " + QString::number(EXPERT);
	// для PROCESS и COMPONENT могут повторятся коды концептуального уровня, то есть инфологическая единица может уже быть в базе
	if (!(table == PROCESSES || table == COMPONENTS))
	{
		str += " AND type IS NULL";
	}
	if(!query.exec(str))
	{
		result.insert(query.lastError().text(), -1);
	}
	while(query.next())
	{
		result.insert(query.value(0).toString(), query.value(1).toInt());
	}
	return result;
}

QAbstractTableModel* DbInfologicalModel::getTableQueriesInsert( )
{
	if(!isConnect())
	{
		return NULL;
	}
	QSqlQueryModel *model = new QSqlQueryModel();
	QString str;
	int i = 0;
	QList<Category> tables= nameTable.keys();
	foreach(Category var, tables)
	{
		i++;
		str += "SELECT '" + QString(var) + "'||id,"  
			"(type || level || '." + QString(var) + "' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '') ) AS code,"
			"name"
			" FROM " + nameTable[var] +
			" WHERE confirm = " + QString::number(USER) + " AND type IS NOT NULL  AND code_group BETWEEN 0 AND 99"
		" UNION SELECT '" + QString(var) + "'||id," +
			"(type || level || '.' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '')) AS code,"
			"name " + 
			" FROM " + nameTable[var] +
			" WHERE confirm = " + QString::number(USER) + " AND type IS NOT NULL AND code_group NOT BETWEEN 0 AND 99 ";
		if (i < tables.count())
		{
			str += " UNION ";
		} 
	}
	model->setQuery(str);
	QString er = model->lastError().text();
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));	
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Наименование"));
	return model;
}
QAbstractTableModel* DbInfologicalModel::getTableQueriesEdit()
{
	if(!isConnect())
	{
		return NULL;
	}
	QList<Category> tables= nameTable.keys();
	QString str;
	QSqlQueryModel *model = new QSqlQueryModel();
	// поиск к какому классу(таблице) принадлежит код
	int i = 0;
	foreach(Category var, tables)
	{
		i++;
		str += "SELECT '" + QString(var) + "'||id,"  
			"(type || level || '." + QString(var) + "' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '') ) AS code,"
			"name, (SELECT name FROM " + nameTable[var] + " WHERE id = b.confirm - 1 - " + QString::number(USER) + ")"
			" FROM " + nameTable[var] + " AS b"
			" WHERE confirm > " + QString::number(USER) + " AND type IS NOT NULL  AND code_group BETWEEN 0 AND 99"
		" UNION SELECT '" + QString(var) + "'||id," +
			"(type || level || '.' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '')) AS code,"
			"name, (SELECT name FROM " + nameTable[var] + " WHERE id = b.confirm - 1 - " + QString::number(USER) + ")"
			" FROM " + nameTable[var] + " AS b"
			" WHERE confirm > " + QString::number(USER) + " AND type IS NOT NULL AND code_group NOT BETWEEN 0 AND 99 ";
		if (i < tables.count())
		{
			str += " UNION ";
		} 		
	}

	model->setQuery(str);
	QString er = model->lastError().text();
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));	
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Новое наименование"));
	model->setHeaderData(3, Qt::Horizontal, QStringLiteral("Старое наименование"));
	return model;
}

// Генерирование кодировки. Если кодировка не допустима, возвращается пустая строка
QString DbInfologicalModel::generateCode(QChar type, int level, QString code_1, QString code_2)
{

	if(!getTypes().values().contains(type))
	{
		return QStringLiteral("Неверный тип"); // type недопустимый
	}
	if (level <= 0)
	{
		return QStringLiteral("Неверный код уровня"); // level неверный
	}
	if(code_1.isEmpty())
	{
		return QStringLiteral("Пустой код 1"); // code_1 пустой
	}
	if(code_2.isEmpty())
	{
		return QStringLiteral("Пустой код 2"); 
	}	
	return  QString(type) + QString::number(level) + "."  + code_1 + "." + code_2;
}

QString DbInfologicalModel::generateCode(QChar type, int level, Category table, int idCategory)
{

	if(!isConnect())
	{
		return NULL;
	}

	if(!getTypes().values().contains(type))
	{
		return QStringLiteral("Неверный тип"); // type недопустимый
	}
	if (level <= 0)
	{
		return QStringLiteral("Неверный код уровня"); // level неверный
	}
	QSqlQuery query;
	QString str;
	str = "SELECT code_group ||'.'|| code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '') FROM "+ nameTable[table] + " WHERE id = " + QString::number(idCategory) +
		" AND confirm = " + QString::number(EXPERT);
	query.exec(str);
	if(!query.next())
	{
		return QStringLiteral("Катагории с данным id не существует");
	}
	return QString(type) + QString::number(level) + "." + QString(table) + query.value(0).toString();
}
// вставить ИСЕ инфлогического уровня
int DbInfologicalModel::insertRecord(UserType user, QChar type, int level, Category table, QString code_1, QString code_2, QString name)
{	
	if(!isConnect())
	{
		return NULL;
	}
	QSqlQuery query;
	QString str;
	code_1 = code_1.toUpper().simplified();
	code_2 = code_2.toUpper().simplified();
	if (table == CYCLES || table == TASKS || table == OBJECTS)
	{
		if(!getTypes().values().contains(type))
		{
			return -2; // type недопустимый
		}
		if (level <= 0)
		{
			return -3; // level неверный
		}
		if(code_1.isEmpty())
		{
			return -41; // code_1 пустой
		}
		if(code_2.isEmpty())
		{
			return -42; 
		}	

		str = "SELECT * FROM " + nameTable[table] +
			" WHERE code_group = '" + code_1 + 
			"' AND code_in_group = '" + code_2 + 
			"' AND type = '" + QString(type) +
			"' AND level = " + QString::number(level) +
			" AND confirm = " + QString::number(EXPERT);
		query.exec(str);
		if(query.next())
		{
			// запись с таким кодом существует
			return -6;
		}
		name = name.toLower().simplified();
		str = "INSERT INTO " + nameTable[table] + " (type, level, code_group, code_in_group, extra_code, name, confirm) VALUES('" +
			QString(type) + "', " + 
			QString::number(level) + ", '" + 
			code_1 + "', '" + 
			code_2 + "', '', '" + 
			name + "', " + 
			QString::number(user) + ")";

		if(!query.exec(str))
		{
			QString er = query.lastError().text();
			return -7;
		}
		return 0;
	}
	else
	{
		// недопустимо для данного класса
		return -8;
	}
}

// вставить ИСЕ инфлогического уровня на основе категории концептуального уровня
int DbInfologicalModel::insertRecord(UserType user, QChar type, int level, Category table, int idCategory)
{
	if(!isConnect())
	{
		return NULL;
	}
	QSqlQuery query, record;
	QString str;

	if(!getTypes().values().contains(type))
	{
		return -2; // type недопустимый
	}
	if (level <= 0)
	{
		return -3; // level неверный
	}
	str = "SELECT code_group, code_in_group, extra_code, type FROM "+ nameTable[table] + " WHERE id = " + QString::number(idCategory) +
		" AND confirm = " + QString::number(EXPERT);
	record.exec(str);
	if(!record.next())
	{
		// параметра с данным id не существует
		return -4;
	}
	QString isType= record.value(3).toString();
	str = "SELECT code_in_group, code_group, extra_code FROM " + nameTable[table] +
		" WHERE code_group = '" + record.value(0).toString() + 
		"' AND code_in_group = '" + record.value(1).toString() + 
		"' AND extra_code = '" + record.value(2).toString() + 
		"' AND type = '" + QString(type) +
		"' AND level = " + QString::number(level) +
		" AND confirm = " + QString::number(EXPERT);
	query.exec(str);
	if(query.next())
	{
		// запись с таким кодом существует
		return -6;
	}
	if(user == EXPERT && !(table == PROCESSES || table == COMPONENTS) ||  isType.isEmpty())
	{
		str = "UPDATE " + nameTable[table] + 
			" SET type = '" + QString(type) +
			"', level = " + QString::number(level) +
			" WHERE id = " +  QString::number(idCategory) +
			" AND confirm = " + QString::number(EXPERT);
	}
	// для  PROCESS и COMPONENT могут повторятся концептуальные кода, значит необходимо создать новую запись
	else
	{
		str = "INSERT INTO " + nameTable[table] + " (confirm, type, level, name, code_group, code_in_group, extra_code)" 
			" SELECT '" + QString::number(user) + "', '"+QString(type)+"', "+QString::number(level) +", name, code_group, code_in_group, extra_code FROM " +
			nameTable[table] + " WHERE id = " + QString::number(idCategory);
	}

	if(!query.exec(str))
	{
		QString er = query.lastError().text();
		return -7;
	}
	return 0;
}


int DbInfologicalModel::deleteRecord(QString id, Category table)
{
	if(!isConnect())
	{
		return -1;
	}
	QSqlQuery query;
	QString str;
	QChar c = id[0];
	QList<Category> tablesForFind = nameTable.keys();
	QSqlQueryModel *model = new QSqlQueryModel();
	// поиск к какому классу(таблице) принадлежит код
	foreach(Category var, tablesForFind)
	{
		if (c == var)
		{
			table = var;
			id = id.mid(1);
			break;
		}
	}
	// если запись была создана  как новая на инфологическом уровне необходимо удалить полностью
	str = "SELECT * FROM " + nameTable[table] + " WHERE code_group BETWEEN 0 AND 99 AND id = " + id;
	query.exec(str);
	QString er = query.lastError().text();
	if(query.next())
	{
		str = "UPDATE " + nameTable[table] + 
			" SET type = NULL, level = NULL WHERE code_group BETWEEN 0 AND 99 AND id = " + id;
		query.exec(str);
		er = query.lastError().text();
		return 0;
	}

	return DbElementsOfStructure::deleteRecord(id, table);
}

// получить коды групп созданных единиц
QList<QPair<QString, QVariant>> DbInfologicalModel::getGroupCodes(Category table)
{
	QList<QPair<QString, QVariant>> result;
	if(!isConnect())
	{
		return result;
	}
	QList<QPair<QString, int>> groupCodes = getClassifierCodeGroup(table);
	QString str;
	QSqlQuery codes;
	str = "SELECT DISTINCT code_group FROM " + nameTable[table] + 
		" WHERE type IS NOT NULL "
		" AND confirm = " + QString::number(EXPERT);
	codes.exec(str);
	if(!codes.exec(str))
	{
		QString er = codes.lastError().text();
	}

	while(codes.next())
	{
		bool isClassifier = false;
		QPair<QString, int> var;
		foreach (var, groupCodes)
		{
			if(var.second == codes.value(0))
			{
				result.push_back(var);
				isClassifier = true;
			}
		}
		if(!isClassifier)
		{
			// для единиц без заданных классификационых кодов
			result.push_back(qMakePair(codes.value(0).toString(),codes.value(0)) );
		}
	}
	return result;
}
// получить коды в группе созданных единиц
QList<QVariant> DbInfologicalModel::getInGroupCodes(QVariant codeGroup, Category table)
{
	QList<QVariant> resault;
	if(!isConnect())
	{
		return resault;
	} 
	QString str;
	QSqlQuery codes;
	str = "SELECT DISTINCT code_in_group FROM " + nameTable[table] + 
		" WHERE type IS NOT NULL AND code_group = " + codeGroup.toString().toUpper() +
		" AND confirm = " + QString::number(EXPERT) + " ORDER BY code_in_group";
	codes.exec(str);
	if(!codes.exec(str))
	{
		QString er = codes.lastError().text();
	}

	while(codes.next())
	{
		resault.push_back(codes.value(0));
	}
	return resault;
}


// Поиск по коду
QAbstractTableModel* DbInfologicalModel::findCode(QString code,  SearchType st)
{
	if(!isConnect())
	{
		return NULL;
	}
	code = code.toUpper().simplified();
	QChar c = code[0];
	int table = -1;
	QList<Category> tablesForFind = nameTable.keys();
	QString str;
	QSqlQueryModel *model = new QSqlQueryModel();
	int i = 0;
	foreach(Category var, tablesForFind)
	{
		i++;
		QString codeF = code;
		str += "SELECT '" + QString(var) + "'||id," 
			"(type || level || '." + QString(var) + "' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '') ) AS code,"
			"name " + 
			" FROM " + nameTable[var] +
			" WHERE confirm = " + QString::number(EXPERT) + " AND type IS NOT NULL AND code_group BETWEEN 0 AND 99 ";
		int j = 2;
		// цикл в два прохода для записей с особым кодом
		while(--j >= 0)
		{
			if (st == FULL)
			{
				str += " AND (code = '" + codeF + "')";
			} 
			else
			{			
				if(st == PARTIAL)
				{
					codeF = "%" + codeF + "%";
				}
				str += " AND (code LIKE '" + codeF + "')";
			}
			if(j == 1)
			{
				str += "UNION SELECT '" + QString(table) + "'||id, " 
					"(type || level || '.' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '')) AS code," 
					"name FROM " + nameTable[var] +
					" WHERE confirm = " + QString::number(EXPERT) + " AND type IS NOT NULL AND code_group NOT BETWEEN 0 AND 99 ";
			}
		}
		if (i < tablesForFind.count())
		{
			str += " UNION ";
		} 
	}
	model->setQuery(str);
	QString er = model->lastError().text();
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));	
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Наименование"));
	return model;
}

// Поиск с выбранным типом поиска (st) по классам, типам и уровню 
QAbstractTableModel* DbInfologicalModel::findText(QString string, SearchType st, QList<Category> categories, QList<QChar> types, int level)
{
	if(!isConnect() || categories.isEmpty())
	{
		return NULL;
	}
	QSqlQueryModel *model = new QSqlQueryModel();
	QString str = "";
	string = string.toLower().simplified();
	int i = 0;
	foreach(Category var, categories)
	{
		i++;
		QString strT = string;
		str += "SELECT '" + QString(var) + "'||id," 
			"(type || level || '." + QString(var) + "' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '') ) AS code," +
			"name " +
			" FROM " + nameTable[var] +
			" WHERE confirm = " + QString::number(EXPERT) + " AND type IS NOT NULL AND code_group BETWEEN 0 AND 99";
		int j = 2;
		// цикл в два прохода для записей с особым кодом
		while(--j >= 0)
		{
			if(level > 0)
			{
				str += " AND level = " + QString::number(level);
			}
			int tcount = types.count();
			if(tcount > 0)
			{
				str += " AND type IN (";
				for(int j = 0; j < tcount; j++)
				{
					str += "'" + QString(types[j]) + "'";
					if(j < tcount -1)
					{
						str += ", ";
					}
				}
				str += ") ";
			}

			if (st == FULL)
			{
				str += " AND (name = '" + strT + "')";
			} 
			else
			{			
				if(st == PARTIAL)
				{
					strT = "%" + strT + "%";
				}
				str += " AND (name LIKE '" + strT + "')";
			}
			if(j == 1)
			{
				str += "UNION SELECT '" + QString(var) + "'||id," 
					"(type || level || '.' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '')) AS code," 
					"name FROM " + nameTable[var] +
					" WHERE confirm = " + QString::number(EXPERT) + " AND type IS NOT NULL AND code_group NOT BETWEEN 0 AND 99 ";
			}
		}

		if (i < categories.count())
		{
			str += " UNION ";
		} 
	}
	model->setQuery(str);
	QString er = model->lastError().text();
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));	
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Наименование"));
	return model;
}

QAbstractTableModel* DbInfologicalModel::findText(QString string, SearchType st, Category category, QList<QChar> types,  QString codeGroup, QString codeInGroup, int level)
{
	if(!isConnect())
	{
		return NULL;
	}
	QSqlQueryModel *model = new QSqlQueryModel();
	QString str = "";
	string = string.toLower().simplified();
	codeGroup = codeGroup.toUpper().simplified();
	codeInGroup = codeInGroup.toUpper().simplified();
	QString strT = string;
	str += "SELECT '" + QString(category) + "'||id," 
		"(type || level || '." + QString(category) + "' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '') ) AS code," +
		"name " +
		" FROM " + nameTable[category] +
		" WHERE confirm = " + QString::number(EXPERT) + " AND code_group BETWEEN 0 AND 99";
	int j = 2;
	// цикл в два прохода для записей с особым кодом
	while(--j >= 0)
	{
		if(level > 0)
		{
			str += " AND level = " + QString::number(level);
		}
		int tcount = types.count();
		if(tcount > 0)
		{
			str += " AND type IN (";
			for(int j = 0; j < tcount; j++)
			{
				str += "'" + QString(types[j]) + "'";
				if(j < tcount -1)
				{
					str += ", ";
				}
			}
			str += ") ";
		}
		if(!codeGroup.isEmpty() && codeGroup.toInt() != -1)
		{
			str += " AND code_group = '" + codeGroup + "' ";
		}
		if(!codeInGroup.isEmpty() && codeInGroup.toInt() != -1)
		{
			str += " AND code_in_group = '" + codeInGroup + "' ";
		}
		if (st == FULL)
		{
			str += " AND (name = '" + strT + "')";
		} 
		else
		{			
			if(st == PARTIAL)
			{
				strT = "%" + strT + "%";
			}
			str += " AND (name LIKE '" + strT + "')";
		}
		if(j == 1)
		{
			str += " UNION SELECT '" + QString(category) + "'||id," 
				"(type || level || '.' || code_group || '.' || code_in_group || COALESCE((SELECT '.' || extra_code WHERE extra_code <> ''), '')) AS code," 
				"name FROM " + nameTable[category] +
				" WHERE confirm = " + QString::number(EXPERT) + " AND type IS NOT NULL AND code_group NOT BETWEEN 0 AND 99 ";
		}
	}	
	model->setQuery(str);
	QString er = model->lastError().text();
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));	
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("Наименование"));
	return model;
}