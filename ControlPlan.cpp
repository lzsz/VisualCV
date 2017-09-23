#include <QRadioButton>
#include "CommandInclude/BlurFilter.h"

#include "ControlPlan.h"

QControlPlan::QControlPlan()
{
    ui.setupUi(this);
	CreateBorderType();

	Disable();

	connect(ui.dsp_delta,SIGNAL(valueChanged(double)),this,SLOT(ValueChange()));
	connect(ui.dsp_scale,SIGNAL(valueChanged(double)),this,SLOT(ValueChange()));
	connect(ui.dsp_sigmacolor,SIGNAL(valueChanged(double)),this,SLOT(ValueChange()));
	connect(ui.dsp_sigmaspace,SIGNAL(valueChanged(double)),this,SLOT(ValueChange()));
	connect(ui.dsp_sigmax,SIGNAL(valueChanged(double)),this,SLOT(ValueChange()));
	connect(ui.dsp_sigmay,SIGNAL(valueChanged(double)),this,SLOT(ValueChange()));

	connect(ui.sp_height,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
	connect(ui.sp_width,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
	connect(ui.sp_x,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
	connect(ui.sp_y,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
	connect(ui.sp_iterations,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
	connect(ui.sp_ksize,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));

	connect(ui.pb_Ok,SIGNAL(clicked(bool)),this,SLOT(PushOk()));
	connect(ui.pb_Cancel,SIGNAL(clicked(bool)),this,SLOT(PushCancel()));
}

QControlPlan::~QControlPlan()
{

}

double QControlPlan::GetSigmaColor()
{
    return ui.dsp_sigmacolor->value();
}

double QControlPlan::GetSigmaSpace()
{
    return ui.dsp_sigmaspace->value();
}

double QControlPlan::GetScale()
{
    return ui.dsp_scale->value();
}

double QControlPlan::GetDelta()
{
    return ui.dsp_delta->value();
}

Size QControlPlan::GetSize()
{
    return Size(ui.sp_width->value(),ui.sp_height->value());
}

Point QControlPlan::GetPoint()
{
    return Point(ui.sp_x->value(),ui.sp_y->value());
}

int QControlPlan::GetIterations()
{
    return ui.sp_iterations->value();
}

int QControlPlan::GetKSize()
{
    return ui.sp_ksize->value();
}

int QControlPlan::GetBorderType()
{
	return BORDER_DEFAULT;	//temp

	if(rb_default->isChecked())
		return BORDER_DEFAULT;
		
	if(rb_wrap->isChecked())
		return BORDER_WRAP;

	if(rb_replicate->isChecked())
		return BORDER_REPLICATE;

	if(rb_reflect->isChecked())
		return BORDER_REFLECT;

	if(rb_replicate101->isChecked())
		return BORDER_REFLECT_101;
		
	if(rb_reflect101->isChecked())
		return BORDER_REFLECT101;

	if(rb_constant->isChecked())
		return BORDER_CONSTANT;

	if(rb_transparent->isChecked())
		return BORDER_TRANSPARENT;

	if(rb_isolated->isChecked())
		return BORDER_ISOLATED;

	return BORDER_DEFAULT;
}

void QControlPlan::BeginOperation(VCV_IMAGE_OPERATION operation)
{
	switch(operation)
	{
	case IMAGE_FILTER_BLUR:
		image_operation = operation;
		EnableBlur();
		break;
	case IMAGE_FILTER_GAUSSIANBLUR:
		image_operation = operation;
		EnableGaussianBlur();
		break;
	case IMAGE_FILTER_MEDIANBLUR:
		image_operation = operation;
		EnableMedianBlur();
		break;
	case IMAGE_FILTER_BILATERAL:
		image_operation = operation;
		EnableBilateral();
		break;
	case IMAGE_FILTER_EROSION:
		image_operation = operation;
		EnableErosion();
		break;
	case IMAGE_FILTER_DILATION:
		image_operation = operation;
		EnableErosion();
		break;
	default:
		break ;
	}
}

void QControlPlan::EndOperation(VCV_IMAGE_OPERATION operation)
{
	switch(operation)
	{
	case IMAGE_FILTER_BLUR:
		DisableBlur();
		break;
	case IMAGE_FILTER_GAUSSIANBLUR:
		DisableGaussianBlur();
		break;
	case IMAGE_FILTER_MEDIANBLUR:
		DisableMedianBlur();
		break;
	case IMAGE_FILTER_BILATERAL:
		DisableBilateral();
		break;
	case IMAGE_FILTER_EROSION:
		DisableErosion();
		break;
	case IMAGE_FILTER_DILATION:
		DisableErosion();
		break;
	default:
		break ;
	}

	image_operation = IMAGE_NONE;
}

void QControlPlan::ValueChange()
{
	GetAllParameter();

	emit ParameterChange(command_para);
}

void QControlPlan::PushOk()
{
	EndOperation(image_operation);

	GetAllParameter();

	emit ControlPlanOk(command_para);
}

void QControlPlan::PushCancel()
{
	EndOperation(image_operation);

	GetAllParameter();

	emit ControlPlanCancel(command_para);
}

void QControlPlan::CreateBorderType()
{
	rb_default = new QRadioButton(tr("BORDER_DEFAULT"));
	rb_wrap = new QRadioButton(tr("BORDER_WRAP"));
	rb_replicate = new QRadioButton(tr("BORDER_DEFAULT"));
	rb_reflect = new QRadioButton(tr("BORDER_REFLECT"));
	rb_replicate101 = new QRadioButton(tr("BORDER_REFLECT_101"));
	rb_reflect101 = new QRadioButton(tr("BORDER_REFLECT101"));
	rb_constant = new QRadioButton(tr("BORDER_CONSTANT"));
	rb_transparent = new QRadioButton(tr("BORDER_TRANSPARENT"));
	rb_isolated = new QRadioButton(tr("BORDER_ISOLATED"));
	QGridLayout *gridbox = new QGridLayout;

	gridbox->addWidget(rb_default,0,0);
	gridbox->addWidget(rb_wrap,1,0);
	gridbox->addWidget(rb_replicate,2,0);
	gridbox->addWidget(rb_reflect,3,0);
	gridbox->addWidget(rb_replicate101,4,0);
	gridbox->addWidget(rb_reflect101,0,1);
	gridbox->addWidget(rb_constant,1,1);
	gridbox->addWidget(rb_transparent,2,1);
	gridbox->addWidget(rb_isolated,3,1);

	ui.gb_BoardType->setLayout(gridbox);

}

void QControlPlan::GetAllParameter()
{
	command_para.delta = GetDelta();
	command_para.iterations =GetIterations();
	command_para.ksize = GetKSize();
	command_para.point = GetPoint();
	command_para.scale = GetScale();
	command_para.sigmacolor = GetSigmaColor();
	command_para.sigmaspace = GetSigmaSpace();
	command_para.size = GetSize();
	command_para.bordertype = GetBorderType();
}

void QControlPlan::Disable()
{
	ui.dsp_delta->setEnabled(false);
	ui.dsp_scale->setEnabled(false);
	ui.dsp_sigmacolor->setEnabled(false);
	ui.dsp_sigmaspace->setEnabled(false);
	ui.dsp_sigmax->setEnabled(false);
	ui.dsp_sigmay->setEnabled(false);
	ui.sp_height->setEnabled(false);
	ui.sp_width->setEnabled(false);
	ui.sp_x->setEnabled(false);
	ui.sp_y->setEnabled(false);
	ui.sp_iterations->setEnabled(false);
	ui.sp_ksize->setEnabled(false);
	ui.gb_BoardType->setEnabled(false);
	ui.pb_Ok->setEnabled(false);
	ui.pb_Cancel->setEnabled(false);
}

void QControlPlan::EnableBlur()
{
	Disable();

	ui.sp_width->setEnabled(true);
	ui.sp_height->setEnabled(true);
	ui.sp_x->setEnabled(true);
	ui.sp_y->setEnabled(true);
	ui.pb_Ok->setEnabled(true);
	ui.pb_Cancel->setEnabled(true);
	ui.gb_BoardType->setEnabled(true);
	ui.sp_width->setMinimum(1);
	ui.sp_height->setMinimum(1);
	ui.sp_width->setValue(0);
	ui.sp_height->setValue(0);
	ui.sp_x->setMinimum(-1);
	ui.sp_y->setMinimum(-1);
	ui.sp_x->setValue(-1);
	ui.sp_y->setValue(-1);
	
	connect(ui.sp_width,SIGNAL(valueChanged(int)),ui.sp_height,SLOT(setValue(int)));
	connect(ui.sp_height,SIGNAL(valueChanged(int)),ui.sp_width,SLOT(setValue(int)));
}

void QControlPlan::DisableBlur()
{
	Disable();

	disconnect(ui.sp_width,SIGNAL(valueChanged(int)),ui.sp_height,SLOT(setValue(int)));
	disconnect(ui.sp_height,SIGNAL(valueChanged(int)),ui.sp_width,SLOT(setValue(int)));
}

void QControlPlan::EnableGaussianBlur()
{
	Disable();

	ui.dsp_sigmacolor->setEnabled(true);
	ui.dsp_sigmaspace->setEnabled(true);
	ui.sp_width->setEnabled(true);
	ui.sp_height->setEnabled(true);
	ui.pb_Ok->setEnabled(true);
	ui.pb_Cancel->setEnabled(true);
	ui.gb_BoardType->setEnabled(true);
	ui.dsp_sigmacolor->setMinimum(0);
	ui.dsp_sigmaspace->setMinimum(0);
	ui.dsp_sigmacolor->setValue(1);
	ui.sp_width->setMinimum(1);
	ui.sp_height->setMinimum(1);
	ui.sp_width->setValue(1);
	ui.sp_height->setValue(1);
	ui.sp_width->setSingleStep(2);
	ui.sp_height->setSingleStep(2);
}

void QControlPlan::DisableGaussianBlur()
{
	ui.sp_width->setSingleStep(1);
	ui.sp_height->setSingleStep(1);
	Disable();
}

void QControlPlan::EnableMedianBlur()
{
	Disable();
	ui.sp_ksize->setEnabled(true);
	ui.pb_Ok->setEnabled(true);
	ui.pb_Cancel->setEnabled(true);
	ui.sp_ksize->setMinimum(1);
	ui.sp_ksize->setValue(1);
	ui.sp_ksize->setSingleStep(2);
}

void QControlPlan::DisableMedianBlur()
{
	ui.sp_ksize->setMinimum(0);
	ui.sp_ksize->setSingleStep(1);
	Disable();
}

void QControlPlan::EnableBilateral()
{
	Disable();
	ui.dsp_sigmacolor->setEnabled(true);
	ui.dsp_sigmaspace->setEnabled(true);
	ui.sp_ksize->setEnabled(true);
	ui.pb_Ok->setEnabled(true);
	ui.pb_Cancel->setEnabled(true);
}

void QControlPlan::DisableBilateral()
{
	Disable();
}

void QControlPlan::EnableErosion()
{
	Disable();
	ui.sp_x->setEnabled(true);
	ui.sp_y->setEnabled(true);
	ui.sp_iterations->setEnabled(true);
	ui.pb_Ok->setEnabled(true);
	ui.pb_Cancel->setEnabled(true);
	ui.sp_x->setMinimum(-1);
	ui.sp_y->setMinimum(-1);
	ui.sp_iterations->setMinimum(1);
	ui.sp_iterations->setSingleStep(2);
}

void QControlPlan::DisableErosion()
{
	Disable();
}