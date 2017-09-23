#ifndef CUSTOMFILTERDLG_H
#define CUSTOMFILTERDLG_H

#include <QDialog>

#include "SystemDefine.h"
#include "delegate.h"
#include "ui_CustomFilter.h"

#define CUSTOM_FILTER_TABLEWIDGET_ITEM_WIDTH 40

class QCustomFilterDlg : public QDialog
{
	Q_OBJECT
public:
	QCustomFilterDlg();
	~QCustomFilterDlg();

	bool IsInit();
	bool Initialize(const Mat &mat);

	Point GetAnchor();
	int GetDepth();
	double GetDelta();
	Mat GetKernel();
protected slots:
	void PushOk();
	void PushCancel();
	void PushApply();
	void KernelSizeChange();

protected:
	SpinBoxDelegate sp_delegate;
	Ui_CustomFilter2D ui;
	
	bool initialization;

	Mat src_mat;
	Mat dst_mat;
	Mat kernel;

	virtual void paintEvent(QPaintEvent *e);

	void InitUI();

	void GetKernelFromUI();
};

#endif