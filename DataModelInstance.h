#ifndef QDATAMODELINSTANCE_H
#define QDATAMODELINSTANCE_H

#include <QObject>

class QVCVDataModel;

class QDataModelInstance : public QObject
{
	Q_OBJECT
public:
	static QVCVDataModel* Instance();
	
private:
	QDataModelInstance();

	static QVCVDataModel *data_model;
};

#endif