#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include "SystemDefine.h"

class QControlPanel : public QWidget
{
    Q_OBJECT
public:
    explicit QControlPanel(QWidget *parent = nullptr);
    ~QControlPanel();

    virtual void BeginOperation(VCV_IMAGE_OPERATION operation);
    virtual void EndOperation(VCV_IMAGE_OPERATION operation);

signals:

public slots:
};

#endif // CONTROLPANEL_H
