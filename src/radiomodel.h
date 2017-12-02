#ifndef RADIOMODEL_H
#define RADIOMODEL_H

#include "sqlquerymodel.h"

class RadioModel : public SqlQueryModel
{
	Q_OBJECT
public:
	RadioModel(QObject *parent = nullptr);
public slots:
	virtual void refresh();
};

#endif // RADIOMODEL_H
