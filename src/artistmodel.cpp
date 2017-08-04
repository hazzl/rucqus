#include "artistmodel.h"
#include "rucqusapp.h"
#include <QQuickItem>

ArtistModel::ArtistModel(QObject *parent):
	SqlQueryModel(parent)
{
	refresh();
}

void ArtistModel::refresh()
{
	const RucqusApp *app = static_cast<RucqusApp*>(parent());
	int genre = getCurrentIndex(app->genreList(), app->genreModel());

	if (q->isActive()) q->finish();
	QString query = QStringLiteral("SELECT DISTINCT a.id,a.name FROM ");
	query.reserve(219);

	if (genre == -1)
		query.append("artists a ");
	else
		query.append(QStringLiteral("song_genres g INNER JOIN songs s ON g.song_id = s.id "
			     "INNER JOIN song_artists sa USING (song_id) "
			     "INNER JOIN artists a ON a.id = sa.artist_id "
				 "WHERE g.genre_id=:gid "));
	query.append(QStringLiteral("ORDER BY upper(a.name);"));
	q->prepare(query);
	if (genre >= 0) q->bindValue(":gid", genre);
	q->exec();
	setQuery(*q);
}
