#include "opencv2/imgproc.hpp"
#include "VCVData.h"
#include "CommandInclude/Dilation.h"

QDilation::QDilation()
{

}

QDilation::~QDilation()
{

}

bool QDilation::SetParameter(const CommandParameter &para)
{
	if(para.iterations%2!=1)
		return false;

	iterations = para.iterations;
	anchor = para.point;

	return true;
}

void QDilation::undo()
{
	if(!initialization)
		return;

	connection_data->SetDisplayImage(original_image);	
}

void QDilation::redo()
{
	if(!initialization)
		return;

	if(anchor.x>=connection_data->width())
		anchor.x = connection_data->width()-1;
	if(anchor.y>=connection_data->height())
		anchor.y = connection_data->height()-1;

	final_image.release();
	final_image.create(original_image.rows,original_image.cols,original_image.type());
	
	Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(iterations, iterations),anchor);

	dilate(original_image,final_image,kernel,anchor,iterations);

	connection_data->SetDisplayImage(final_image);
}
