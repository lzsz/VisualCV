#include "AboutDialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    connect(ui->pb_ok,SIGNAL(clicked(bool)),this,SLOT(PushOk()));
    setFixedSize(this->width(),this->height());
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::PushOk()
{
    close();
}
