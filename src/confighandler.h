#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

#include <QtCore/QObject>
#include <QHash>
#include <QVariant>
#include <QString>

class ConfigHandler : public QObject
{
	Q_OBJECT
	Q_PROPERTY (float scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged)
	Q_PROPERTY (bool hidePointer READ isPointerHidden WRITE hidePointer NOTIFY hidePointerChanged)
public:
	explicit ConfigHandler(QObject *parent = 0);
	const QVariant get(const QString &key) const {return p_config[key];}
	float scaleFactor() const { return p_config[QStringLiteral("scaleFactor")].toFloat();}
	void setScaleFactor(const float newScaleFactor);
	bool isPointerHidden() const {return p_config["hidePointer"].toBool();}
	void hidePointer(bool newState);
	void set(const QString &key, const QVariant &value);
signals:
	void configChanged(const QString &key, const QVariant &value);
	void scaleFactorChanged();
	void hidePointerChanged();
private:
	QHash <QString, QVariant>p_config;
};

#endif // CONFIGHANDLER_H
