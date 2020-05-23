#include "MainWindow_1.h"
#include "MainWindow2.h"
#include "MainWindow3.h"
#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
