#include "rucqusapp.h"
#include <QObject>
#include <QQmlContext>
#include <QSqlQuery>
#include <QSqlError>
#include <QUrl>
#include <QQmlApplicationEngine>
#include <QCursor>
#include <QDebug>

RucqusApp::RucqusApp(int & argc, char ** argv)
	: QGuiApplication(argc, argv)
	, p_loaderPage{nullptr}
//	, p_genreLView{nullptr}
//	, p_artistLView{nullptr}
//	, p_albumLView{nullptr}
//	, p_playLView{nullptr}
{
	p_db = QSqlDatabase::addDatabase("QSQLITE");
	p_db.setDatabaseName("rucqus.sqlite");
	if (!p_db.open())
	{
		QSqlError err = p_db.lastError();
		qCritical(err.driverText().toUtf8());
		qFatal(err.databaseText().toUtf8());
	}
	p_conf = new ConfigHandler(this);
	if (p_conf->get("hidePointer").toBool())
		setOverrideCursor(QCursor(Qt::BlankCursor));
	QQmlApplicationEngine *engine = new QQmlApplicationEngine(this);
	QQmlContext *context = engine->rootContext();
	song = new RucqusPlayer(this);
	context->setContextProperty("song", song);
	p_genreModel = new GenreModel(this);
	context->setContextProperty("genreModel", p_genreModel);
	p_artistModel = new ArtistModel(this);
	context->setContextProperty("artistModel", p_artistModel);
	p_albumModel = new AlbumModel(this);
	context->setContextProperty("albumModel", p_albumModel);
	p_plistModel = new PListModel(this);
	context->setContextProperty("plistModel", p_plistModel);

	connect(engine, &QQmlApplicationEngine::objectCreated, this, &RucqusApp::setupLoader);
	connect(p_plistModel, &PListModel::newData, song, &RucqusPlayer::replacePList);

	engine->load(QUrl(QStringLiteral("qrc:/main.qml")));
}

void RucqusApp::setupLoader(QObject *object)
{
	p_loaderPage = object->findChild<QQuickItem *>("loaderPage");
	Q_CHECK_PTR(p_loaderPage);
	connect(p_loaderPage, SIGNAL(loaded()), this, SLOT(findViews()));
	if(p_loaderPage->property("source").toUrl() == QUrl())
		p_loaderPage->setProperty("source",QUrl("qrc:/MusicPlayer.qml"));
}

void RucqusApp::findViews()
{
	if (p_loaderPage && p_loaderPage->property("source").toUrl() == QUrl("qrc:/MusicPlayer.qml"))
	{
			const QQuickItem *gv = genreList();
			connect(gv, SIGNAL(currentIndexChanged()), p_artistModel, SLOT(refresh()));
			connect(gv, SIGNAL(currentIndexChanged()), p_albumModel, SLOT(refresh()));
			connect(gv, SIGNAL(currentIndexChanged()), p_plistModel, SLOT(refresh()));
			const QQuickItem *arv = artistList();
			connect(arv, SIGNAL(currentIndexChanged()), p_albumModel, SLOT(refresh()));
			connect(arv, SIGNAL(currentIndexChanged()), p_plistModel, SLOT(refresh()));
			connect(albumList(), SIGNAL(currentIndexChanged()), p_plistModel, SLOT(refresh()));
			connect(playListV(), SIGNAL(clicked(int)), song->playlist(), SLOT(setCurrentIndex(int)));
			song->replacePList();
	}
}

RucqusApp::~RucqusApp()
{
	p_db.close();
}
