#include "plistmodel.h"
#include "rucqusapp.h"

PListModel::PListModel(QObject *parent):
	SqlQueryModel(parent)
{
	refresh();
}

void PListModel::refresh()
{
    const RucqusApp *app = dynamic_cast<RucqusApp*>(parent());
	int genre = getCurrentIndex(app->genreList(), app->genreModel());
	int artist = getCurrentIndex(app->artistList(), app->artistModel());
	int album = getCurrentIndex(app->albumList(), app->albumModel());

	if (q->isActive()) q->finish();
	QString query = QStringLiteral("SELECT DISTINCT s.id,s.name,s.path FROM songs s "
				       "INNER JOIN albums b on s.album=b.id "
				       "INNER JOIN song_genres g ON g.song_id=s.id "
				       "INNER JOIN song_artists a ON a.song_id=s.id");
	query.reserve(273);
	QString where;
	if (genre >= 0) where.append("g.genre_id=:gid");
	if (artist >= 0)
	{
		if (!where.isEmpty()) where.append(" AND ");
		where.append("a.artist_id=:aid");
	}
	if (album >= 0)
	{
		if (!where.isEmpty()) where.append(" AND ");
		where.append("s.album=:alb");
	}
	if (!where.isEmpty())
	{
		query.append(" WHERE ");
		query.append(where);
	}
	query.append(" ORDER BY b.lastplayed,s.album,s.trackno;");

	q->prepare(query);
	if (genre >= 0) q->bindValue(":gid", genre);
	if (artist >= 0) q->bindValue(":aid", artist);
	if (album >= 0 ) q->bindValue(":alb", album);
	q->exec();
	setQuery(*q);
	emit newData();
}
