#include "opencv2/imgproc.hpp"
#include "VCVData.h"
#include "CommandInclude/CustomFilter2D.h"

QCustomFilter2D::QCustomFilter2D()
{

}

QCustomFilter2D::~QCustomFilter2D()
{

}

bool QCustomFilter2D::SetParameter(const CommandParameter &para)
{
	border_type = para.bordertype;
    depth = para.depth;
	delta = para.delta;
	anchor = para.point;
	kernel = para.kernel;

	return true;
}

void QCustomFilter2D::undo()
{
	if(!initialization)
		return;

	connection_data->SetDisplayImage(original_image);
}

void QCustomFilter2D::redo()
{
	if(!initialization)
		return;

	final_image.release();
	final_image.create(original_image.rows,original_image.cols,original_image.type());

	filter2D(original_image,final_image,depth,kernel,anchor,delta,border_type);

	connection_data->SetDisplayImage(final_image);
}
