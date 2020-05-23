#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QThread>

#include "Background_Image_Worker.h"
#include "Sutra_Image_Worker.h"

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

	// 紙張大小
	void on_spinBox_Paper_Heigth_valueChanged(int arg1);
	void on_spinBox_Paper_Width_valueChanged(int arg1);
	void on_spinBox_Board_Top_valueChanged(int arg1);
	void on_spinBox_Board_Bottom_valueChanged(int arg1);
	void on_spinBox_Board_Left_valueChanged(int arg1);
	void on_spinBox_Board_Right_valueChanged(int arg1);

	// 背景 圖片
	void on_radioButton_Background_Text_clicked();
	void on_radioButton_Background_Picture_clicked();
	void on_pushButton_Background_File_clicked();
	void on_horizontalSlider_Background_Size_valueChanged(int value);
	void on_horizontalSlider_Background_Threadhold_valueChanged(int value);

	// 背景 文字
	void on_lineEdit_Background_Text_textEdited(const QString &arg1);
	void on_fontComboBox_Backgraund_Text_currentFontChanged(const QFont &f);
	void on_spinBox_Background_Text_Font_Size_valueChanged(int arg1);
	void on_horizontalSlider_Font_Space_valueChanged(int value);
	void on_pushButton_Background_Show_clicked();

	//經文
	void on_pushButton_Sutra_File_clicked();

	void on_fontComboBox_Sutra_currentFontChanged(const QFont &f);

	void on_spinBox_Sutra_Font_valueChanged(int arg1);

	void on_horizontalSlider_Sutra_FontSpace_valueChanged(int value);

	void on_horizontalSlider_Sutra_FontDistance_valueChanged(int value);

	void on_checkBox_stateChanged(int arg1);

private slots:

	void Windows_Loaded();

	void Background_Image(QImage image);
	void Sutra_word(QGraphicsSimpleTextItem Word);

	void gentle_zoom(double factor);

	void Ajust_Paper();

	void Ajust_Background();

	void Ajust_Sutra();


	void on_pushButton_clicked();

private:
	Ui::MainWindow *ui;
	int Pixel_to_mm(int Pixel);
	int mm_to_Pixel(int mm);

	QPointF target_scene_pos;
	QPointF target_viewport_pos;
	double _zoom_factor_base;
	QPixmap m_Background_Image;

	QGraphicsScene*			m_Scene;
	QGraphicsRectItem*		m_Paper;
	QGraphicsRectItem*		m_Boder;
	QGraphicsPixmapItem*		m_Background;
	QGraphicsItemGroup*		m_Sutra;

	QThread				m_Back_thread;
	Background_Image_Worker*	m_Back_worker;
	QThread				m_Sutra_thread;
	Sutra_Image_Worker*		m_Sutra_worker;


	QGraphicsSimpleTextItem*	_test;


private:
	bool Check_Overlap(QPixmap Overlap_Pixmap);
	// QObject interface
public:
	bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // MAINWINDOW_H
