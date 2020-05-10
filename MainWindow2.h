#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QMainWindow>
#include <QGraphicsItem>


namespace Ui {
	class MainWindow2;
}

class MainWindow2 : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow2(QWidget *parent = nullptr);
	~MainWindow2();

	void gentle_zoom(double factor);

private slots:
	void on_pushButton_clicked();

	bool eventFilter(QObject* object, QEvent* event);

	int Pixel_to_mm(int Pixel);
	int mm_to_Pixel(int mm);

	QPixmap QPixmapFromItem(QGraphicsItem *item);

	void on_pushButton_2_clicked();

	void on_pushButton_3_clicked();

	void on_horizontalSlider_valueChanged(int value);

private:
	Ui::MainWindow2 *ui;
	QPointF target_scene_pos;
	QPointF target_viewport_pos;
	double _zoom_factor_base;

	QGraphicsPixmapItem* pic;
	QGraphicsPixmapItem* pic2;
};

#endif // MAINWINDOW2_H
