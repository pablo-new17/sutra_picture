#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsItem>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow() override;

private slots:
	void on_toolBox_currentChanged(int index);

	void on_spinBox_Paper_Heigth_valueChanged(int arg1);

	void on_spinBox_Paper_Width_valueChanged(int arg1);

	void on_spinBox_Board_Top_valueChanged(int arg1);

	void on_spinBox_Board_Bottom_valueChanged(int arg1);

	void on_spinBox_Board_Left_valueChanged(int arg1);

	void on_spinBox_Board_Right_valueChanged(int arg1);


	void UI_Shown();
	void gentle_zoom(double factor);

	void Ajust_Paper();

	void Ajust_Background();

	void Ajust_Sutra();

	void on_radioButton_Background_Text_clicked();

	void on_radioButton_Background_Picture_clicked();

	void on_pushButton_Background_File_clicked();

	void on_horizontalSlider_Background_Size_valueChanged(int value);

	void on_horizontalSlider_Background_Threadhold_valueChanged(int value);

	void on_lineEdit_Background_Text_textEdited(const QString &arg1);

	void on_fontComboBox_currentFontChanged(const QFont &f);

	void on_spinBox_valueChanged(int arg1);

	void on_horizontalSlider_Font_Space_valueChanged(int value);

	void on_pushButton_Sutra_File_clicked();

	void on_fontComboBox_Sutra_currentFontChanged(const QFont &f);

	void on_spinBox_Sutra_Font_valueChanged(int arg1);

	void on_horizontalSlider_Sutra_FontSpace_valueChanged(int value);

	void on_horizontalSlider_Sutra_FontDistance_valueChanged(int value);

	void on_checkBox_stateChanged(int arg1);

private:
	Ui::MainWindow *ui;
	int Pixel_to_mm(int Pixel);
	int mm_to_Pixel(int mm);

	QPointF target_scene_pos;
	QPointF target_viewport_pos;
	double _zoom_factor_base;
	QPixmap m_Background_Image;

	QGraphicsScene*		m_Scene;
	QGraphicsRectItem*	m_Paper;
	QGraphicsRectItem*	m_Boder;
	QGraphicsPixmapItem*	m_Background;
	QGraphicsItemGroup*	m_Sutra;

	QGraphicsSimpleTextItem* _test;
private:
	bool Check_Overlap(QPixmap Overlap_Pixmap);
	// QObject interface
public:
	bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // MAINWINDOW_H
