#include "ThresholdPanel.h"

QThresholdPanel::QThresholdPanel(QWidget *parent,Qt::WindowFlags f)
{
    ui.setupUi(this);
    ui.rb_binary->setChecked(true);
    ui.rb_mean_c->setChecked(true);
    setWindowFlags(f);
    command_para = new CommandParameter_Threshold;
}

QThresholdPanel::~QThresholdPanel()
{
    if(command_para!=NULL)
        delete command_para;
}

void QThresholdPanel::BeginOperation(VCV_IMAGE_OPERATION operation)
{
    image_operation = operation;
    switch(image_operation)
    {
    case IMAGE_THRESHOLD_THRESHOLD:
        EnableThreshold();
        break;
    case IMAGE_THRESHOLD_ADAPTIVE:
        EnableAdaptive();
        break;
    default:
        break;
    }
}

void QThresholdPanel::EndOperation(VCV_IMAGE_OPERATION operation)
{
    DisableAll();
    image_operation = IMAGE_NONE;
}

void QThresholdPanel::ValueChange()
{
    GetAllParameter();

    emit ParameterChange(command_para);
}

void QThresholdPanel::PushOk()
{
    GetAllParameter();

    emit PanelOk(command_para);
}

void QThresholdPanel::PushCancel()
{
    GetAllParameter();

    emit PanelCancel(command_para);
}

void QThresholdPanel::EnableThreshold()
{
    DisableAll();
    ui.dsp_thresh->setEnabled(true);
    ui.dsp_max->setEnabled(true);
    ui.ThresholdTypeGroup->setEnabled(true);
}

void QThresholdPanel::EnableAdaptive()
{
    DisableAll();
    ui.dsp_constant->setEnabled(true);
    ui.dsp_max->setEnabled(true);
    ui.sp_blocksize->setEnabled(true);
    ui.ThresholdTypeGroup->setEnabled(true);
    ui.AdaptiveTypeGroup->setEnabled(true);
}

void QThresholdPanel::DisableAll()
{
    ui.dsp_thresh->setEnabled(false);
    ui.dsp_max->setEnabled(false);
    ui.dsp_constant->setEnabled(false);
    ui.sp_blocksize->setEnabled(false);
    ui.ThresholdTypeGroup->setEnabled(false);
    ui.AdaptiveTypeGroup->setEnabled(false);
}

void QThresholdPanel::GetAllParameter()
{
    if(command_para==NULL)
        return;

    command_para->thresh = ui.dsp_thresh->value();
    command_para->max = ui.dsp_max->value();
    command_para->constant = ui.dsp_constant->value();
    command_para->block_size = ui.sp_blocksize->value();
}

ThresholdTypes QThresholdPanel::GetThresholdType()
{
    if(ui.rb_binary->isChecked())
        return THRESH_BINARY;
    if(ui.rb_binary_inv->isChecked())
        return THRESH_BINARY_INV;
    if(ui.rb_trunc->isChecked())
        return THRESH_TRUNC;
    if(ui.rb_tozero->isChecked())
        return THRESH_TOZERO;
    if(ui.rb_tozero_inv->isChecked())
        return THRESH_TOZERO_INV;
    if(ui.rb_mask->isChecked())
        return THRESH_MASK;
    if(ui.rb_otsu->isChecked())
        return THRESH_OTSU;
    if(ui.rb_triangle->isChecked())
        return THRESH_TRIANGLE;

    return THRESH_OTSU;
}

AdaptiveThresholdTypes QThresholdPanel::GetAdaptiveType()
{
    if(ui.rb_mean_c->isChecked())
        return ADAPTIVE_THRESH_MEAN_C;
    if(ui.rb_gaussian_c->isChecked())
        return ADAPTIVE_THRESH_GAUSSIAN_C;

    return ADAPTIVE_THRESH_MEAN_C;
}

