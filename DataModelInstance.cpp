#include "VCVDataModel.h"
#include "DataModelInstance.h"

QVCVDataModel* QDataModelInstance::data_model = NULL;

QVCVDataModel* QDataModelInstance::Instance()
{
	if(data_model==NULL)
		data_model = new QVCVDataModel;

	return data_model;
}
