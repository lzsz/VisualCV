#ifndef VCVDATAMODEL_H
#define VCVDATAMODEL_H

#include <QObject>
#include <vector>

class QVCVData;

using namespace std;

class QVCVDataModel : public QObject
{
	Q_OBJECT
public:
	QVCVDataModel(QObject *parent = 0);
	~QVCVDataModel();

public:
	void Notify();
	
	void AddData(QVCVData *data);
	bool DeleteData(int index);
	bool DeleteData(const QString &file_name);

	//bool InsertData(int index,QVCVData *data);
	QVCVData* GetData(int index);
	QVCVData* GetData(const QString &file_name);
	int Count();
private:
	vector<QVCVData*> data_list;
};

#endif