#ifndef RUCQUSPLAYER_H
#define RUCQUSPLAYER_H
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaContent>
#include <QUrl>

class RucqusPlayer : public QMediaPlayer
{
	Q_OBJECT
	Q_PROPERTY(QUrl source READ source NOTIFY sourceChanged STORED false FINAL)
public:
	explicit	RucqusPlayer(QObject *parent = nullptr);
	QUrl		source();
signals:
	void		sourceChanged();
	void		currentIndexChanged(int index);
public slots:
	void		replacePList();
	void		setRadioStation(int id);
private:
	QMediaPlaylist	*p_plist;
	int			getUid(int index);
	int			lastSong;
    void		recordLastplayed(const int uid);
    void		recordTimesplayed(const int uid);
private slots:
	void		onMediaChanged();
	void		onStateChanged(const QMediaPlayer::State state);
	void		onError(const QMediaPlayer::Error err);
};
#endif // RUCQUSPLAYER_H
