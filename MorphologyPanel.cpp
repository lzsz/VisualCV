#include "SystemDefine.h"
#include "MorphologyPanel.h"

QMorphologyPanel::QMorphologyPanel(QWidget *parent, Qt::WindowFlags f)
{
    ui.setupUi(this);
    setWindowFlags(f);
    command_para = new CommandParameter_Morphology;
    CreateBorderGroup(ui.groupBox);
}

QMorphologyPanel::~QMorphologyPanel()
{
    if(command_para!=NULL)
        delete command_para;
}

void QMorphologyPanel::BeginOperation(VCV_IMAGE_OPERATION operation)
{
    setEnabled(true);
    QControlPanel::BeginOperation(operation);
}

void QMorphologyPanel::EndOperation(VCV_IMAGE_OPERATION operation)
{
//    DisableAll();
}

void QMorphologyPanel::ValueChange()
{
    GetAllParameter();
    emit ParameterChange(command_para);
}

void QMorphologyPanel::PushOk()
{
    GetAllParameter();
    emit PanelOk(command_para);
}

void QMorphologyPanel::PushCancel()
{
    GetAllParameter();
    emit PanelCancel(command_para);
}

void QMorphologyPanel::GetAllParameter()
{
    command_para->anchor.x = ui.sp_anchorx->value();
    command_para->anchor.y = ui.sp_anchory->value();
    command_para->borderType = GetBorderType();
    command_para->iteration = ui.sp_iterations->value();
    command_para->kernelAnchor.x = ui.sp_kernelanchorx->value();
    command_para->kernelAnchor.y = ui.sp_kernelanchory->value();
    command_para->ksize = ui.sp_kernelsize->value();

    switch(ui.cb_morphshape->currentIndex())
    {
    case 0:
        command_para->shape = MORPH_RECT;
        break;
    case 1:
        command_para->shape = MORPH_CROSS;
        break;
    case 2:
        command_para->shape = MORPH_ELLIPSE;
        break;
    default:
        command_para->shape = MORPH_RECT;
    }

    command_para->tp1 = ui.sp_tp1->value();
    command_para->tp2 = ui.sp_tp2->value();
    command_para->tp3 = ui.sp_tp3->value();
    command_para->tp4 = ui.sp_tp4->value();
}

void QMorphologyPanel::DisableAll()
{
    setEnabled(false);
}
