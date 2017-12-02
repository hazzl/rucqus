#ifndef PLISTMODEL_H
#define PLISTMODEL_H

#include "sqlquerymodel.h"

class PListModel : public SqlQueryModel
{
	Q_OBJECT
public:
	PListModel(QObject *parent = nullptr);
public slots:
	virtual void refresh();
signals:
	void newData();
};

#endif // PLISTMODEL_H
