#include <QImage>
#include <QPainter>
#include <QScrollBar>
#include "ControlPanel.h"
#include "FilterPanel.h"
#include "CustomFilterDlg.h"
#include "ThresholdPanel.h"
#include "MorphologyPanel.h"
#include "edgedetectionpanel.h"
#include "VCVData.h"
#include "VCVDataModel.h"
#include "DataModelInstance.h"
#include "CommandInclude/CommandBuilder.h"
#include "CommandInclude/VCVUndoCommand.h"
#include "CommandInclude/ImageProcCommand.h"
#include "CommandInclude/Threshold.h"
#include "VCVChildWindow.h"

QVCVChildWindow::QVCVChildWindow(QWidget *parent, Qt::WindowFlags f)
        :QWidget(parent,f)
{
    update_image = NULL;
    setAttribute(Qt::WA_DeleteOnClose);
    setMinimumSize(300,300);

    v_scrollbar = new QScrollBar(Qt::Vertical,this);
    h_scrollbar = new QScrollBar(Qt::Horizontal,this);

    display_scale = 1.0;

    filter_panel = NULL;
    customfilter_panel = NULL;
    filter_command = NULL;
    custom_filter_command = NULL;
    threshold_panel = NULL;
    threshold_command = NULL;
    morphology_panel = NULL;
    morphology_command = NULL;
    edgedetection_panel = NULL;
    edgedetection_command = NULL;

    connect(v_scrollbar,SIGNAL(valueChanged(int)),this,SLOT(repaint()));
    connect(h_scrollbar,SIGNAL(valueChanged(int)),this,SLOT(repaint()));
}

QVCVChildWindow::~QVCVChildWindow()
{
    if(update_image!=NULL)
        delete update_image;
    if(filter_panel!=NULL)
    {
        delete filter_panel;
        filter_panel = NULL;
    }
    if(customfilter_panel!=NULL)
    {
        delete customfilter_panel;
        customfilter_panel = NULL;
    }
    if(threshold_panel != NULL)
    {
        delete threshold_panel;
        threshold_panel = NULL;
    }
    if(morphology_panel!=NULL)
    {
        delete morphology_panel;
        morphology_panel = NULL;
    }
}

////////////////////////
//public function

bool QVCVChildWindow::Update(const Mat &image)
{
    if(image.empty())
        return false;

    if(update_image!=NULL)
        delete update_image;

    update_image = MatToQImage(image);

    AdjustScrollBarRange();

    repaint();

    return true;
}

void QVCVChildWindow::SetDisplayScale(float scale)
{
    display_scale = scale;

    repaint();
}

float QVCVChildWindow::GetDisplayScale()
{
    return display_scale;
}

QFilterPanel* QVCVChildWindow::GetFilterPanel(VCV_IMAGE_OPERATION operation)
{
    if(filter_panel==NULL)
    {
        filter_panel = new QFilterPanel(this,Qt::WindowStaysOnTopHint);

        if(filter_panel==NULL)
            return NULL;
        connect(filter_panel,SIGNAL(ParameterChange(const CommandParameter*)),this,SLOT(FilterParameterChangeRespond(const CommandParameter*)));
        connect(filter_panel,SIGNAL(PanelOk(const CommandParameter*)),this,SLOT(FilterPanelOk(const CommandParameter*)));
        connect(filter_panel,SIGNAL(PanelCancel(const CommandParameter*)),this,SLOT(FilterPanelCancel(const CommandParameter*)));
    }

    DoOperation((QControlPanel*)filter_panel,(QVCVUndoCommand**)&filter_command,operation);

    return filter_panel;
}

