#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

#include <QtCore/QObject>
#include <QHash>
#include <QVariant>
#include <QString>
#include <QDir>

class ConfigHandler : public QObject
{
	Q_OBJECT
    Q_PROPERTY (float scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged)
	Q_PROPERTY (bool hidePointer READ isPointerHidden WRITE hidePointer NOTIFY hidePointerChanged)
    Q_PROPERTY (QString libraryPath READ libraryPath WRITE setLibraryPath NOTIFY libraryPathChanged)
public:
	explicit ConfigHandler(QObject *parent = nullptr);
	const QVariant get(const QString &key) const {return p_config[key];}
    float scaleFactor() const { return p_config[QStringLiteral("scaleFactor")].toFloat();}
	void setScaleFactor(const float newScaleFactor);
	bool isPointerHidden() const {return p_config["hidePointer"].toBool();}
	void hidePointer(bool newState);
	void set(const QString &key, const QVariant &value);
    const QString libraryPath() const {return p_config["libraryPath"].toString();}
    void setLibraryPath(const QString &path);
signals:
	void configChanged(const QString &key, const QVariant &value);
	void scaleFactorChanged();
	void hidePointerChanged();
    void libraryPathChanged(const QString &path);
private:
	QHash <QString, QVariant>p_config;
private slots:
    void writeBack(const QString &key, const QVariant &value);
};

#endif // CONFIGHANDLER_H
