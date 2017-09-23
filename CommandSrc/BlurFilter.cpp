#include "opencv2/imgproc.hpp"
#include "VCVData.h"
#include "CommandInclude/BlurFilter.h"

using namespace cv;

QBlurFilter::QBlurFilter()
{
	initialization = false;
}

QBlurFilter::~QBlurFilter()
{
	original_image.release();
	final_image.release();
}

bool QBlurFilter::SetParameter(const CommandParameter &para)
{
	if(para.size.width!=para.size.height)
		return false;

	size = para.size;
	anchor = para.point;
	border_type = para.bordertype;

	if(anchor.x>=size.width)
		anchor.x = size.width-1;
	if(anchor.y>=size.height)
		anchor.y = size.height-1;

	return true;
}

void QBlurFilter::undo()
{
	if(!initialization)
		return;

	connection_data->SetDisplayImage(original_image);
}

void QBlurFilter::redo()
{
	if(!initialization)
		return;

	final_image.release();
	final_image.create(original_image.rows,original_image.cols,original_image.type());

	blur(original_image,final_image,size,anchor,border_type);

	connection_data->SetDisplayImage(final_image);
}
