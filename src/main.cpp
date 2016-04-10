#include "rucqusapp.h"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtSql/QSqlDatabase>

int main(int argc, char *argv[])
{
	RucqusApp app(argc, argv);
	return app.exec();
}
