#include "opencv2/imgproc.hpp"
#include "VCVData.h"
#include "CommandInclude/MedianBlurFilter.h"

QMedianBlurFilter::QMedianBlurFilter()
{
		initialization = false;
}

QMedianBlurFilter::~QMedianBlurFilter()
{

}

bool QMedianBlurFilter::SetParameter(const CommandParameter &para)
{
	if(para.ksize%2!=1)
		return false;

	ksize = para.ksize;
    return true;
}

void QMedianBlurFilter::undo()
{
	if(!initialization)
		return;

	connection_data->SetDisplayImage(original_image);	
}

void QMedianBlurFilter::redo()
{
	if(!initialization)
		return;

	final_image.release();
	final_image.create(original_image.rows,original_image.cols,original_image.type());

	medianBlur(original_image,final_image,ksize);

	connection_data->SetDisplayImage(final_image);
}
