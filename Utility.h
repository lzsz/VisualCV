#ifndef UTILITY_H
#define UTILITY_H


#include <QImage>
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

using namespace cv;

QImage* MatToQImage(const Mat &mat);

Mat* QImageToMat(QImage &image);


#endif