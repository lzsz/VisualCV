#ifndef VCVCHILDWINDOW_H
#define VCVCHILDWINDOW_H

#include <QWidget>
#include "SystemDefine.h"
class QMenu;
class QAction;
class QScrollBar;
class QFilterPanel;
class QControlPanel;
class QVCVData;
class QCommandBuilder;
class QVCVUndoCommand;
class QImageProcCommand;
class QCustomFilterDlg;

#define SCROLLBAR_SIZE 20

class QVCVChildWindow : public QWidget
{
	Q_OBJECT
public:
	QVCVChildWindow(QWidget *parent = 0, Qt::WindowFlags f = 0);
	~QVCVChildWindow();

public:

	bool Update(const Mat &image);

	void SetDisplayScale(float scale);
	float GetDisplayScale();

    QFilterPanel* GetFilterPanel(VCV_IMAGE_OPERATION operation);
    QCustomFilterDlg* GetCustomFilterPanel();

public slots:
    void FilterParameterChangeRespond(const CommandParameter *para);
    void FilterPanelOk(const CommandParameter *para);
    void FilterPanelCancel(const CommandParameter *para);
    void CustomFilterParameterChange(const CommandParameter *para);
    void CustomFilterPanelOk(const CommandParameter *para);
    void CustomFilterPanelCancel(const CommandParameter *para);
protected slots:
	void VScrollBarRangeChanged(int min, int max);
	void HScrollBarRangeChanged(int min, int max);

protected:
	virtual void paintEvent(QPaintEvent *e);
	virtual void closeEvent(QCloseEvent *e);
	virtual void resizeEvent(QResizeEvent *e);

protected:
	QImage *update_image;

	QScrollBar *h_scrollbar;
	QScrollBar *v_scrollbar;

    QFilterPanel *filter_panel;
    QCustomFilterDlg *customfilter_panel;

    QCommandBuilder *command_builder;
    QImageProcCommand *filter_command;
    QImageProcCommand *custom_filter_command;
    QVCVData *operator_data;

	int image_top;
	int image_left;
	QRect image_display_area;
	QRect widget_display_area;
	float display_scale;

    void AdjustScrollBar();
    void AdjustScrollBarRange();

    void DrawClient();

    void DoOperation(QControlPanel *panel, QVCVUndoCommand **command, VCV_IMAGE_OPERATION operation);
    void PanelValueChange(QVCVUndoCommand *command, const CommandParameter *para);
    void PanelOk(QVCVUndoCommand **command, const CommandParameter *para);
    void PanelCencel(QVCVUndoCommand **command, const CommandParameter *para);
};

#endif
