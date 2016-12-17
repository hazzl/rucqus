#include "radiomodel.h"
RadioModel::RadioModel(QObject *parent):
	SqlQueryModel(parent)
{
	q->prepare(QStringLiteral("SELECT id,name,url FROM radio ORDER BY name;"));
	q->exec();
	setQuery(*q);
}

void RadioModel::refresh(){}
