#ifndef MAINWINDOW3_H
#define MAINWINDOW3_H

#include <QMainWindow>

namespace Ui {
	class MainWindow3;
}

class MainWindow3 : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow3(QWidget *parent = nullptr);
	~MainWindow3();

private slots:
	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

	void on_pushButton_3_clicked();

private:
	Ui::MainWindow3 *ui;
};

#endif // MAINWINDOW3_H
