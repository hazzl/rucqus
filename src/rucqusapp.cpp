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

    auto *engine = new QQmlApplicationEngine(this);
	QQmlContext *context = engine->rootContext();
	p_conf = new ConfigHandler(this);
	if (p_conf->isPointerHidden())
		setOverrideCursor(QCursor(Qt::BlankCursor));
	connect(p_conf, &ConfigHandler::hidePointerChanged, [=]{
		setOverrideCursor(QCursor(p_conf->isPointerHidden()? Qt::BlankCursor : Qt::ArrowCursor));});
	context->setContextProperty("config", p_conf);
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
	p_radioModel = new RadioModel(this);
	context->setContextProperty("radioModel", p_radioModel);

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
	QTimer::singleShot(0, this, &RucqusApp::findViews);
}

void RucqusApp::findViews()
{
	if (!p_loaderPage)
		return;
	QUrl source = p_loaderPage->property("source").toUrl();
	if ( source == QUrl("qrc:/MusicPlayer.qml"))
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
	} else if ( source == QUrl("qrc:/Radio.qml"))
	{
		p_playLView = nullptr;
        const auto *stationLView = p_loaderPage->findChild<const QQuickItem*>("stationListV");
		connect(stationLView, SIGNAL(clicked(int)), song, SLOT(setRadioStation(int)));
		song->setRadioStation(0);
	}
}

RucqusApp::~RucqusApp()
{
	p_db.close();
}

void RucqusApp::initDB()
{
    QSqlQuery q;
    q.exec("BEGIN EXCLUSIVE TRANSACTION");
    q.exec("CREATE TABLE IF NOT EXISTS config ("
           "key TEXT PRIMARY KEY ON CONFLICT REPLACE,"
           "value TEXT"
           ") WITHOUT ROWID");
    q.exec("CREATE TABLE IF NOT EXISTS songs ("
           "id	INTEGER PRIMARY KEY,"
           "album	INTEGER REFERENCES albums(id),"
           "trackno	INTEGER,"
           "name	TEXT,"
           "path	TEXT UNIQUE ON CONFLICT REPLACE,"
           "rating	INTEGER,"
           "timesplayed INTEGER,"
           "timesskipped INTEGER,"
           "lastplayed DATETIME,"
           "filectime DATETIME"
           ")");
    q.exec("CREATE UNIQUE INDEX IF NOT EXISTS pathidx ON songs (path)");
    q.exec("CREATE TABLE IF NOT EXISTS albums ("
           "id	INTEGER PRIMARY KEY,"
           "category INTEGER REFERENCES categories(id),"
           "name	TEXT,"
           "trackcount INTEGER,"
           "datetime DATE,"
           "lastplayed DATETIME"
           ")");
    q.exec("CREATE INDEX IF NOT EXISTS albumidx ON albums (name)");
    q.exec("CREATE TABLE IF NOT EXISTS artists ("
           "id	INTEGER PRIMARY KEY,"
           "name	TEXT"
           ")");
    q.exec("CREATE INDEX IF NOT EXISTS artistidx ON artists (name)");
    q.exec("CREATE TABLE IF NOT EXISTS song_artists ("
           "song_id	INTEGER REFERENCES songs(id),"
           "artist_id INTEGER REFERENCES artists(id),"
           "artist_type  INTEGER"
           ")");
    q.exec("CREATE TABLE IF NOT EXISTS genres ("
           "id	INTEGER PRIMARY KEY,"
           "name	TEXT"
           ")");
    q.exec("CREATE TABLE IF NOT EXISTS song_genres ("
           "song_id	INTEGER REFERENCES songs(id),"
           "genre_id INTEGER REFERENCES genres(id)"
           ")");
    q.exec("CREATE TABLE IF NOT EXISTS categories ("
           "id	INTEGER PRIMARY KEY,"
           "name	TEXT"
           ")");
    q.exec("COMMIT TRANSACTION");
}
