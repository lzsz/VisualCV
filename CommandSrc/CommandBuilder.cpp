#include "CommandInclude/VCVUndoCommand.h"
#include "CommandInclude/BlurFilter.h"
#include "CommandInclude/GaussianBlurFilter.h"
#include "CommandInclude/MedianBlurFilter.h"
#include "CommandInclude/BilateralFilter.h"
#include "CommandInclude/CustomFilter2D.h"
#include "CommandInclude/Erosion.h"
#include "CommandInclude/Dilation.h"
#include "CommandInclude/Threshold.h"
#include "CommandInclude/Morphology.h"
#include "CommandInclude/edgedetection.h"

#include "CommandInclude/CommandBuilder.h"


QCommandBuilder::QCommandBuilder()
{

}

QCommandBuilder::~QCommandBuilder()
{

}

QVCVUndoCommand* QCommandBuilder::CreateCommand(VCV_IMAGE_OPERATION operation)
{
	switch(operation)
	{
	case IMAGE_FILTER_BLUR:
		return new QBlurFilter;
	case IMAGE_FILTER_GAUSSIANBLUR:
		return new GaussianBlurFilter;
	case IMAGE_FILTER_MEDIANBLUR:
		return new QMedianBlurFilter;
	case IMAGE_FILTER_BILATERAL:
		return new QBilateralFilter;
	case IMAGE_FILTER_EROSION:
		return new QErosion;
	case IMAGE_FILTER_DILATION:
		return new QDilation;
    case IMAGE_FILTER_CUSTOM2D:
        return new QCustomFilter2D;
    case IMAGE_THRESHOLD_THRESHOLD:
        return new QThreshold;
    case IMAGE_THRESHOLD_ADAPTIVE:
        return new QAdaptiveThreshold;
    case IMAGE_MORPHOLOGY:
        return new QMorphology;
    case IMAGE_EDGE_LAPLACIAN:
        return new QLaplaction;
    case IMAGE_EDGE_SOBEL:
        return new QSobel;
    case IMAGE_EDGE_CANNY:
        return new QCanny;
	default:
		return NULL;
	}
}
