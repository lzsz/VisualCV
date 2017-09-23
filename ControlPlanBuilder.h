#ifndef CONTROLPLANBUILDER_H
#define CONTROLPLANBUILDER_H

#include <QObject>
#include <vector>
#include "SystemDefine.h"

using namespace std;

class QWidget;
class QLabel;
class QGroupBox;
class QCheckBox;
class QGridLayout;
class QSpinBox;
class QDoubleSpinBox;
class QPushButton;




class QControlPlanBuilder : public QObject
{
public:
    explicit QControlPlanBuilder();
    ~QControlPlanBuilder();


private:


};

#endif // CONTROLPLANBUILDER_H
