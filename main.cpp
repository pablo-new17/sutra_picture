#include "MainWindow_1.h"
#include "MainWindow2.h"
#include "MainWindow3.h"
#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
//	MainWindow2 w2;

//	MainWindow3 w3;

	MainWindow w;


//	w2.show();
//	w3.show();

	w.show();

	return a.exec();
}
