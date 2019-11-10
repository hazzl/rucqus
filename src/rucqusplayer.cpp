#include <QMediaPlaylist>
#include <QMediaContent>
#include <QModelIndex>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include "rucqusplayer.h"
#include "rucqusapp.h"
#include "plistmodel.h"

RucqusPlayer::RucqusPlayer(QObject *parent):
	QMediaPlayer(parent, QMediaPlayer::StreamPlayback)
{
	Q_CHECK_PTR (parent);
	p_plist = new QMediaPlaylist(this);
	setPlaylist(p_plist);
	lastSong = -1;
	setAudioRole(QAudio::MusicRole);
	connect (this, &RucqusPlayer::currentMediaChanged, this, &RucqusPlayer::onMediaChanged);
	connect (this, &RucqusPlayer::stateChanged, this, &RucqusPlayer::onStateChanged);
	connect (this ,static_cast<void(QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error), this, &RucqusPlayer::onError);
	connect (p_plist, &QMediaPlaylist::loaded, this, &QMediaPlayer::play);
}

void RucqusPlayer::replacePList()
{
	const PListModel *model = dynamic_cast<const RucqusApp*>(parent())->plistModel();
	int max_rows = std::min(model->rowCount(), 63);
	p_plist->clear();

	/* there is always only one physical column in the SqlQueryModel
	 * logical columns are implemented as roles for access by QML
	 */
	for (int row = 0; row < max_rows; ++row)
	{
		QModelIndex i = model->index(row,0);
		QString path = model->data(i, Qt::UserRole+2).toString();
		p_plist->addMedia(QMediaContent(QUrl::fromLocalFile(path)));
	}
}

void RucqusPlayer::setRadioStation(int id)
{
	const RadioModel *model = dynamic_cast<const RucqusApp*>(parent())->radioModel();
    const QUrl url(model->data(model->index(id, 0), Qt::UserRole+2).toString());
    const QString filename = url.fileName().toLower();

	p_plist->clear();
    if (filename.endsWith(".m3u") || filename.endsWith(".pls") || filename.endsWith(".m3u8"))
        p_plist->load(url);
    else
        p_plist->addMedia(QMediaContent(url));
}

QUrl RucqusPlayer::source()
{
	return currentMedia().canonicalUrl();
}

void RucqusPlayer::onMediaChanged()
{
	const int pListIndex = p_plist->currentIndex();
	const QQuickItem *pv = dynamic_cast<RucqusApp*>(parent())->playListV();
	if (!pv)
		return;
	const int lViewIndex = pv->property("currentIndex").toInt();
	if ( (getUid(lViewIndex) == lastSong)
			&& (pListIndex == lViewIndex + 1))
	{
		emit currentIndexChanged(pListIndex);
		recordTimesplayed(lastSong);
	}
	const int uid = getUid(pListIndex);
	lastSong = uid;
	if (state() == QMediaPlayer::PlayingState)
		recordLastplayed(uid);
	emit sourceChanged();
}

/*
 * get the uid of the song at an index
 */
int RucqusPlayer::getUid(int i)
{
	const PListModel *model = dynamic_cast<RucqusApp*>(parent())->plistModel();
	return model->data(model->index(i,0),Qt::UserRole).toInt();
}

void RucqusPlayer::recordLastplayed(const int uid)
{
	const uint time = QDateTime::currentDateTimeUtc().toTime_t();
	QSqlQuery q;
	q.prepare(QStringLiteral("UPDATE songs SET lastplayed=:time WHERE id=:uid;"));
	q.bindValue(":time",time);
	q.bindValue(":uid",uid);
	q.exec();
	q.finish();
	q.prepare(QStringLiteral("SELECT album FROM songs WHERE id=:uid;"));
	q.bindValue(":uid",uid);
	q.exec();
	if(q.first())
	{
		const int album = q.value(0).toInt();
		q.prepare(QStringLiteral("UPDATE albums SET lastplayed=:time WHERE id=:alb;"));
		q.bindValue(":time",time);
		q.bindValue(":alb",album);
		q.exec();
	}
}

void RucqusPlayer::recordTimesplayed(const int uid)
{
	QSqlQuery q;
	q.prepare(QStringLiteral("SELECT timesplayed FROM songs WHERE id=:uid;"));
	q.bindValue(":uid",uid);
	q.exec();
	q.first();
	const int timesplayed = q.value(0).toInt()+1;
	q.prepare(QStringLiteral("UPDATE songs SET timesplayed=:tp WHERE id=:uid;"));
	q.bindValue(":tp", timesplayed);
	q.bindValue(":uid", uid);
	q.exec();
}

void RucqusPlayer::onStateChanged(const QMediaPlayer::State state)
{
	if (!errorString().isEmpty())
		qDebug() << errorString().toLatin1().constData();
	switch (state) {
	case QMediaPlayer::StoppedState:
		if (error()==QMediaPlayer::FormatError) {
			play();
			qDebug("===============");
		}
		break;
	case QMediaPlayer::PlayingState:
		recordLastplayed(getUid(p_plist->currentIndex()));
		break;
	case QMediaPlayer::PausedState:
		break;
	}
}

void RucqusPlayer::onError(const QMediaPlayer::Error err)
{
	static quint8 counter = 0;
	qDebug() << err;
	if ((err == QMediaPlayer::FormatError) 
		&& (p_plist->previousIndex() >= 0)
		&& (++counter < 6)) 
	{
		p_plist->previous();
	}
}
