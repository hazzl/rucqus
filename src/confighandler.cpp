#include "confighandler.h"
#include <QSqlQuery>

ConfigHandler::ConfigHandler(QObject *parent) : QObject(parent)
{
	QSqlQuery q("SELECT * from config;");
	q.exec();
	while (q.next())
		p_config[q.value(0).toString()]=q.value(1);
}

void ConfigHandler::set(QString key, QVariant value)
{
	p_config[key]=value;
	QSqlQuery q("INSERT OR REPLACE INTO config VALUES (:key,:value);");
	q.bindValue(":key",key);
	q.bindValue(":value",value);
	q.exec();
}