QCustomFilterDlg* QVCVChildWindow::GetCustomFilterPanel()
{
    if(customfilter_panel==NULL)
    {
        customfilter_panel = new QCustomFilterDlg(this,Qt::WindowStaysOnTopHint);
        if(customfilter_panel==NULL)
            return NULL;
        connect(customfilter_panel,SIGNAL(ParameterChange(const CommandParameter*)),this,SLOT(CustomFilterParameterChange(const CommandParameter*)));
        connect(customfilter_panel,SIGNAL(PanelOk(const CommandParameter*)),this,SLOT(CustomFilterPanelOk(const CommandParameter*)));
        connect(customfilter_panel,SIGNAL(PanelCancel(const CommandParameter*)),this,SLOT(CustomFilterPanelCancel(const CommandParameter*)));
    }

    DoOperation((QControlPanel*)customfilter_panel,(QVCVUndoCommand**)&custom_filter_command,IMAGE_FILTER_CUSTOM2D);

    return customfilter_panel;
}

QThresholdPanel* QVCVChildWindow::GetThresholdPanel(VCV_IMAGE_OPERATION operation)
{
    if(threshold_panel==NULL)
    {
        threshold_panel = new QThresholdPanel(this,Qt::WindowStaysOnTopHint);
        if(threshold_panel==NULL)
            return NULL;

        connect(threshold_panel,SIGNAL(ParameterChange(const CommandParameter*)),this,SLOT(ThresholdParameterChange(const CommandParameter*)));
        connect(threshold_panel,SIGNAL(PanelOk(const CommandParameter*)),this,SLOT(ThresholdPanelOk(const CommandParameter*)));
        connect(threshold_panel,SIGNAL(PanelCancel(const CommandParameter*)),this,SLOT(ThresholdPanelCancel(const CommandParameter*)));
    }

    DoOperation(threshold_panel,(QVCVUndoCommand**)&threshold_command,operation);

    return threshold_panel;
}

QMorphologyPanel* QVCVChildWindow::GetMorphologyPanel(VCV_IMAGE_OPERATION operation)
{
    if(morphology_panel==NULL)
    {
        morphology_panel = new QMorphologyPanel(this,Qt::WindowStaysOnTopHint);
        if(morphology_panel==NULL)
            return NULL;

        connect(morphology_panel,SIGNAL(ParameterChange(const CommandParameter*)),this,SLOT(MorphologyParameterChange(const CommandParameter*)));
        connect(morphology_panel,SIGNAL(PanelOk(const CommandParameter*)),this,SLOT(MorphologyPanelOk(const CommandParameter*)));
        connect(morphology_panel,SIGNAL(PanelCancel(const CommandParameter*)),this,SLOT(MorphologyPanelCancel(const CommandParameter*)));
    }

    DoOperation(morphology_panel,(QVCVUndoCommand**)&morphology_command,operation);

    return morphology_panel;
}

QEdgeDetectionPanel* QVCVChildWindow::GetEdgeDetectionPanel(VCV_IMAGE_OPERATION operation)
{
    if(edgedetection_panel==NULL)
    {
        edgedetection_panel = new QEdgeDetectionPanel(this,Qt::WindowStaysOnTopHint);
        if(edgedetection_panel==NULL)
            return NULL;

        connect(edgedetection_panel,SIGNAL(ParameterChange(const CommandParameter*)),this,SLOT(EdgeDetectionParameterChange(const CommandParameter*)));
        connect(edgedetection_panel,SIGNAL(PanelOk(const CommandParameter*)),this,SLOT(EdgeDetectionPanelOk(const CommandParameter*)));
        connect(edgedetection_panel,SIGNAL(PanelCancel(const CommandParameter*)),this,SLOT(EdgeDetectionPanelCancel(const CommandParameter*)));
    }

    DoOperation(edgedetection_panel,(QVCVUndoCommand**)&edgedetection_command,operation);

    return edgedetection_panel;
}

/////////////////////
//slot function
void QVCVChildWindow::FilterParameterChangeRespond(const CommandParameter *para)
{
    if(filter_command==NULL)
        return;

    if(!filter_command->IsInit())
    {
        if(!filter_command->Initialize(operator_data,para))
            return;
    }
    else
    {
        if(!filter_command->SetParameter(para))
            return;
    }

    filter_command->redo();
}

void QVCVChildWindow::FilterPanelOk(const CommandParameter *para)
{
    if(filter_command==NULL)
        return;

    if(!filter_command->IsInit())
    {
        if(!filter_command->Initialize(operator_data,para))
            return;
    }
    else
    {
        if(!filter_command->SetParameter(para))
            return;
    }

    if(operator_data!=NULL)
        operator_data->AddStep(filter_command);
    filter_command = NULL;
}

