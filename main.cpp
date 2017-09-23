#include <QApplication>

#include "VCVMainWindow.h"

int main(int argc, char **argv)
{
	QApplication app(argc,argv);
	
	
	QVCVMainWindow vcvmw;

	vcvmw.show();

    return app.exec();
}
