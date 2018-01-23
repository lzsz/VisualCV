#include "SystemDefine.h"
#include "edgedetectionpanel.h"

QEdgeDetectionPanel::QEdgeDetectionPanel(QWidget *parent, Qt::WindowFlags f)
{
    ui.setupUi(this);
    command_para = new CommandParameter_Edge;

    setWindowFlags(f);

    connect(ui.sp_depth,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
    connect(ui.sp_ksize,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
    connect(ui.sp_dx,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
    connect(ui.sp_dy,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
    connect(ui.sp_aperturesize,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
    connect(ui.sp_l2gradient,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
    connect(ui.dsp_delta,SIGNAL(valueChanged(double)),this,SLOT(ValueChange()));
    connect(ui.dsp_scale,SIGNAL(valueChanged(double)),this,SLOT(ValueChange()));

    connect(ui.pb_ok,SIGNAL(clicked(bool)),this,SLOT(PushOk()));
    connect(ui.pb_cancel,SIGNAL(clicked(bool)),this,SLOT(PushCancel()));
}

QEdgeDetectionPanel::~QEdgeDetectionPanel()
{
    if(command_para!=NULL)
        delete command_para;
}

int QEdgeDetectionPanel::GetDepth()
{
    return ui.sp_depth->value();
}

int QEdgeDetectionPanel::GetKSize()
{
    return ui.sp_ksize->value();
}

int QEdgeDetectionPanel::GetDx()
{
    return ui.sp_dx->value();
}

int QEdgeDetectionPanel::GetDy()
{
    return ui.sp_dy->value();
}

int QEdgeDetectionPanel::GetApertureSize()
{
    return ui.sp_aperturesize->value();
}

bool QEdgeDetectionPanel::GetL2gradient()
{
    if(ui.sp_l2gradient->value()==0)
        return false;

    return true;
}

double QEdgeDetectionPanel::GetScale()
{
    return ui.dsp_scale->value();
}

double QEdgeDetectionPanel::GetDelta()
{
    return ui.dsp_delta->value();
}

void QEdgeDetectionPanel::GetAllParameter()
{
    if(command_para==NULL)
        command_para = new CommandParameter_Edge;

    ((CommandParameter_Edge*)command_para)->ddepth = ui.sp_depth->value();
    ((CommandParameter_Edge*)command_para)->ksize = ui.sp_ksize->value();
    ((CommandParameter_Edge*)command_para)->dx = ui.sp_dx->value();
    ((CommandParameter_Edge*)command_para)->dy = ui.sp_dy->value();
    ((CommandParameter_Edge*)command_para)->scale = ui.dsp_scale->value();
    ((CommandParameter_Edge*)command_para)->delta = ui.dsp_delta->value();
    ((CommandParameter_Edge*)command_para)->aperturesize = ui.sp_aperturesize->value();
    ((CommandParameter_Edge*)command_para)->l2gradient = ui.sp_l2gradient->value();
    ((CommandParameter_Edge*)command_para)->bordertype = GetBorderType();
}

void QEdgeDetectionPanel::DisableAll()
{
    ui.sp_depth->setEnabled(false);
    ui.sp_ksize->setEnabled(false);
    ui.sp_dx->setEnabled(false);
    ui.sp_dy->setEnabled(false);
    ui.dsp_delta->setEnabled(false);
    ui.dsp_scale->setEnabled(false);
    ui.sp_aperturesize->setEnabled(false);
    ui.sp_l2gradient->setEnabled(false);
    ui.pb_ok->setEnabled(false);
    ui.pb_cancel->setEnabled(false);
}

void QEdgeDetectionPanel::BeginOperation(VCV_IMAGE_OPERATION operation)
{
    switch (operation) {
    case IMAGE_EDGE_LAPLACIAN:
        EnableLaplacian();
        break;
    case IMAGE_EDGE_SOBEL:
        EnableSobel();
        break;
    case IMAGE_EDGE_CANNY:
        EnableCanny();
        break;
    default:
        break;
    }
}

void QEdgeDetectionPanel::EndOperation(VCV_IMAGE_OPERATION operation)
{
    DisableAll();
}

void QEdgeDetectionPanel::EnableLaplacian()
{
    DisableAll();
    ui.sp_depth->setEnabled(true);
    ui.sp_ksize->setEnabled(true);
    ui.dsp_scale->setEnabled(true);
    ui.dsp_delta->setEnabled(true);
    ui.label_double1->setText(tr("Scale"));
    ui.label_double2->setText(tr("Delta"));
    ui.pb_cancel->setEnabled(true);
    ui.pb_ok->setEnabled(true);
}

void QEdgeDetectionPanel::EnableSobel()
{
    DisableAll();
    ui.sp_depth->setEnabled(true);
    ui.sp_ksize->setEnabled(true);
    ui.sp_dx->setEnabled(true);
    ui.sp_dy->setEnabled(true);
    ui.dsp_scale->setEnabled(true);
    ui.dsp_delta->setEnabled(true);
    ui.label_double1->setText(tr("Scale"));
    ui.label_double2->setText(tr("Delta"));
    ui.pb_cancel->setEnabled(true);
    ui.pb_ok->setEnabled(true);
}

void QEdgeDetectionPanel::EnableCanny()
{
    DisableAll();
    ui.dsp_scale->setEnabled(true);
    ui.dsp_delta->setEnabled(true);
    ui.sp_aperturesize->setEnabled(true);
    ui.sp_l2gradient->setEnabled(true);
    ui.label_double1->setText(tr("Threshold1"));
    ui.label_double2->setText(tr("Threshold2"));
    ui.pb_cancel->setEnabled(true);
    ui.pb_ok->setEnabled(true);
}