void QVCVChildWindow::FilterPanelCancel(const CommandParameter *para)
{
    if(filter_command==NULL)
        return;

    if(!filter_command->IsInit())
        if(!filter_command->Initialize(operator_data,para))
            return;
    filter_command->undo();
    delete filter_command;
    filter_command = NULL;
}

void QVCVChildWindow::CustomFilterParameterChange(const CommandParameter *para)
{
    PanelValueChange(custom_filter_command,para);
}

void QVCVChildWindow::CustomFilterPanelOk(const CommandParameter *para)
{
    PanelOk((QVCVUndoCommand**)&custom_filter_command,para);
}

void QVCVChildWindow::CustomFilterPanelCancel(const CommandParameter *para)
{
    PanelCencel((QVCVUndoCommand**)&custom_filter_command,para);
}

void QVCVChildWindow::ThresholdParameterChange(const CommandParameter *para)
{
    PanelValueChange((QVCVUndoCommand*)threshold_command,para);
}

void QVCVChildWindow::ThresholdPanelOk(const CommandParameter *para)
{
    PanelOk((QVCVUndoCommand**)&threshold_command,para);
}

void QVCVChildWindow::ThresholdPanelCancel(const CommandParameter *para)
{
    PanelCencel((QVCVUndoCommand**)&threshold_command,para);
}

void QVCVChildWindow::MorphologyParameterChange(const CommandParameter *para)
{
    PanelValueChange((QVCVUndoCommand*)morphology_command,para);
}

void QVCVChildWindow::MorphologyPanelOk(const CommandParameter *para)
{
    PanelOk((QVCVUndoCommand**)&morphology_command,para);
}

void QVCVChildWindow::MorphologyPanelCancel(const CommandParameter *para)
{
    PanelCencel((QVCVUndoCommand**)&morphology_command,para);
}

void QVCVChildWindow::EdgeDetectionParameterChange(const CommandParameter *para)
{
    PanelValueChange((QVCVUndoCommand*)edgedetection_command,para);
}

void QVCVChildWindow::EdgeDetectionPanelOk(const CommandParameter *para)
{
    PanelOk((QVCVUndoCommand**)&edgedetection_command,para);
}

void QVCVChildWindow::EdgeDetectionPanelCancel(const CommandParameter *para)
{
    PanelCencel((QVCVUndoCommand**)&edgedetection_command,para);
}

void QVCVChildWindow::VScrollBarRangeChanged(int min, int max)
{
    if (max - min == 0)
        v_scrollbar->hide();
    else
        v_scrollbar->show();
}

void QVCVChildWindow::HScrollBarRangeChanged(int min, int max)
{

    if (max - min == 0)
        h_scrollbar->hide();
    else
        h_scrollbar->show();
}

/////////////////////
//event function

void QVCVChildWindow::paintEvent(QPaintEvent *e)
{
    if(update_image==NULL)
    {
        QWidget::paintEvent(e);
        return;
    }

    DrawClient();

    QWidget::paintEvent(e);
}

void QVCVChildWindow::closeEvent(QCloseEvent *e)
{
    QDataModelInstance::Instance()->DeleteData(windowTitle());
    QWidget::closeEvent(e);
}

void QVCVChildWindow::resizeEvent(QResizeEvent *e)
{
    AdjustScrollBar();

    QWidget::resizeEvent(e);
}

////////////////////////
//private function

void QVCVChildWindow::AdjustScrollBar()
{
    int width = SCROLLBAR_SIZE; //scrollbar width
    int x = size().width() - width;
    x = x < 0 ? 0: x;
    int height = size().height()-SCROLLBAR_SIZE;
    v_scrollbar->setGeometry(x, 0, width, height);

    height = SCROLLBAR_SIZE;
    int y = size().height() - height;
    y = y < 0 ? 0: y;
    width  = size().width()-SCROLLBAR_SIZE;
    h_scrollbar->setGeometry(0, y, width, height);

    if(update_image!=NULL)
        AdjustScrollBarRange();
}

