#include "albummodel.h"
#include "rucqusapp.h"
#include <QQuickItem>
#include <QString>

AlbumModel::AlbumModel(QObject *parent):
	SqlQueryModel(parent)
{
	refresh();
}

void AlbumModel::refresh()
{
	const RucqusApp *app = static_cast<RucqusApp*>(parent());
	int genre = getCurrentIndex(app->genreList(), app->genreModel());
	int artist = getCurrentIndex(app->artistList(), app->artistModel());

	if (q->isActive()) q->finish();
    QString query = QStringLiteral("SELECT DISTINCT al.id,al.name,c.name AS category FROM "
				       "song_artists sa INNER JOIN songs s ON sa.song_id = s.id "
				       "INNER JOIN song_genres g ON g.song_id = s.id "
				       "INNER JOIN albums al ON al.id = s.album "
				       "LEFT OUTER JOIN categories c ON c.id=al.category");
	QString where;
	query.reserve(316);

	if (genre >= 0)
		where.append("g.genre_id = :gid");
	if (artist >= 0)
	{
		if (!where.isEmpty())
			where.append(" AND ");
		where.append("sa.artist_id = :aid");
	}
	if (!where.isEmpty())
	{
		query.append(" WHERE ");
		query.append(where);
	}
    query.append(" ORDER BY c.id DESC, al.name;");

	q->prepare(query);
	if (genre >= 0) q->bindValue(":gid", genre);
	if (artist >= 0) q->bindValue(":aid", artist);
	q->exec();
	setQuery(*q);
}
