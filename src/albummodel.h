#ifndef ALBUMMODEL_H
#define ALBUMMODEL_H

#include "sqlquerymodel.h"

class AlbumModel : public SqlQueryModel
{
	Q_OBJECT
public:
	AlbumModel(QObject *parent = 0);
public slots:
	virtual void refresh();
};

#endif // ALBUMMODEL_H
