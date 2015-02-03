#pragma once

#include <QtSql>

enum Category
{
	CYCLES = 'S',
	PROCESSES = 'P',
	TASKS = 'Z',
	COMPONENTS = 'K',
	OBJECTS = 'O',
	ATTRIBUTES = 'R',
	VALUES = 'C'
};
enum SearchType
{
	FULL,
	TEMPLATE,
	PARTIAL
};
enum UserType
{
	EXPERT,
	USER
};

class DbElementsOfStructure
{
public:
	DbElementsOfStructure ();
	virtual ~DbElementsOfStructure();
	static int connect(QString path); // подключение к БД
	static void disconnect();
	// наименования классов в правильном порядке
	static QStringList getSortClassNames(); 
	// набор соответствий наименований классов к категориям
	static QMap<QString, Category> getCategoryNames();
	// получить таблицу типов, формат возвращаемых данных: code[QChar], name[string]
	QMap<QString, QChar> getTypes();
	// получить справочник классификатора кодов групп для категорий , формат возвращаемых данных: полное наименование[QString], код[int]
	QList<QPair<QString, int>> getClassifierCodeGroup(Category table = ATTRIBUTES); 
    // получить классификацию кодов в группе для категории "Компоненты"
	QList<QPair<QString, int>> getClassifierCodeInGroupComponent();
	// формат возвращаемых данных getTable: id[string], code[string], name[string]
	virtual QAbstractTableModel* getTable(Category table = ATTRIBUTES) = 0;
	// получить коды групп созданных единиц: QPair<полное наименование, код группы>
	virtual QList<QPair<QString, QVariant>> getGroupCodes(Category table = ATTRIBUTES) = 0;
	// получить коды в группе созданных единиц
	virtual QList<QVariant> getInGroupCodes(QVariant codeGroup, Category table = ATTRIBUTES) = 0;

	// Получить последниый допустимый код в группе (недопустимо для COMPONENTS) 
	virtual int getValidCodeInGroupe(int codeGroup, Category table = ATTRIBUTES);
	// получить таблицу с запросами на добавление данных от обычных пользователей
	virtual QAbstractTableModel* getTableQueriesInsert() = 0;
	// получить таблицу с запросами на изменения данных от обычных пользователей
	virtual QAbstractTableModel* getTableQueriesEdit() = 0;
	// подтвердить запрос пользователя
	static int confirmQuery(QString id, Category table = ATTRIBUTES);
	//Редактирование, id - id редактируемой записи, возможно без указания table
	static int editRecord(UserType user, QString id, QString name, Category table = ATTRIBUTES);	
	// Удаление записи всей записи на всех уровнях в том числе и для удаления запроса пользователя , возможно без указания table
	static int deleteRecord(QString id, Category table = ATTRIBUTES);
	// Поиск по коду
	virtual QAbstractTableModel* findCode(QString code,  SearchType st) = 0;
protected:
	static QSqlDatabase _sdb;
	static QMap<Category, QString> nameTable;
	static bool isConnect();

};

class DbInitialModel: public DbElementsOfStructure
{
public:
	DbInitialModel();
	// формат возвращаемых данных getTable: id[string], code[string], name[string]
	QAbstractTableModel* getTable(Category table = ATTRIBUTES);
	// получить таблицу с запросами на добавление данных от обычных пользователей
	QAbstractTableModel* getTableQueriesInsert();
	// получить таблицу с запросами на изменения данных от обычных пользователей
	QAbstractTableModel* getTableQueriesEdit();
	// получить коды групп созданных признаков
	QList<QPair<QString, QVariant>> getGroupCodes(Category table = ATTRIBUTES);
	// получить коды в группе созданных единиц
	QList<QVariant> getInGroupCodes(QVariant codeGroup, Category table = ATTRIBUTES);
	// Генерирование кодировки. Если кодировка не допустима, возвращается пустая строка
	QString generateCode(int codeGroup, int codeInGroup);
	// вставить параметр начального уровня
	int insertRecord(UserType user, int codeGroup, int codeInGroup, QString name);

	// Поиск по тексту с выбранным типом поиска (st) 
	QAbstractTableModel* findText(QString string, SearchType st);
	// Поиск с выбранным типом поиска (st) по классификатору  
	QAbstractTableModel* findText(QString string, SearchType st, int code);
	// Поиск по коду
	QAbstractTableModel* findCode(QString code,  SearchType st);
};


