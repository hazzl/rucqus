#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QtCore/QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
class Downloader : public QObject
{
	Q_OBJECT
public:
	explicit Downloader(QObject *parent = nullptr);
public slots:
	void finished(QNetworkReply *reply);
	void get(const QUrl &url);
signals:
	void downloaded(const QUrl &file);
private:
	QString saveFileName(const QUrl &url);
	QString filename;
	bool saveToDisk(const QString &filename, QIODevice *data);
	QNetworkAccessManager manager;
	QNetworkReply *currentDownload;
};

#endif // DOWNLOADER_H
