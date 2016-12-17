#ifndef RADIOMODEL_H
#define RADIOMODEL_H

#include "sqlquerymodel.h"

class RadioModel : public SqlQueryModel
{
	Q_OBJECT
public:
	RadioModel(QObject *parent = 0);
public slots:
	virtual void refresh();
};

#endif // RADIOMODEL_H
