#include "downloader.h"

#include <QString>
#include <QFileInfo>
#include <QFile>

Downloader::Downloader(QObject *parent) : QObject(parent)
{
	connect(&manager, &QNetworkAccessManager::finished, this, &Downloader::finished);
}

void Downloader::get(const QUrl &url)
{
	QNetworkRequest req(url);
	currentDownload = manager.get(req);
}

QString Downloader::saveFileName(const QUrl &url)
{
	QString path = url.path();
	QString basename = QFileInfo(path).fileName();

	if (basename.isEmpty())
		basename = "download";

	return QStringLiteral("/tmp/")+basename;
}

bool Downloader::saveToDisk(const QString &filename, QIODevice *data)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
			qCritical("Could not open %s for writing: %s\n",
					qPrintable(filename),
					qPrintable(file.errorString()));
			return false;
		}

	file.write(data->readAll());
	file.close();
	QUrl url = QUrl::fromLocalFile(filename);
	emit downloaded(url);
	return true;
}

void Downloader::finished(QNetworkReply *reply)
{
	QUrl url = reply->url();
		if (reply->error()) {
			qWarning("Download of %s failed: %s\n",
					url.toEncoded().constData(),
					qPrintable(reply->errorString()));
		} else {
			QString filename = saveFileName(url);
			saveToDisk(filename, reply);
		}
}
