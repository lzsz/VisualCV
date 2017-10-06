#include <QMenu>
#include <QAction>
#include <QMdiArea>
#include <QSplitter>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QTabWidget>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QMdiSubWindow>

#include "VCVData.h"
#include "VCVDataModel.h"
#include "DataModelInstance.h"
#include "VCVChildWindow.h"
#include "FilterPanel.h"
#include "CustomFilterDlg.h"
#include "NewDialog.h"

#include "CommandInclude/CommandBuilder.h"
#include "CommandInclude/VCVUndoCommand.h"
#include "CommandInclude/CustomFilter2D.h"

#include "VCVMainWindow.h"

QVCVMainWindow::QVCVMainWindow(QWidget *parent , Qt::WindowFlags f)
	:QMainWindow(parent,f)
{
	InitUI();

	setMinimumSize(800,600);

	mdi_area = new QMdiArea(this);
	mdi_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdi_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setCentralWidget(mdi_area);

	CreateConnection();

	image_operation = IMAGE_NONE;
	current_command = NULL;
	operation_data = NULL;
	command_builder = new QCommandBuilder;
}

QVCVMainWindow::~QVCVMainWindow()
{

}

void QVCVMainWindow::InitUI()
{
	CreateAction();
	CreateMenu();
    CreateControlPanel();

	QLabel *label1 = new QLabel(tr("OK"));
	statusBar()->addWidget(label1);

	setWindowTitle(tr("Visual CV"));
}

void QVCVMainWindow::CreateAction()
{
	file_menu_new = new QAction(tr("&New"),this);
	file_menu_open = new QAction(tr("&Open"),this);
	file_menu_save = new QAction(tr("&Save"),this);
	file_menu_saveas = new QAction(tr("Save as"),this);

	view_tile = new QAction(tr("&Tile"),this);
	view_cascade = new QAction(tr("&Cascade"),this);
	view_zoomin = new QAction(tr("Zoom&In"),this);
	view_zoomout = new QAction(tr("Zoom&Out"),this);
	view_normalsize = new QAction("&Normal Size",this);

	filter_blur = new QAction(tr("Blur"),this);
	filter_gaussian = new QAction(tr("GaussianBlur"),this);
	filter_median = new QAction(tr("MedianBlur"),this);
	filter_bilateral = new QAction(tr("Bilateral"),this);
	filter_custom2d = new QAction(tr("CustomFilter2D"),this);
	filter_erosion = new QAction(tr("Erosion"),this);
	filter_dilation = new QAction(tr("Dilation"),this);
}

void QVCVMainWindow::CreateMenu()
{
	file_menu = menuBar()->addMenu(tr("&File"));
	edit_menu = menuBar()->addMenu(tr("&Edit"));
	view_menu = menuBar()->addMenu(tr("&View"));
	filter_menu =  menuBar()->addMenu(tr("&Filter"));
	help_menu = menuBar()->addMenu(tr("&Help"));

	file_menu->addAction(file_menu_new);
	file_menu->addAction(file_menu_open);
	file_menu->addAction(file_menu_save);
	file_menu->addAction(file_menu_saveas);

	view_menu->addAction(view_tile);
	view_menu->addAction(view_cascade);
	view_menu->addSeparator();
	view_menu->addAction(view_zoomin);
	view_menu->addAction(view_zoomout);
	view_menu->addAction(view_normalsize);

	filter_menu->addAction(filter_blur);
	filter_menu->addAction(filter_gaussian);
	filter_menu->addAction(filter_median);
	filter_menu->addAction(filter_bilateral);
	filter_menu->addAction(filter_custom2d);
	filter_menu->addSeparator();
	filter_menu->addAction(filter_erosion);
	filter_menu->addAction(filter_dilation);
}

void QVCVMainWindow::CreateControlPanel()
{
//	tabwidget = new QTabWidget;
//    dockwidget = new QDockWidget;
    filterpanel = new QFilterPanel;
//    tabwidget->addTab(filterpanel,tr("Filter"));
//    dockwidget->setWidget(tabwidget);
//    dockwidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
//    addDockWidget(Qt::BottomDockWidgetArea,dockwidget);
}

void QVCVMainWindow::WindowActive(QMdiSubWindow *subwin)
{
	if(subwin==NULL)
		return;
	QDataModelInstance::Instance()->Notify();
	QVCVData *current_data = QDataModelInstance::Instance()->GetData(subwin->windowTitle().toStdString().c_str());
	edit_menu->clear();
	edit_menu->addAction(current_data->GetUndoAction());
	edit_menu->addAction(current_data->GetRedoAction());
}

