#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRubberBand>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_lineEdit_textEdited(const QString &arg1);

	void on_fontComboBox_currentFontChanged(const QFont &f);

	void on_spinBox_valueChanged(int arg1);

	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

private:
	Ui::MainWindow *ui;
	QRubberBand* rubberBand;

	void Font_Information();
};
#endif // MAINWINDOW_H
