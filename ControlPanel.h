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

protected slots:
    virtual void ValueChange();
    virtual void PushOk();
    virtual void PushCancel();
signals:
    void ParameterChange(const CommandParameter *para);
    void PanelOk(const CommandParameter *para);
    void PanelCancel(const CommandParameter *para);

protected:
    virtual void GetAllParameter();

};

#endif // CONTROLPANEL_H