void QVCVMainWindow::New()
{
	QNewDialog nd;
	nd.exec();
}

void QVCVMainWindow::OpenFile()
{
	QStringList files;
	files = QFileDialog::getOpenFileNames(this,
		tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.bmp)"));

	if(files.size()<0)
		return;

	int i;
	QVCVData *vcvdata = NULL;
	QVCVChildWindow *viewer = NULL;
	for(i=0;i<files.size();++i)
	{
		vcvdata = new QVCVData;
		vcvdata->Initialize();
		viewer = new QVCVChildWindow;
		QMdiSubWindow *sub_win = new QMdiSubWindow;
		sub_win->setWidget(viewer);
		vcvdata->RegisterViewer(viewer);
		vcvdata->Load(files.at(i));
		viewer->setWindowTitle(files.at(i));
		mdi_area->addSubWindow(sub_win);
		QDataModelInstance::Instance()->AddData(vcvdata);

		viewer->show();
	}
}

void QVCVMainWindow::Save()
{

}

void QVCVMainWindow::Saveas()
{
	if(QDataModelInstance::Instance()->Count()<=0)
		return;

	QString file;
	file = QFileDialog::getSaveFileName(this,
		tr("Open Image"), "/home", tr("Image Files(*.jpg);;Image Files(*.png);;Image Files(*.bmp)"));

	if(!QDataModelInstance::Instance()->GetData(mdi_area->activeSubWindow()->windowTitle().toStdString().c_str())->SaveAs(file))
		QMessageBox::information(NULL,tr("Information"),tr("Write file to headdisk fail !"));
}

void QVCVMainWindow::BlurFilter()
{
    if(filterpanel!=NULL)
        filterpanel->show();
	DoOperation(IMAGE_FILTER_BLUR);
}

void QVCVMainWindow::GaussianBlurFilter()
{
	DoOperation(IMAGE_FILTER_GAUSSIANBLUR);
}

void QVCVMainWindow::MedianBlurFilter()
{
	DoOperation(IMAGE_FILTER_MEDIANBLUR);
}

void QVCVMainWindow::BilateralFilter()
{
	DoOperation(IMAGE_FILTER_BILATERAL);
}

void QVCVMainWindow::CustomFilter2D()
{
	if(mdi_area->activeSubWindow()==NULL)
		return;

	QCustomFilterDlg *customfilterdlg = new QCustomFilterDlg;

	customfilterdlg->Initialize(QDataModelInstance::Instance()->GetData(mdi_area->activeSubWindow()->windowTitle().toStdString().c_str())->GetDisplayImage());

	int result = customfilterdlg->exec();
	if(result ==QDialog::Accepted)
	{
		operation_data = QDataModelInstance::Instance()->GetData(mdi_area->activeSubWindow()->windowTitle().toStdString().c_str());

		if(operation_data==NULL)
			return;

		CommandParameter cp;
		cp.depth = customfilterdlg->GetDepth();
		cp.delta = customfilterdlg->GetDelta();
		cp.kernel = customfilterdlg->GetKernel();
		cp.point = customfilterdlg->GetAnchor();
		cp.bordertype = BORDER_DEFAULT;
		QCustomFilter2D *command = new QCustomFilter2D;

		command->Initialize(operation_data,cp);
		command->redo();
		
		operation_data->AddStep(command);

		operation_data = NULL;
	}

	delete customfilterdlg;
}

void QVCVMainWindow::Erosion()
{
	DoOperation(IMAGE_FILTER_EROSION);
}

void QVCVMainWindow::Dilation()
{
	DoOperation(IMAGE_FILTER_DILATION);
}

void QVCVMainWindow::ZoomIn()
{
	if(mdi_area->activeSubWindow()!=NULL)
		((QVCVChildWindow*)mdi_area->activeSubWindow()->widget())->SetDisplayScale(
			((QVCVChildWindow*)mdi_area->activeSubWindow()->widget())->GetDisplayScale()*1.1);
}

void QVCVMainWindow::ZoomOut()
{
	if(mdi_area->activeSubWindow()!=NULL)
		((QVCVChildWindow*)mdi_area->activeSubWindow()->widget())->SetDisplayScale(
			((QVCVChildWindow*)mdi_area->activeSubWindow()->widget())->GetDisplayScale()*0.9);
}

