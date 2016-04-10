#ifndef GENREMODEL_H
#define GENREMODEL_H

#include "sqlquerymodel.h"

class GenreModel : public SqlQueryModel
{
	Q_OBJECT
public:
	GenreModel(QObject *parent = 0);
public slots:
	virtual void refresh();
};

#endif // GENREMODEL_H
