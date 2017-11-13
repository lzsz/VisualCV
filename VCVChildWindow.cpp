#include <QImage>
#include <QPainter>
#include <QScrollBar>
#include "ControlPanel.h"
#include "FilterPanel.h"
#include "VCVDataModel.h"
#include "DataModelInstance.h"
#include "CommandInclude/CommandBuilder.h"
#include "CommandInclude/VCVUndoCommand.h"
#include "CommandInclude/ImageProcCommand.h"
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

    filter_panel = Q_NULLPTR;


	connect(v_scrollbar,SIGNAL(valueChanged(int)),this,SLOT(repaint()));
	connect(h_scrollbar,SIGNAL(valueChanged(int)),this,SLOT(repaint()));
}

QVCVChildWindow::~QVCVChildWindow()
{
	if(update_image!=NULL)
		delete update_image;
    if(filter_panel!=Q_NULLPTR)
    {
        delete filter_panel;
        filter_panel = Q_NULLPTR;
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
        filter_panel = new QFilterPanel(NULL,Qt::WindowStaysOnTopHint);
        connect(filter_panel,SIGNAL(ParameterChange(const CommandParameter*)),this,SLOT(FilterParameterChangeRespond(const CommandParameter*)));
        connect(filter_panel,SIGNAL(FilterPanelOk(const CommandParameter*)),this,SLOT(FilterPanelOk(const CommandParameter*)));
        connect(filter_panel,SIGNAL(FilterPanelCancel(const CommandParameter*)),this,SLOT(FilterParameterChangeRespond(const CommandParameter*)));
    }

    if(filter_panel!=NULL)
        DoOperation((QControlPanel*)filter_panel,(QVCVUndoCommand**)&filter_command,operation);

    return filter_panel;
}

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

}

void QVCVChildWindow::FilterPanelCancel(const CommandParameter *para)
{

}
/////////////////////
//slot function
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
    if(QDataModelInstance::Instance()->Count()<=0)
        return;

    operator_data = QDataModelInstance::Instance()->GetData(windowTitle().toStdString().c_str());

    if(operator_data==NULL)
        return;

    panel->BeginOperation(operation);

    *command = command_builder->CreateCommand(operation);
}
