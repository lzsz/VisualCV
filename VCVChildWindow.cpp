#include <QImage>
#include <QPainter>
#include <QScrollBar>

#include "VCVDataModel.h"
#include "DataModelInstance.h"
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

	connect(v_scrollbar,SIGNAL(valueChanged(int)),this,SLOT(repaint()));
	connect(h_scrollbar,SIGNAL(valueChanged(int)),this,SLOT(repaint()));
	//connect(v_scrollbar,SIGNAL(	rangeChanged ( int,int)),this,SLOT(VScrollBarRangeChanged(int,int)));
	//connect(h_scrollbar,SIGNAL(	rangeChanged ( int,int)),this,SLOT(HScrollBarRangeChanged(int,int)));
}

QVCVChildWindow::~QVCVChildWindow()
{
	if(update_image!=NULL)
		delete update_image;
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
