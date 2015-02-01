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
	static int connect(QString path); // ����������� � ��
	static void disconnect();
	// ������������ ������� � ���������� �������
	static QStringList getSortClassNames(); 
	// ����� ������������ ������������ ������� � ����������
	static QMap<QString, Category> getCategoryNames();
	// �������� ������� �����, ������ ������������ ������: code[QChar], name[string]
	QMap<QString, QChar> getTypes();
	// �������� ���������� �������������� ����� ����� ��� ��������� , ������ ������������ ������: ������ ������������[QString], ���[int]
	QList<QPair<QString, int>> getClassifierCodeGroup(Category table = ATTRIBUTES); 
    // �������� ������������� ����� � ������ ��� ��������� "����������"
	QList<QPair<QString, int>> getClassifierCodeInGroupComponent();
	// ������ ������������ ������ getTable: id[string], code[string], name[string]
	virtual QAbstractTableModel* getTable(Category table = ATTRIBUTES) = 0;
	// �������� ���� ����� ��������� ������: QPair<������ ������������, ��� ������>
	virtual QList<QPair<QString, QVariant>> getGroupCodes(Category table = ATTRIBUTES) = 0;
	// �������� ���� � ������ ��������� ������
	virtual QList<QVariant> getInGroupCodes(QVariant codeGroup, Category table = ATTRIBUTES) = 0;

	// �������� ���������� ���������� ��� � ������ (����������� ��� COMPONENTS) 
	virtual int getValidCodeInGroupe(int codeGroup, Category table = ATTRIBUTES);
	// �������� ������� � ��������� �� ���������� ������ �� ������� �������������
	virtual QAbstractTableModel* getTableQueriesInsert() = 0;
	// �������� ������� � ��������� �� ��������� ������ �� ������� �������������
	virtual QAbstractTableModel* getTableQueriesEdit() = 0;
	// ����������� ������ ������������
	static int confirmQuery(QString id, Category table = ATTRIBUTES);
	//��������������, id - id ������������� ������, �������� ��� �������� table
	static int editRecord(UserType user, QString id, QString name, Category table = ATTRIBUTES);	
	// �������� ������ ���� ������ �� ���� ������� � ��� ����� � ��� �������� ������� ������������ , �������� ��� �������� table
	static int deleteRecord(QString id, Category table = ATTRIBUTES);
	// ����� �� ����
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
	// ������ ������������ ������ getTable: id[string], code[string], name[string]
	QAbstractTableModel* getTable(Category table = ATTRIBUTES);
	// �������� ������� � ��������� �� ���������� ������ �� ������� �������������
	QAbstractTableModel* getTableQueriesInsert();
	// �������� ������� � ��������� �� ��������� ������ �� ������� �������������
	QAbstractTableModel* getTableQueriesEdit();
	// �������� ���� ����� ��������� ���������
	QList<QPair<QString, QVariant>> getGroupCodes(Category table = ATTRIBUTES);
	// �������� ���� � ������ ��������� ������
	QList<QVariant> getInGroupCodes(QVariant codeGroup, Category table = ATTRIBUTES);
	// ������������� ���������. ���� ��������� �� ���������, ������������ ������ ������
	QString generateCode(int codeGroup, int codeInGroup);
	// �������� �������� ���������� ������
	int insertRecord(UserType user, int codeGroup, int codeInGroup, QString name);

	// ����� �� ������ � ��������� ����� ������ (st) 
	QAbstractTableModel* findText(QString string, SearchType st);
	// ����� � ��������� ����� ������ (st) �� ��������������  
	QAbstractTableModel* findText(QString string, SearchType st, int code);
	// ����� �� ����
	QAbstractTableModel* findCode(QString code,  SearchType st);
};


