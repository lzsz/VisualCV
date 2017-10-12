#ifndef SYSTEMDEFINE_H
#define SYSTEMDEFINE_H

#include <QVariant>
#include <vector>

#include "opencv/cv.h"
#include "opencv/cxcore.h"

#include "Utility.h"

using namespace std;
using namespace cv;

enum VCV_IMAGE_OPERATION
{
	IMAGE_NONE,
	IMAGE_NEW,
	IMAGE_LOAD,
	IMAGE_FILTER_BLUR,
	IMAGE_FILTER_GAUSSIANBLUR,
	IMAGE_FILTER_MEDIANBLUR,
	IMAGE_FILTER_BILATERAL,
	IMAGE_FILTER_CUSTOM2D,
	IMAGE_FILTER_EROSION,
	IMAGE_FILTER_DILATION
};

struct CommandParameter
{
	double sigmacolor;
	double sigmaspace;
	double scale;
	double delta;
	int iterations;
	int ksize;
	int bordertype;
	int depth;
	Size size;
	Point point;
	Mat kernel;
};


#endif
