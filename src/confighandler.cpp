#include "confighandler.h"
#include "rucqusapp.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#define SCALE QStringLiteral("scaleFactor")

ConfigHandler::ConfigHandler(QObject *parent) : QObject(parent)
{
	QSqlQuery q("SELECT * from config;");
    if(q.exec())    // returns true on no error
    {
        while (q.next())
            p_config[q.value(0).toString()]=q.value(1);
    } else {                     // there was an error, probably missing tables
        dynamic_cast<RucqusApp*>(parent)->initDB();
    }
    if (p_config[SCALE] < .5f)  // if there is no value associated with SCALE, 0.0 is returned
        set(SCALE, QVariant(1.0f));
}

void ConfigHandler::set(const QString &key, const QVariant &value)
{
	if (p_config[key]!=value) {
			qDebug() << "setting " << key << " to " << value;
			p_config[key]=value;
            emit configChanged(key, value);
            writeBack(key, value);
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

void ConfigHandler::writeBack(const QString &key, const QVariant &value)
{
    QSqlQuery q;
    q.prepare(QStringLiteral("INSERT OR REPLACE INTO config VALUES (:key,:value);"));
    q.bindValue(":key",key);
    q.bindValue(":value",value);
    if (!q.exec())
    {
        qDebug() << q.executedQuery();
        qDebug() << q.lastError().text();
    }
}
