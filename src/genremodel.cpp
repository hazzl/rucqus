#include "genremodel.h"
GenreModel::GenreModel(QObject *parent):
	SqlQueryModel(parent)
{
	q->prepare(QStringLiteral("SELECT id,name FROM genres ORDER BY name;"));
	q->exec();
	setQuery(*q);
}

void GenreModel::refresh()
{
	if (q->isActive()) q->finish();
	q->exec();
}
