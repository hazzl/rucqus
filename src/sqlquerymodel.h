#ifndef SQLQUERYMODEL_H
#define SQLQUERYMODEL_H
#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class QQuickItem; //forward declaration

class SqlQueryModel : public QSqlQueryModel
{
	Q_OBJECT
public:
    SqlQueryModel(QObject *parent);
    ~SqlQueryModel();
    void setQuery(const QString &query, const QSqlDatabase &db = QSqlDatabase());
    void setQuery(const QSqlQuery &query);
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const { return m_roleNames; }

public slots:
    virtual void refresh() {}

private:
    void generateRoleNames();
    QHash<int, QByteArray> m_roleNames;
protected:
    int getCurrentIndex(const QQuickItem *, const SqlQueryModel *);
    QSqlQuery *q;
};
#endif // SQLQUERYMODEL_H