void QVCVChildWindow::AdjustScrollBarRange()
{
    if(update_image==NULL)
        return;

    if(update_image->height()*display_scale-height()+SCROLLBAR_SIZE>0)
    {
        int pos = v_scrollbar->value();
        v_scrollbar->setRange(0,update_image->height()*display_scale-height()+SCROLLBAR_SIZE);
        v_scrollbar->setValue(pos);
    }
    else
    {
        v_scrollbar->setRange(0,0);
    }

    if(update_image->width()*display_scale-width()+SCROLLBAR_SIZE>0)
    {
        int pos = h_scrollbar->value();
        h_scrollbar->setRange(0,update_image->width()*display_scale-width()+SCROLLBAR_SIZE);
        h_scrollbar->setValue(pos);
    }
    else
    {
        h_scrollbar->setRange(0,0);
    }
}

void QVCVChildWindow::DrawClient()
{
    if(update_image==NULL)
        return;

    QPainter painter(this);

    if(update_image->height()*display_scale-height()+SCROLLBAR_SIZE>0)
    {
        widget_display_area.setTop(0);
        widget_display_area.setHeight(height()-SCROLLBAR_SIZE);
        image_display_area.setTop(v_scrollbar->value()/display_scale);
        image_display_area.setHeight((height()-SCROLLBAR_SIZE)/display_scale);
    }
    else
    {
        widget_display_area.setTop((height()-(update_image->height()*display_scale)-SCROLLBAR_SIZE)/2);
        widget_display_area.setHeight(update_image->height()*display_scale);
        image_display_area.setTop(0);
        image_display_area.setHeight(update_image->height());
    }

    if(update_image->width()*display_scale-width()+SCROLLBAR_SIZE>0)
    {
        widget_display_area.setLeft(0);
        widget_display_area.setWidth(width()-SCROLLBAR_SIZE);
        image_display_area.setLeft(h_scrollbar->value()/display_scale);
        image_display_area.setWidth((width()-SCROLLBAR_SIZE)/display_scale);
    }
    else
    {
        widget_display_area.setLeft((width()-(update_image->width()*display_scale)-SCROLLBAR_SIZE)/2);
        widget_display_area.setWidth(update_image->width()*display_scale);
        image_display_area.setLeft(0);
        image_display_area.setWidth(update_image->width());
    }

    painter.drawImage(widget_display_area,*update_image,image_display_area);
}

void QVCVChildWindow::DoOperation(QControlPanel *panel,QVCVUndoCommand **command, VCV_IMAGE_OPERATION operation)
{
    if(command == NULL)
        return;

    if(QDataModelInstance::Instance()->Count()<=0)
        return;

    operator_data = QDataModelInstance::Instance()->GetData(windowTitle().toStdString().c_str());

    if(operator_data==NULL)
        return;

    panel->BeginOperation(operation);

    *command = command_builder->CreateCommand(operation);
}

void QVCVChildWindow::PanelValueChange(QVCVUndoCommand *command, const CommandParameter *para)
{
    if(command==NULL)
        return;

    if(!command->IsInit())
    {
        if(!command->Initialize(operator_data,para))
            return;
    }
    else
    {
        if(!command->SetParameter(para))
            return;
    }

    command->redo();
}

void QVCVChildWindow::PanelOk(QVCVUndoCommand **command, const CommandParameter *para)
{
    if(*command==NULL)
        return;

    if(!(*command)->IsInit())
    {
        if(!(*command)->Initialize(operator_data,para))
            return;
    }
    else
    {
        if(!(*command)->SetParameter(para))
            return;
    }

    if(operator_data!=NULL)
        operator_data->AddStep(*command);
    *command = NULL;
}

void QVCVChildWindow::PanelCencel(QVCVUndoCommand **command, const CommandParameter *para)
{
     if(*command==NULL)
        return;

    if(!(*command)->IsInit())
        if(!(*command)->Initialize(operator_data,para))
            return;
    (*command)->undo();
    delete *command;
    *command = NULL;
}
