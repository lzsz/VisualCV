#ifndef CONTROLPLAN_H
#define CONTROLPLAN_H

#include <QWidget>

#include "ui_FilterPanel.h"
#include "SystemDefine.h"


using namespace std;

class QRadioButton;

class QFilterPanel : public QWidget
{
	Q_OBJECT
public:
    explicit QFilterPanel(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~QFilterPanel();

    double GetSigmaColor();
    double GetSigmaSpace();
    double GetScale();
    double GetDelta();
    Size GetSize();
    Point GetPoint();
    int GetIterations();
    int GetKSize();
	int GetBorderType();

	void Disable();

	void BeginOperation(VCV_IMAGE_OPERATION operation);
	void EndOperation(VCV_IMAGE_OPERATION operation);

protected slots:
	void ValueChange();
	void PushOk();
	void PushCancel();


signals:
    void ParameterChange(const CommandParameter *para);
    void FilterPanelOk(const CommandParameter *para);
    void FilterPanelCancel(const CommandParameter *para);

private:
    Ui_FilterPanel ui;

	QRadioButton *rb_default;
	QRadioButton *rb_wrap;
	QRadioButton *rb_replicate;
	QRadioButton *rb_reflect;
	QRadioButton *rb_replicate101;
	QRadioButton *rb_reflect101;
	QRadioButton *rb_constant;
	QRadioButton *rb_transparent;
	QRadioButton *rb_isolated;

	VCV_IMAGE_OPERATION image_operation;

    CommandParameter_Filter *command_para;
	void CreateBorderType();
	void GetAllParameter();

	void EnableBlur();
	void DisableBlur();

	void EnableGaussianBlur();
	void DisableGaussianBlur();

	void EnableMedianBlur();
	void DisableMedianBlur();

	void EnableBilateral();
	void DisableBilateral();

	void EnableErosion();
	void DisableErosion();

	void EnableDilation();
	void DisableDilation();
};

#endif // CONTROLPLAN_H
