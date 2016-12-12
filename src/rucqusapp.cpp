#include "rucqusapp.h"
#include <QObject>
#include <QQmlContext>
#include <QSqlQuery>
#include <QSqlError>
#include <QUrl>
#include <QQmlApplicationEngine>
#include <QCursor>
#include <QTimer>
#include <QDebug>

RucqusApp::RucqusApp(int & argc, char ** argv)
	: QGuiApplication(argc, argv)
	, p_genreLView{nullptr}
	, p_artistLView{nullptr}
	, p_albumLView{nullptr}
	, p_playLView{nullptr}
	, p_loaderPage{nullptr}
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
	connect(p_loaderPage, SIGNAL(loaded()), this, SLOT(initTimer()));
	if(p_loaderPage->property("source").toUrl() == QUrl())
		p_loaderPage->setProperty("source",QUrl("qrc:/MusicPlayer.qml"));
}

void RucqusApp::initTimer()
{
	// delay calling findViews until the end of this event loop
	// otherwise we get old references when they should be overwritten
	QTimer::singleShot(0, this, SLOT(findViews()));
}

void RucqusApp::findViews()
{
	if (p_loaderPage && p_loaderPage->property("source").toUrl() == QUrl("qrc:/MusicPlayer.qml"))
	{
		p_genreLView = p_loaderPage->findChild<const QQuickItem*>("genreLView");
		p_artistLView = p_loaderPage->findChild<const QQuickItem*>("artistLView");
		p_albumLView = p_loaderPage->findChild<const QQuickItem*>("albumLView");
		p_playLView = p_loaderPage->findChild<const QQuickItem*>("pListView");

		connect(p_genreLView, SIGNAL(currentIndexChanged()), p_artistModel, SLOT(refresh()));
		connect(p_genreLView, SIGNAL(currentIndexChanged()), p_albumModel, SLOT(refresh()));
		connect(p_genreLView, SIGNAL(currentIndexChanged()), p_plistModel, SLOT(refresh()));
		connect(p_artistLView, SIGNAL(currentIndexChanged()), p_albumModel, SLOT(refresh()));
		connect(p_artistLView, SIGNAL(currentIndexChanged()), p_plistModel, SLOT(refresh()));
		connect(p_albumLView, SIGNAL(currentIndexChanged()), p_plistModel, SLOT(refresh()));
		connect(p_playLView, SIGNAL(clicked(int)), song->playlist(), SLOT(setCurrentIndex(int)));
		song->replacePList();
	}
}

RucqusApp::~RucqusApp()
{
	p_db.close();
}
