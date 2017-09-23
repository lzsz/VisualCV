#ifndef NEWDIALOG_H
#define NEWDIALOG_H
#include "ui_NewDialog.h"

class QNewDialog : public QDialog
{
	Q_OBJECT
public:
	explicit QNewDialog();
	~QNewDialog();

	QString GetName();
	int GetWidth();
	int GetHeight();
	int GetResolution();
	int GetMode();
	int GetContent();

protected slots:
	void PushOk();
	void PushCancel();
	void ModeChange(int index);
protected:
	Ui_NewDialog ui;

};
#endif