void QVCVMainWindow::NormalSize()
{
	if(mdi_area->activeSubWindow()!=NULL)
		((QVCVChildWindow*)mdi_area->activeSubWindow()->widget())->SetDisplayScale(1.0);
}

void QVCVMainWindow::ParameterChangeRespond(const CommandParameter &para)
{
	if(current_command==NULL)
		return;

	if(!current_command->IsInit())
	{
		if(!current_command->Initialize(operation_data,para))
			return;
	}
	else
	{
		if(!current_command->SetParameter(para))
			return;
	}

	current_command->redo();
}

void QVCVMainWindow::FilterPanelOk(const CommandParameter &para)
{
//	mdi_area->setEnabled(true);
//	menuBar()->setEnabled(true);

	if(current_command==NULL)
		return;

	if(!current_command->IsInit())
	{
		if(!current_command->Initialize(operation_data,para))
			return;
	}
	else
	{
		if(!current_command->SetParameter(para))
			return;
	}

	operation_data->AddStep(current_command);
	current_command = NULL;
	operation_data = NULL;
}

void QVCVMainWindow::FilterPanelCancel(const CommandParameter &para)
{
	mdi_area->setEnabled(true);
	menuBar()->setEnabled(true);

	if(current_command==NULL)
		return;

	if(!current_command->IsInit())
		if(!current_command->Initialize(operation_data,para))
			return;
	current_command->undo();
	delete current_command;
	current_command = NULL;

//	mdi_area->setEnabled(true);
//	menuBar()->setEnabled(true);
}


void QVCVMainWindow::DoOperation(VCV_IMAGE_OPERATION operation)
{
	if(QDataModelInstance::Instance()->Count()<=0)
		return;

	image_operation = operation;

	if(mdi_area->activeSubWindow()==NULL)
		return;
	
	operation_data = QDataModelInstance::Instance()->GetData(mdi_area->activeSubWindow()->windowTitle().toStdString().c_str());

	if(operation_data==NULL)
		return;

    filterpanel->BeginOperation(image_operation);

	current_command = command_builder->CreateCommand(image_operation);

    //mdi_area->setEnabled(false);
    //menuBar()->setEnabled(false);
}

void QVCVMainWindow::CreateConnection()
{
	connect(file_menu_new,SIGNAL(triggered()),this,SLOT(New()));
	connect(file_menu_open,SIGNAL(triggered()),this,SLOT(OpenFile()));
	connect(file_menu_saveas,SIGNAL(triggered()),this,SLOT(Saveas()));


	connect(mdi_area,SIGNAL(subWindowActivated(QMdiSubWindow*)),this,SLOT(WindowActive(QMdiSubWindow*)));
    connect(filterpanel,SIGNAL(ParameterChange(const CommandParameter&)),this,SLOT(ParameterChangeRespond(const CommandParameter&)));
    connect(filterpanel,SIGNAL(FilterPanelOk(const CommandParameter&)),this,SLOT(FilterPanelOk(const CommandParameter&)));
    connect(filterpanel,SIGNAL(FilterPanelCancel(const CommandParameter &)),this,SLOT(FilterPanelCancel(const CommandParameter &)));

	connect(filter_blur,SIGNAL(triggered()),this,SLOT(BlurFilter()));
	connect(filter_gaussian,SIGNAL(triggered()),this,SLOT(GaussianBlurFilter()));
	connect(filter_median,SIGNAL(triggered()),this,SLOT(MedianBlurFilter()));
	connect(filter_bilateral,SIGNAL(triggered()),this,SLOT(BilateralFilter()));
	connect(filter_custom2d,SIGNAL(triggered()),this,SLOT(CustomFilter2D()));
	connect(filter_erosion,SIGNAL(triggered()),this,SLOT(Erosion()));
	connect(filter_dilation,SIGNAL(triggered()),this,SLOT(Dilation()));

	connect(view_zoomin,SIGNAL(triggered()),this,SLOT(ZoomIn()));
	connect(view_zoomout,SIGNAL(triggered()),this,SLOT(ZoomOut()));
	connect(view_normalsize,SIGNAL(triggered()),this,SLOT(NormalSize()));

	connect(view_tile,SIGNAL(triggered()),mdi_area,SLOT(tileSubWindows()));
	connect(view_cascade,SIGNAL(triggered()),mdi_area,SLOT(cascadeSubWindows()));
}
