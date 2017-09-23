#include "opencv2/imgproc.hpp"
#include "VCVData.h"
#include "CommandInclude/BilateralFilter.h"

QBilateralFilter::QBilateralFilter()
{
	initialization = false;
}

QBilateralFilter::~QBilateralFilter()
{

}

bool QBilateralFilter::SetParameter(const CommandParameter &para)
{
	if((para.sigmacolor<0)||(para.sigmaspace<0)||(para.ksize<0))
		return false;

	sigma_color = para.sigmacolor;
	sigma_space = para.sigmaspace;
	d = para.ksize;
	border_type = para.bordertype;

	return true;
}

void QBilateralFilter::undo()
{
	if(!initialization)
		return;

	connection_data->SetDisplayImage(original_image);
}

void QBilateralFilter::redo()
{
	if(!initialization)
		return;

	final_image.release();
	final_image.create(original_image.rows,original_image.cols,original_image.type());

	bilateralFilter(original_image,final_image,d,sigma_color,sigma_space,border_type);

	connection_data->SetDisplayImage(final_image);
}