class DbConceptualModel: public DbElementsOfStructure
{
public:
	DbConceptualModel();
	// ������ ������������ ������ getTable: id[string], code[string], name[string]
	QAbstractTableModel* getTable(Category table = ATTRIBUTES);
	// �������� ������� � ��������� �� ���������� ������ �� ������� �������������
	QAbstractTableModel* getTableQueriesInsert();
	// �������� ������� � ��������� �� ��������� ������ �� ������� �������������
	QAbstractTableModel* getTableQueriesEdit();
	// �������� ����� ����������, <��� � �������������, ������>
	QMap<QString, int>  getParametrs();
	// �������� ��������� ���������� �������������� ��� 
	int getValidExtraCode(Category table, int codeGroup, int codeInGroup);
	int getValidExtraCode(int idParametr);
	// �������� ���� ����� ��������� ������
	QList<QPair<QString, QVariant>> getGroupCodes(Category table = ATTRIBUTES);
	// �������� ���� � ������ ��������� ������
	QList<QVariant> getInGroupCodes(QVariant codeGroup, Category table = ATTRIBUTES);
	// ������������� ���������. ���� ��������� �� ���������, ������������ ������ ������
	QString generateCode(Category table, int codeGroup, int codeInGroup, int extraCode);
	QString generateCode(int idParametr, int extraCode);
	// �������� ��������� ��������������� ������
	int insertRecord(UserType user, Category table, int codeGroup, int codeInGroup, int extraCode, QString name);
	// �������� ������� ��������������� ������ �� ������ ���������
	int insertRecord(UserType user, int idParametr, int extraCode, QString name);

	// ����� � ��������� ����� ������ (st) �� ���������� 
	QAbstractTableModel* findText(QString string, SearchType st, QList<Category> categories);
	// ����� � ��������� ����� ������ (st) �� �������������� ��������� 
	QAbstractTableModel* findText(QString string, SearchType st, Category category, int codeGroup = -1, int codeInGroup = -1);
	// ����� �� ����
	QAbstractTableModel* findCode(QString code,  SearchType st);
private:
	// ��������� ������ �� ������������
	int checkData(Category table, int codeGroup, int codeInGroup, int extraCode, QString name);
};

class DbInfologicalModel: public DbElementsOfStructure
{
public:
	DbInfologicalModel();
	// ������ ������������ ������ getTable: id[string], code[string], name[string]
	QAbstractTableModel* getTable(Category table = ATTRIBUTES);
	// �������� ������� � ��������� �� ���������� ������ �� ������� �������������
	QAbstractTableModel* getTableQueriesInsert();
	// �������� ������� � ��������� �� ��������� ������ �� ������� �������������
	QAbstractTableModel* getTableQueriesEdit();
	// �������� ����� ��������� <��� � �������������, ������>
	QMap<QString, int>  getCategories(Category table);
	// �������� ���� ����� ��������� ������
	QList<QPair<QString, QVariant>> getGroupCodes(Category table = ATTRIBUTES);
	// �������� ���� � ������ ��������� ������
	QList<QVariant> getInGroupCodes(QVariant codeGroup, Category table = ATTRIBUTES);
	// ������������� ���������. ���� ��������� �� ���������, ������������ ������ ������
	QString generateCode(QChar type, int level, QString code_1, QString code_2);
	QString generateCode(QChar type, int level, Category table, int idCategory);
	// �������� ��� �������������� ������
	int insertRecord(UserType user, QChar type, int level, Category table, QString code_1, QString code_2, QString name);
	// �������� ��� �������������� ������ �� ������ ��������� ��������������� ������
	int insertRecord(UserType user, QChar type, int level, Category table, int idCategory);
	// �������� ������ ��������������� ������
	int deleteRecord(QString id, Category table = ATTRIBUTES);

	// ����� � ��������� ����� ������ (st) �� ����������, �� �����, �� ������
	QAbstractTableModel* findText(QString string, SearchType st, QList<Category> categories, QList<QChar> types, int level = -1);
	// ����� � ��������� ����� ������ (st) �� �������������� ��������� 
	QAbstractTableModel* findText(QString string, SearchType st, Category category, QList<QChar> types,  QString codeGroup = NULL, QString codeInGroup = NULL, int level = -1);
	// ����� �� ����
	QAbstractTableModel* findCode(QString code,  SearchType st);
};