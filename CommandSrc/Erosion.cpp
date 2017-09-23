#include "opencv2/imgproc.hpp"
#include "VCVData.h"
#include "CommandInclude/Erosion.h"

QErosion::QErosion()
{

}

QErosion::~QErosion()
{

}

bool QErosion::SetParameter(const CommandParameter &para)
{
	if(para.iterations%2!=1)
		return false;

	iterations = para.iterations;
	anchor = para.point;

	return true;
}

void QErosion::undo()
{
	if(!initialization)
		return;

	connection_data->SetDisplayImage(original_image);	
}

void QErosion::redo()
{
	if(!initialization)
		return;

	if(anchor.x>=connection_data->width())
		anchor.x = connection_data->width()-1;
	if(anchor.y>=connection_data->height())
		anchor.y = connection_data->height()-1;

	final_image.release();
	final_image.create(original_image.rows,original_image.cols,original_image.type());
	
	Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(iterations, iterations),anchor );

	erode(original_image,final_image,kernel,anchor,iterations);

	connection_data->SetDisplayImage(final_image);
}
