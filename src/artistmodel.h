#ifndef ARTISTMODEL_H
#define ARTISTMODEL_H

#include "sqlquerymodel.h"

class ArtistModel : public SqlQueryModel
{
	Q_OBJECT
public:
	ArtistModel(QObject *parent = 0);
public slots:
	virtual void refresh();
};

#endif // ARTISTMODEL_H
