#include "sqlquerymodel.h"
#include "rucqusapp.h"
#include <QSqlRecord>
#include <QModelIndex>

SqlQueryModel::SqlQueryModel(QObject *parent) :
    QSqlQueryModel(parent)
{
	Q_CHECK_PTR (parent);
	q = new QSqlQuery;
}

SqlQueryModel::~SqlQueryModel()
{
	delete q;
}

void SqlQueryModel::setQuery(const QString &query, const QSqlDatabase &db)
{
    QSqlQueryModel::setQuery(query, db);
    generateRoleNames();
}

void SqlQueryModel::setQuery(const QSqlQuery & query)
{
    QSqlQueryModel::setQuery(query);
    generateRoleNames();
}

void SqlQueryModel::generateRoleNames()
{
    m_roleNames.clear();
    for( int i = 0; i < record().count(); ++i) {
		m_roleNames.insert(Qt::UserRole + i, record().fieldName(i).toUtf8());
    }
}

QVariant SqlQueryModel::data(const QModelIndex &searchIndex, int role) const
{
    QVariant value;

    if(role < Qt::UserRole) {
		value = QSqlQueryModel::data(searchIndex, role);
    }
    else {
		int column = role - Qt::UserRole;
		value = QSqlQueryModel::data(this->index(searchIndex.row(), column));
    }
    return value;
}

int SqlQueryModel::getCurrentIndex(const QQuickItem *list, const SqlQueryModel *model)
{
	int ret {-1};
	if (list)
	{
		int row = list->property("currentIndex").toInt();
		if (row >= 0)
		{
			/* we want get the uid of the element in the row pointed to by "currentIndex"
			 * as the uid is always stored in column Qt::UserRole (first column in the SELECT statement)
			 * we can skip looking up the column number by name in m_roleNames
			 */
			QModelIndex modelIndex = this->createIndex(row, 0);
			ret = model->data(modelIndex, Qt::UserRole).toInt();
		}
	}
	return ret;
}
