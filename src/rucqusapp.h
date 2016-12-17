#ifndef RUCQUSAPP_H
#define RUCQUSAPP_H
#include <QSqlDatabase>
#include <QQuickItem>

#include <QGuiApplication>
#include "artistmodel.h"
#include "genremodel.h"
#include "albummodel.h"
#include "plistmodel.h"
#include "radiomodel.h"
#include "rucqusplayer.h"
#include "confighandler.h"

class RucqusApp : public QGuiApplication
{
	Q_OBJECT
public:
	RucqusApp(int & argc, char ** argv);
	~RucqusApp();
	const QSqlDatabase *db() const {return &p_db;}
	const GenreModel *genreModel() const {return p_genreModel;}
	const QQuickItem *genreList() const {return p_genreLView;}
	const ArtistModel *artistModel() const {return p_artistModel;}
	const QQuickItem *artistList() const {return p_artistLView;}
	const AlbumModel *albumModel() const {return p_albumModel;}
	const QQuickItem *albumList() const {return p_albumLView;}
	const PListModel *plistModel() const {return p_plistModel;}
	const RadioModel *radioModel() const {return p_radioModel;}
	const QQuickItem *playListV() const {return p_playLView;}
	const RucqusPlayer *player() const {return song;}
	const ConfigHandler *conf() const {return p_conf;}
private slots:
	void setupLoader(QObject *object);
	void initTimer();
	void findViews();
private:
	QSqlDatabase	p_db;
	RucqusPlayer	*song;
	QQuickItem	const *p_genreLView, *p_artistLView, *p_albumLView, *p_playLView;
	QQuickItem	*p_loaderPage;
	GenreModel	*p_genreModel;
	ArtistModel	*p_artistModel;
	AlbumModel	*p_albumModel;
	PListModel	*p_plistModel;
	RadioModel	*p_radioModel;
	ConfigHandler	*p_conf;
};
#endif // RUCQUSAPP_H
