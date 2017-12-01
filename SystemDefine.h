#ifndef SYSTEMDEFINE_H
#define SYSTEMDEFINE_H

#include <QVariant>
#include <vector>

#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv2/imgproc/imgproc.hpp"

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
    IMAGE_FILTER_DILATION,
    IMAGE_MORPH_OPEN,

    IMAGE_THRESHOLD_THRESHOLD,
    IMAGE_THRESHOLD_ADAPTIVE
};

class CommandParameter
{

};

class CommandParameter_Filter : public CommandParameter
{
public:
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

class CommandParameter_Threshold : public CommandParameter
{
public:
    double thresh;
    double max;
    double constant;
    int block_size;
    cv::ThresholdTypes threshold_type;
    cv::AdaptiveThresholdTypes adaptive_type;
};

class CommandParameter_Morphology : public CommandParameter
{
public:
    int shape;
    int ksize;
    Point kernelAnchor;
    Point anchor;
    int iteration;
    BorderTypes borderType;
    int tp1;
    int tp2;
    int tp3;
    int tp4;
};

#endif
