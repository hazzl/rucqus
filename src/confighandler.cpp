#include "confighandler.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#define SCALE QStringLiteral("scaleFactor")

ConfigHandler::ConfigHandler(QObject *parent) : QObject(parent)
{
	QSqlQuery q("SELECT * from config;");
	q.exec();
	while (q.next())
		p_config[q.value(0).toString()]=q.value(1);
	if (p_config[SCALE] < .5f)
		set(SCALE, QVariant(1.0f));
}

void ConfigHandler::set(const QString &key, const QVariant &value)
{
	if (p_config[key]!=value) {
			qDebug() << "setting " << key << " to " << value;
			p_config[key]=value;
			emit configChanged(key, value);
			QSqlQuery q;
			q.prepare(QStringLiteral("INSERT OR REPLACE INTO config VALUES (:key,:value);"));
			q.bindValue(":key",key);
			q.bindValue(":value",value);
			if (!q.exec())
			{
				qDebug() << q.lastError().text();
				qDebug() << q.executedQuery();
			}
	}
}

void ConfigHandler::setScaleFactor(const float newScaleFactor)
{
	float sf = scaleFactor();
	if ((newScaleFactor <= sf - 0.01f)
			|| (newScaleFactor >= sf + 0.01f))
	{
		set(SCALE, QVariant(newScaleFactor));
		emit scaleFactorChanged();
	}
}

void ConfigHandler::hidePointer(bool newState)
{
	if (newState != isPointerHidden())
	{
		set("hidePointer", QVariant(newState));
		emit hidePointerChanged();
	}
}
