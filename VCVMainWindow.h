#ifndef QVCVMAINWINDOW_H
#define QVCVMAINWINDOW_H

#include <QMainWindow>
#include "SystemDefine.h"

class QMenu;
class QAction;
class QMdiArea;
class QSplitter;
class QVCVData;
class QTabWidget;
class QControlPlan;
class QDockWidget;
class QMdiSubWindow;
class QCustomFilterDlg;
class QCommandBuilder;
class QVCVUndoCommand;



class QVCVMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	QVCVMainWindow(QWidget *parent = 0, Qt::WindowFlags f= 0);
	~QVCVMainWindow();

private:
	QVCVMainWindow(const QVCVMainWindow &VCVMainWindow);
	const void operator = (const QVCVMainWindow &VCVMainWindow);

private slots:
	void WindowActive(QMdiSubWindow *subwin);
	void New();
	void OpenFile();
	void Save();
	void Saveas();
	void BlurFilter();
	void GaussianBlurFilter();
	void MedianBlurFilter();
	void BilateralFilter();
	void CustomFilter2D();
	void Erosion();
	void Dilation();

	void ZoomIn();
	void ZoomOut();
	void NormalSize();

	void ParameterChangeRespond(const CommandParameter &para);
	void ControlPlanOk(const CommandParameter &para);
	void ControlPlanCancel(const CommandParameter &para);

private:
	QMenu *file_menu;
	QMenu *view_menu;
	QMenu *edit_menu;
	QMenu *help_menu;

	QMenu *filter_menu;

	QAction *file_menu_new;
	QAction *file_menu_open;
	QAction *file_menu_save;
	QAction *file_menu_saveas;

	QAction *view_tile;
	QAction *view_cascade;
	QAction *view_zoomin;
	QAction *view_zoomout;
	QAction *view_normalsize;

	QAction *filter_blur;
	QAction *filter_gaussian;
	QAction *filter_median;
	QAction *filter_bilateral;
	QAction *filter_custom2d;
	QAction *filter_erosion;
	QAction *filter_dilation;

	QMdiArea *mdi_area;

	QTabWidget *tabwidget;
    QDockWidget *dockwidget;
    QControlPlan *controlplan;
	QCommandBuilder *command_builder;
	QVCVUndoCommand *current_command;

    VCV_IMAGE_OPERATION image_operation;

	QVCVData *operation_data;

	void InitUI();
	void CreateAction();
	void CreateMenu();
    void CreateControlPlan();
	void CreateConnection();

	void DoOperation(VCV_IMAGE_OPERATION operation);
};

#endif
