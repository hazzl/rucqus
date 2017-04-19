#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

#include <QtCore/QObject>
#include <QHash>
#include <QVariant>
#include <QString>

class ConfigHandler : public QObject
{
	Q_OBJECT
public:
	explicit ConfigHandler(QObject *parent = 0);
	const QVariant get(const QString &key) const {return p_config[key];}
	void set(const QString &key, const QVariant &value);
private:
	QHash <QString, QVariant>p_config;
};

#endif // CONFIGHANDLER_H
