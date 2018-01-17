#ifndef EDGEDETECTIONPANEL_H
#define EDGEDETECTIONPANEL_H

#include <QWidget>
#include "ControlPanel.h"
#include "ui_edgedetectionpanel.h"

class QEdgeDetectionPanel : public QControlPanel
{
public:
    explicit QEdgeDetectionPanel(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~QEdgeDetectionPanel();

    int GetDepth();
    int GetKSize();
    int GetDx();
    int GetDy();
    int GetApertureSize();
    bool GetL2gradient();
    double GetScale();
    double GetDelta();
    double GetThreshold1();
    double GetThreshold2();

    void DisableAll();

    virtual void BeginOperation(VCV_IMAGE_OPERATION operation);
    virtual void EndOperation(VCV_IMAGE_OPERATION operation);

protected:
    Ui_EdgeDetectionPanel ui;

protected:
    virtual void GetAllParameter();
private:
    void EnableLaplacian();
    void EnableSobel();
    void EnableCanny();
    void SetConnection();
};

#endif // EDGEDETECTIONPANEL_H