class DbConceptualModel: public DbElementsOfStructure
{
public:
	DbConceptualModel();
	// формат возвращаемых данных getTable: id[string], code[string], name[string]
	QAbstractTableModel* getTable(Category table = ATTRIBUTES);
	// получить таблицу с запросами на добавление данных от обычных пользователей
	QAbstractTableModel* getTableQueriesInsert();
	// получить таблицу с запросами на изменения данных от обычных пользователей
	QAbstractTableModel* getTableQueriesEdit();
	// получить набор параметров, <код с наименованием, индекс>
	QMap<QString, int>  getParametrs();
	// Получить последний допустимый дополнительный код 
	int getValidExtraCode(Category table, int codeGroup, int codeInGroup);
	int getValidExtraCode(int idParametr);
	// получить коды групп созданных единиц
	QList<QPair<QString, QVariant>> getGroupCodes(Category table = ATTRIBUTES);
	// получить коды в группе созданных единиц
	QList<QVariant> getInGroupCodes(QVariant codeGroup, Category table = ATTRIBUTES);
	// Генерирование кодировки. Если кодировка не допустима, возвращается пустая строка
	QString generateCode(Category table, int codeGroup, int codeInGroup, int extraCode);
	QString generateCode(int idParametr, int extraCode);
	// вставить категорию концептуального уровня
	int insertRecord(UserType user, Category table, int codeGroup, int codeInGroup, int extraCode, QString name);
	// вставить признак концептуального уровня на основе параметра
	int insertRecord(UserType user, int idParametr, int extraCode, QString name);

	// Поиск с выбранным типом поиска (st) по категориям 
	QAbstractTableModel* findText(QString string, SearchType st, QList<Category> categories);
	// Поиск с выбранным типом поиска (st) по классификатору категорий 
	QAbstractTableModel* findText(QString string, SearchType st, Category category, int codeGroup = -1, int codeInGroup = -1);
	// Поиск по коду
	QAbstractTableModel* findCode(QString code,  SearchType st);
private:
	// Проверить данные на правильность
	int checkData(Category table, int codeGroup, int codeInGroup, int extraCode, QString name);
};

class DbInfologicalModel: public DbElementsOfStructure
{
public:
	DbInfologicalModel();
	// формат возвращаемых данных getTable: id[string], code[string], name[string]
	QAbstractTableModel* getTable(Category table = ATTRIBUTES);
	// получить таблицу с запросами на добавление данных от обычных пользователей
	QAbstractTableModel* getTableQueriesInsert();
	// получить таблицу с запросами на изменения данных от обычных пользователей
	QAbstractTableModel* getTableQueriesEdit();
	// получить набор категорий <код с наименованием, индекс>
	QMap<QString, int>  getCategories(Category table);
	// получить коды групп созданных единиц
	QList<QPair<QString, QVariant>> getGroupCodes(Category table = ATTRIBUTES);
	// получить коды в группе созданных единиц
	QList<QVariant> getInGroupCodes(QVariant codeGroup, Category table = ATTRIBUTES);
	// Генерирование кодировки. Если кодировка не допустима, возвращается пустая строка
	QString generateCode(QChar type, int level, QString code_1, QString code_2);
	QString generateCode(QChar type, int level, Category table, int idCategory);
	// вставить ИСЕ инфлогического уровня
	int insertRecord(UserType user, QChar type, int level, Category table, QString code_1, QString code_2, QString name);
	// вставить ИСЕ инфлогического уровня на основе категории концептуального уровня
	int insertRecord(UserType user, QChar type, int level, Category table, int idCategory);
	// Удаление данных инфологического уровня
	int deleteRecord(QString id, Category table = ATTRIBUTES);

	// Поиск с выбранным типом поиска (st) по категориям, по типам, по уровню
	QAbstractTableModel* findText(QString string, SearchType st, QList<Category> categories, QList<QChar> types, int level = -1);
	// Поиск с выбранным типом поиска (st) по классификатору категорий 
	QAbstractTableModel* findText(QString string, SearchType st, Category category, QList<QChar> types,  QString codeGroup = NULL, QString codeInGroup = NULL, int level = -1);
	// Поиск по коду
	QAbstractTableModel* findCode(QString code,  SearchType st);
};