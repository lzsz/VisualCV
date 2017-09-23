#include <QDateTime>
#include <QPushButton>
#include "NewDialog.h"

QNewDialog::QNewDialog()
{
	ui.setupUi(this);
	ui.name->setText(tr("NewImage-")+QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm:ss"));

	connect(ui.buttonBox->button(QDialogButtonBox::Ok),SIGNAL(clicked()),this,SLOT(PushOk()));
	connect(ui.buttonBox->button(QDialogButtonBox::Cancel),SIGNAL(clicked()),this,SLOT(PushCancel()));

	connect(ui.mode,SIGNAL(currentIndexChanged(int)),this,SLOT(ModeChange(int)));
}

QNewDialog::~QNewDialog()
{

}

QString QNewDialog::GetName()
{
	return ui.name->text();
}

int QNewDialog::GetWidth()
{
	return ui.width->text().toInt();
}

int QNewDialog::GetHeight()
{
	return ui.height->text().toInt();
}

int QNewDialog::GetResolution()
{
	return ui.resolution->text().toInt();
}

int QNewDialog::GetMode()
{
	return ui.mode->modelColumn();
}

int QNewDialog::GetContent()
{
	if(ui.content_white->isChecked())
		return 1;
	if(ui.content_black->isChecked())
		return 2;
	if(ui.content_transparent->isChecked())
		return 3;

	return 0;
}

void QNewDialog::PushOk()
{
	accept();
}

void QNewDialog::PushCancel()
{
	reject();
}

void QNewDialog::ModeChange(int index)
{
	if(index==1)
	{
		if(ui.content_transparent->isChecked())
			ui.content_white->setChecked(true);

		ui.content_transparent->setEnabled(false);
	}
	else
	{
		ui.content_transparent->setEnabled(true);
	}
}