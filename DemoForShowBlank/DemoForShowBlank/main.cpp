#include "ShowTheState.h"
#include <QtWidgets/QApplication>
#include <QtCore/QTranslator>
#include <QtCore/QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	//Õº±Í
	a.setWindowIcon(QIcon("./Logo.png"));
	//º”‘ÿ∑≠“Î
	QTranslator qtTrans;
	//qtTrans.load("D://DEMO//DemoForBroExcel//ShowTheState//ShowTheState//showthestate_zh.qm");
	qtTrans.load("./demoforshowblank_zh.qm");
	a.installTranslator(&qtTrans);

    ShowTheState w;
    w.show();
    return a.exec();
}
