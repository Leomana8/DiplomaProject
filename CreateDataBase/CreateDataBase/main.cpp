
#include <QtCore/QCoreApplication>
#include <QtSql>

int main(int argc, char *argv[])
{

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //изменения
	QCoreApplication a(argc, argv);
	printf("Hello\n");
	QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
	sdb.setDatabaseName("db_elements_of_structures.sqlite");
	if (!sdb.open()) {
		printf("Don't open DB");
		return 1;
	}

	QSqlQuery query_create_table;
	QString str;
	str = "CREATE TABLE Types ("
			"code TEXT PRIMARY KEY,"
			"name TEXT"
			");";
	query_create_table.exec(str);

	str = "CREATE TABLE ClassifierCategories ("
		    "class TEXT,"
			"code INTEGER,"
			"name TEXT,"
			"PRIMARY KEY (class, code)"
			");";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());
	str = "CREATE TABLE Cycles ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT,"
			"type TEXT,"
			"level INTEGER,"
			"code_group TEXT,"
			"code_in_group TEXT,"
			"extra_code INTEGER,"
			"name TEXT,"
			"confirm INTEGER,"
			"FOREIGN KEY (type) REFERENCES Types (code),"
			"FOREIGN KEY (code_group) REFERENCES ClassifierCategories (code)"
			");";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());
	str = "CREATE TABLE Processes ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT,"
			"type TEXT,"
			"level INTEGER,"
			"code_group INTEGER,"
			"code_in_group INTEGER,"
			"extra_code INTEGER,"
			"name TEXT,"
			"confirm INTEGER," 
			"FOREIGN KEY (type) REFERENCES Types (code),"
			"FOREIGN KEY (code_group) REFERENCES ClassifierCategories (code)"
			");";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());
	str = "CREATE TABLE Tasks ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT,"
			"type TEXT,"
			"level INTEGER,"
			"code_group TEXT,"
			"code_in_group TEXT,"
			"extra_code INTEGER,"
			"name TEXT,"
			"confirm INTEGER,"
			"FOREIGN KEY (type) REFERENCES Types (code),"
			"FOREIGN KEY (code_group) REFERENCES Processes(code_in_group)"
			");";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());
	str = "CREATE TABLE Components ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT,"
			"type TEXT,"
			"level INTEGER,"
			"code_group INTEGER,"
			"code_in_group INTEGER,"
			"extra_code INTEGER,"
			"name TEXT,"
			"confirm INTEGER," 
			"FOREIGN KEY (type) REFERENCES Types (code),"
			"FOREIGN KEY (code_group) REFERENCES ClassifierCategories (code)"
			"FOREIGN KEY (code_in_group) REFERENCES ClassifierCategories (code)"
			");";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());
	str = "CREATE TABLE Objects ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT,"
			"type TEXT,"
			"level INTEGER,"
			"code_group TEXT,"
			"code_in_group TEXT,"
			"extra_code INTEGER,"
			"name TEXT,"
			"confirm INTEGER," 
			"FOREIGN KEY (type) REFERENCES Types (code),"
			"FOREIGN KEY (code_group) REFERENCES ClassifierCategories (code)"
			");";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());
	str = "CREATE TABLE Attributes ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT,"
			"type TEXT,"
			"level INTEGER,"
			"code_group INTEGER,"
			"code_in_group INTEGER,"
			"extra_code INTEGER,"
			"name TEXT,"
			"confirm INTEGER," 
			"FOREIGN KEY (type) REFERENCES Types (code),"
			"FOREIGN KEY (code_group) REFERENCES ClassifierCategories (code)"
			");";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());

	str = "CREATE TABLE NumValues ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT,"
			"type TEXT,"
			"level INTEGER,"
			"code_group INTEGER,"
			"code_in_group INTEGER,"
			"extra_code INTEGER,"
			"name TEXT,"
			"confirm INTEGER,"
			"FOREIGN KEY (type) REFERENCES Types (code),"
			"FOREIGN KEY (code_group) REFERENCES ClassifierCategories (code)"
			");";
	query_create_table.exec(str);


	str = "CREATE INDEX CodeIndCycles ON Cycles(code_group, code_in_group, extra_code, type, level );";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());
	str = "CREATE INDEX NameIndxCycles ON Cycles(name)";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());

	str = "CREATE INDEX CodeIndProcesses ON Processes(code_group, code_in_group, extra_code, type, level );";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());
	str = "CREATE INDEX NameIndxProcesses ON Processes(name)";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());

	str = "CREATE INDEX CodeIndTasks ON Tasks(code_group, code_in_group, extra_code, type, level );";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());
	str = "CREATE INDEX NameIndxTasks ON Tasks(name)";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());

	str = "CREATE INDEX CodeIndComponents ON Components(code_group, code_in_group, extra_code, type, level );";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());
	str = "CREATE INDEX NameIndxComponents ON Components(name)";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());

	str = "CREATE INDEX CodeIndxObjects ON Objects(code_group, code_in_group, extra_code, type, level );";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());
	str = "CREATE INDEX NameIndxObjects ON Objects(name)";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());

	str = "CREATE INDEX CodeIndxAttributes ON Attributes(code_group, code_in_group, extra_code, type, level );";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());
	str = "CREATE INDEX NameIndxAttributes ON Attributes(name)";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());

	str = "CREATE INDEX CodeIndxValues ON NumValues(code_group, code_in_group, extra_code, type, level );";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());
	str = "CREATE INDEX ValueIndx ON NumValues(name)";
	query_create_table.exec(str);
	printf(query_create_table.lastError().text().toStdString().c_str());
	
	QTextCodec* codec = QTextCodec::codecForName("UTF-8");	
	QTextCodec::setCodecForLocale(codec);

	QSqlQuery insert;


	
	//str = "INSERT INTO ClassifierCategories (class, code, name) VALUES"
	//	"('M', 'модуль'),"
	//	"('C', 'сущность'),"
	//	"('A','атрибут');";
	//insert.exec(str);
	//printf(insert.lastError().text().toStdString().c_str());

	/*
	INSERT INTO Types (code, name) VALUES
	('M', 'модуль'),
	('C', 'сущность'),
	('A','атрибут');

	INSERT INTO ClassifierCategories (class, code, name) VALUES
	('P', 1, 'функционирование'),
	('P', 2, 'изготовление'),
	('P', 3, 'проектирование'),
	('P', 4, 'подготовка производства'),
	('P', 5, 'исследование');

	INSERT INTO ClassifierCategories (class, code, name) VALUES
	('K', 1, 'субъект'),
	('K', 2, 'объект'),
	('K', 3, 'процесс'),
	('K', 4, 'документ'),
	('K', 5, 'форма'),
	('K', 6, 'условие-объект'),
	('K', 7, 'средство-устройство'),
	('K', 8, 'средство-программа'),
	('K', 9, 'средство-документ'),
	('K', 10, 'средство-форма/модель'),
	('K', 11, 'условие-документ'),
	('K', 12, 'условие- форма/модель');

	INSERT INTO ClassifierCategories (class, code, name) VALUES
	('R', 0, 'признаки связи'),
	('R', 1, 'идентификаторы '),
	('R', 2, 'наименования'),
	('R', 3, 'классификационные признаки'),
	('R', 4, 'признаки назначения/функционирования'),
	('R', 5, 'размерные признаки'),
	('R', 6, 'временные признаки'),
	('R', 7, 'массы'),
	('R', 8, 'энергетические (кинематические) признаки'),
	('R', 9, 'энергетические (динамические) признаки');
	*/
	return a.exec();
}
