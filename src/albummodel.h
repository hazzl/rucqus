#ifndef ALBUMMODEL_H
#define ALBUMMODEL_H

#include "sqlquerymodel.h"

class AlbumModel : public SqlQueryModel
{
	Q_OBJECT
public:
	AlbumModel(QObject *parent = nullptr);
public slots:
	virtual void refresh();
};

#endif // ALBUMMODEL_H
