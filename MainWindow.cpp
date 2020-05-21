#include <QDebug>
#include <QWindow>
#include <QScreen>
#include <QTimer>
#include <QMouseEvent>
#include <QtMath>
#include <QFileDialog>

#include "MainWindow.h"
#include "ui_MainWindow.h"

typedef enum Sutra_Data_Enum
{
	Sutra_Data_Index,
	Sutra_Data_Char,
	Sutra_Data_Location,

}Sutra_Data_Enum;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	this->m_Scene = new QGraphicsScene(ui->graphicsView);
	this->m_Scene->setBackgroundBrush(Qt::darkGray);

	//紙張大小 (包含邊界)
	this->m_Paper = new QGraphicsRectItem();
	this->m_Paper->setBrush(QBrush(Qt::lightGray));

	//實際紙張大小 (中央繪圖部分，不包含邊界)
	this->m_Boder = new QGraphicsRectItem(this->m_Paper);
	this->m_Boder->setBrush(QBrush(Qt::white));

	//中間底圖
	this->m_Background = new QGraphicsPixmapItem();

	//經文文字
	this->m_Sutra = new QGraphicsItemGroup();

	this->m_Scene->addItem(this->m_Paper);
	this->m_Scene->addItem(this->m_Background);
	this->m_Scene->addItem(this->m_Sutra);

	ui->graphicsView->setScene(this->m_Scene);
	ui->graphicsView->viewport()->installEventFilter(this);
	ui->graphicsView->setMouseTracking(true);

	QTimer::singleShot(0, this, &MainWindow::Windows_Loaded);
}

MainWindow::~MainWindow()
{
	delete ui;
}


int MainWindow::Pixel_to_mm(int Pixel)
{
	QWindow *window = windowHandle();
	QScreen *screen=window->screen();
	double len_mm = Pixel * screen->physicalSize().width() / screen->geometry().width();

	qDebug() << Pixel << "Pixel =" << len_mm << "mm";

	return static_cast<int>(len_mm);
}

int MainWindow::mm_to_Pixel(int mm)
{
	QWindow *window = windowHandle();
	QScreen *screen=window->screen();
	double Pixel = mm * screen->geometry().width() / screen->physicalSize().width();

	qDebug() << mm << "mm =" << Pixel << "Pixel";

	return static_cast<int>(Pixel);
}


void MainWindow::Windows_Loaded()
{
	this->_zoom_factor_base = 1.0015;

	this->m_Back_worker = new Background_Image_Worker();
	this->m_Back_worker->moveToThread(&this->m_Back_thread);
	connect(&this->m_Back_thread, &QThread::started, this->m_Back_worker, &Background_Image_Worker::run);
	connect(&this->m_Back_thread, &QThread::finished, this->m_Back_worker, &Background_Image_Worker::deleteLater);
	connect(this->m_Back_worker, &Background_Image_Worker::Background_Image, this, &MainWindow::Background_Image);

	this->m_Back_thread.start();

	ui->toolBox->setCurrentIndex(2);
}


void MainWindow::Ajust_Paper()
{
	//紙張大小，將 mm(公厘) 轉換成 螢幕上的像素 (pixel)
	int Paper_width_Pixel = this->mm_to_Pixel(ui->spinBox_Paper_Width->value());
	int Paper_height_Pixel = this->mm_to_Pixel(ui->spinBox_Paper_Heigth->value());

	//計算各個 邊界 的寬度大小
	int m_Boder_Left_Width_Pixel = this->mm_to_Pixel(ui->spinBox_Board_Left->value());
	int m_Boder_Right_Width_Pixel = this->mm_to_Pixel(ui->spinBox_Board_Right->value());
	int m_Boder_Top_Height_Pixel = this->mm_to_Pixel(ui->spinBox_Board_Top->value());
	int m_Boder_Bottom_Height_Pixel = this->mm_to_Pixel(ui->spinBox_Board_Bottom->value());

	//邊界 的實際大小
	int m_Boder_width = Paper_width_Pixel - m_Boder_Left_Width_Pixel - m_Boder_Right_Width_Pixel;
	int m_Boder_height = Paper_height_Pixel - m_Boder_Top_Height_Pixel - m_Boder_Bottom_Height_Pixel;
	m_Boder_width = (m_Boder_width>=0)? m_Boder_width : 0;
	m_Boder_height = (m_Boder_height>=0)? m_Boder_height : 0;

	//邊界 的位置與大小
	this->m_Boder->setRect(m_Boder_Left_Width_Pixel, m_Boder_Top_Height_Pixel, m_Boder_width, m_Boder_height);
	this->m_Sutra->setX(m_Boder_Left_Width_Pixel);
	this->m_Sutra->setY(m_Boder_Top_Height_Pixel);

	//紙張 的位置
	this->m_Paper->setRect(0, 0, Paper_width_Pixel, Paper_height_Pixel);
	this->m_Scene->setSceneRect(0, 0, Paper_width_Pixel, Paper_height_Pixel);
	ui->graphicsView->fitInView(0, 0 , Paper_width_Pixel , Paper_height_Pixel, Qt::KeepAspectRatio);
}

void MainWindow::Ajust_Background()
{
	unsigned int Threadhold = ui->horizontalSlider_Background_Threadhold->value();

	if(ui->radioButton_Background_Picture->isChecked())
	{
		this->m_Back_worker->Restart();
	}
	else
	{
		Threadhold = 1;
		QFont font = ui->fontComboBox->currentFont();
		font.setPointSize(ui->spinBox->value());
		QFontMetrics fm(font);
		QString Message = ui->lineEdit_Background_Text->text();
		int Y = fm.ascent();
		ui->horizontalSlider_Font_Space->setMaximum( Y );
		ui->horizontalSlider_Font_Space->setMinimum( -Y );
		if(ui->horizontalSlider_Font_Space->value() > Y )
			ui->horizontalSlider_Font_Space->setValue( Y );
		if(ui->horizontalSlider_Font_Space->value() < -Y)
			ui->horizontalSlider_Font_Space->setValue( -Y );

		int Font_Total_Height = fm.height() + (fm.height() + ui->horizontalSlider_Font_Space->value() ) * ( Message.size() - 1);

		this->m_Background_Image = QPixmap(fm.horizontalAdvance(Message.at(0)), Font_Total_Height);
		this->m_Background_Image.fill(Qt::transparent);
		QPainter painter(&this->m_Background_Image);
		painter.setFont(font);
		painter.setPen(Qt::black);

		for(int Index = 0; Index < Message.size(); Index++)
		{
			painter.drawText(0, Y, Message.at(Index));

			Y += (fm.height() + ui->horizontalSlider_Font_Space->value());
		}

		QPixmap Overlap_Pixmap = this->m_Background_Image.copy(this->m_Background_Image.rect());
		QImage Grayscale_Image = Overlap_Pixmap.toImage();

		for(int y = 0; y < Grayscale_Image.size().height(); y++)
			for(int x = 0; x < Grayscale_Image.size().width(); x++)
			{
				if(Grayscale_Image.pixel(x, y) != 0)
					Grayscale_Image.setPixel(x, y, 0xFF000000);
				else
					Grayscale_Image.setPixel(x, y, 0xFFFFFFFF);
			}

		this->m_Background->setPixmap(QPixmap::fromImage(Grayscale_Image));
	}
}

void MainWindow::Ajust_Sutra()
{
	ui->lineEdit_Sutra->setEnabled(false);
	ui->fontComboBox_Sutra->setEnabled(false);
	ui->spinBox_Sutra_Font->setEnabled(false);
	ui->horizontalSlider_Sutra_FontSpace->setEnabled(false);
	ui->horizontalSlider_Sutra_FontDistance->setEnabled(false);


	QFont font = ui->fontComboBox_Sutra->currentFont();
	font.setPointSize(ui->spinBox_Sutra_Font->value());
	QFontMetrics fm(font);
	int Width = this->m_Boder->rect().width();
	int Height = this->m_Boder->rect().height();
	int Word_Height = fm.height() + ui->horizontalSlider_Sutra_FontSpace->value();
	int Word_Width = fm.height() + ui->horizontalSlider_Sutra_FontDistance->value();
	int Word_X = 0;
	int Word_Y = 0;
	int Max_Word_Y = ( Height - 1)/ Word_Height - 1;
	ui->horizontalSlider_Sutra_overlap->setMaximum(Word_Height * Word_Width);
	bool Overlap = true;

	QPixmap pixmap(this->m_Background->boundingRect().size().toSize());
	pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing);

	QStyleOptionGraphicsItem opt;
	this->m_Background->paint(&painter, &opt, this);

	foreach(QGraphicsItem* Item, this->m_Sutra->childItems())
	{
		Overlap = true;
		QGraphicsSimpleTextItem* Text = qgraphicsitem_cast<QGraphicsSimpleTextItem *>(Item);
		if(Text)
		{
			Text->setFont(font);
			do
			{
				Text->setPos(Width - Word_Width * (Word_X + 1), Word_Height * Word_Y);
				Text->setData(Sutra_Data_Location, QPoint(Word_X, Word_Y));

				Word_Y ++;
				if(Word_Y > Max_Word_Y )
				{
					Word_Y = 0;
					Word_X ++;
				}

				QPointF Position = this->m_Background->mapFromScene(this->m_Sutra->mapToScene(Text->pos()));
				QPixmap Overlap_Pixmap = pixmap.copy(Position.x(), Position.y(), Word_Width, Word_Height);
				if(pixmap.size() == Overlap_Pixmap.size())
					Overlap = false;
				else
					Overlap = Check_Overlap(Overlap_Pixmap);

				qDebug() << Overlap << Text->pos() << Word_X << Word_Y << Text->data(Sutra_Data_Index) << Text->data(Sutra_Data_Char) <<" ============ ";
			}
			while (Overlap);
		}
	}

//	QPointF Position = this->m_Background->mapFromScene(_test->pos());
//	QPixmap box = pixmap.copy(Position.x(), Position.y(), Word_Width, Word_Height);
//	qDebug() << Position << pixmap.width() << pixmap.height() << box.width() << box.height();

//	if(pixmap.size() != box.size())
//		ui->label_test->setPixmap(box);

//	_test->setFont(font);
//	qDebug() << Check_Overlap(box) << "--------------";



	ui->horizontalSlider_Sutra_FontSpace->setEnabled(true);
	ui->horizontalSlider_Sutra_FontDistance->setEnabled(true);
	ui->spinBox_Sutra_Font->setEnabled(true);
	ui->fontComboBox_Sutra->setEnabled(true);
	ui->lineEdit_Sutra->setEnabled(true);
}


bool MainWindow::Check_Overlap(QPixmap Overlap_Pixmap)
{
	int Count = 0;
	QImage Source_Image = Overlap_Pixmap.toImage();
	QImage Grayscale_Image(Overlap_Pixmap.size(), QImage::Format_Grayscale8);
	QPainter painter(&Grayscale_Image);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(Grayscale_Image.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawImage(0, 0, Source_Image);
	painter.end();

	for(int y = 0; y < Grayscale_Image.size().height(); y++)
		for(int x = 0; x < Grayscale_Image.size().width(); x++)
		{
//			qDebug() << x << y << Grayscale_Image.pixel(x, y);
			if((Grayscale_Image.pixel(x, y) & 0xFF)==0) Count++;
		}

//	qDebug() << "Count" << Count;

	return (Count > ui->horizontalSlider_Sutra_overlap->value());
}


// 放大/縮小
bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
	Q_UNUSED(watched)

	if (event->type() == QEvent::MouseMove)
	{
		QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
		QPointF delta = target_viewport_pos - mouse_event->pos();
		if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5)
		{
			target_viewport_pos = mouse_event->pos();
			target_scene_pos = ui->graphicsView->mapToScene(mouse_event->pos());
		}
	}
	else if (event->type() == QEvent::Wheel)
	{
		QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);

		if (wheel_event->orientation() == Qt::Vertical)
		{
			double angle = wheel_event->angleDelta().y();
			double factor = qPow(_zoom_factor_base, angle);
			gentle_zoom(factor);
			return true;
		}
	}

	return false;
}
void MainWindow::gentle_zoom(double factor)
{
	ui->graphicsView->scale(factor, factor);
	ui->graphicsView->centerOn(target_scene_pos);
}




void MainWindow::on_toolBox_currentChanged(int index)
{
	switch (index)
	{
		case 0:
		{
			this->m_Background->setFlags(nullptr);

			this->m_Sutra->show();

			break;
		}
		case 1:
		{
			this->m_Sutra->hide();
			this->m_Back_worker->setBoder_Size(QSize(this->m_Boder->boundingRect().width(),
								 this->m_Boder->boundingRect().height()));
			this->m_Background->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);

			this->Ajust_Background();

			this->m_Background->show();
			this->m_Paper->show();

			break;
		}
		case 2:
		{
			this->m_Sutra->hide();
			this->m_Background->hide();

			this->Ajust_Paper();

			this->m_Paper->show();

			break;
		}
	}
}


// 紙張大小
void MainWindow::on_spinBox_Paper_Heigth_valueChanged(int arg1)
{
	Q_UNUSED(arg1)

	this->Ajust_Paper();
}
void MainWindow::on_spinBox_Paper_Width_valueChanged(int arg1)
{
	Q_UNUSED(arg1)

	this->Ajust_Paper();
}
void MainWindow::on_spinBox_Board_Top_valueChanged(int arg1)
{
	Q_UNUSED(arg1)

	this->Ajust_Paper();
}
void MainWindow::on_spinBox_Board_Bottom_valueChanged(int arg1)
{
	Q_UNUSED(arg1)

	this->Ajust_Paper();
}
void MainWindow::on_spinBox_Board_Left_valueChanged(int arg1)
{
	Q_UNUSED(arg1)

	this->Ajust_Paper();
}
void MainWindow::on_spinBox_Board_Right_valueChanged(int arg1)
{
	Q_UNUSED(arg1)

	this->Ajust_Paper();
}

// 背景 圖片
void MainWindow::on_radioButton_Background_Text_clicked()
{
	this->m_Back_worker->setIsText(true);
	ui->groupBox_Background_Text->show();
	ui->groupBox_Background_Picture->hide();
}
void MainWindow::on_radioButton_Background_Picture_clicked()
{
	this->m_Back_worker->setIsText(false);
	ui->groupBox_Background_Text->hide();
	ui->groupBox_Background_Picture->show();
}
void MainWindow::on_pushButton_Background_File_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("選擇圖片"),
							QApplication::applicationDirPath(),
							tr("圖片檔 (*.png *.jpg *.bmp)"));

	ui->lineEdit_Background->setText(fileName);
	if(!fileName.isEmpty())
	{
		this->m_Background_Image = QPixmap(fileName);
		this->m_Back_worker->setFilename(fileName);
		ui->horizontalSlider_Background_Size->setValue(100);

		qDebug() << "load file:" << fileName;
	}
}
void MainWindow::on_horizontalSlider_Background_Size_valueChanged(int value)
{
	this->m_Back_worker->setImage_Scale_Factor(value);

	this->Ajust_Background();
}
void MainWindow::on_horizontalSlider_Background_Threadhold_valueChanged(int value)
{
	this->m_Back_worker->setThreadhold(value);

	this->Ajust_Background();
}

// 背景 文字
void MainWindow::on_lineEdit_Background_Text_textEdited(const QString &arg1)
{
	Q_UNUSED(arg1)

	this->Ajust_Background();
}
void MainWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
	Q_UNUSED(f)

	this->Ajust_Background();
}
void MainWindow::on_spinBox_valueChanged(int arg1)
{
	Q_UNUSED(arg1)

	this->Ajust_Background();
}
void MainWindow::on_horizontalSlider_Font_Space_valueChanged(int value)
{
	Q_UNUSED(value)

	this->Ajust_Background();
}


void MainWindow::on_pushButton_Sutra_File_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("選擇經文"),
							QApplication::applicationDirPath(),
							tr("文字檔 (*.txt)"));

	ui->lineEdit_Sutra->setText(fileName);
	if(!fileName.isEmpty())
	{
		QFile Sutra(fileName);
		Sutra.open(QIODevice::ReadOnly);
		QString Sutra_Content = Sutra.readAll();
		Sutra.close();

		for(int Index = 0; Index < Sutra_Content.size(); Index++)
		{
			QString Word = Sutra_Content.at(Index);
			QGraphicsSimpleTextItem* Text = new QGraphicsSimpleTextItem(Word, this->m_Sutra);
			Text->setData(Sutra_Data_Index, Index);
			Text->setData(Sutra_Data_Char, Word);
			Text->setData(Sutra_Data_Location, QPoint(0, 0));
			Text->setPos(0, 0);
		}

		_test = this->m_Scene->addSimpleText("中");
		_test->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);


		this->Ajust_Sutra();
	}
}

void MainWindow::on_fontComboBox_Sutra_currentFontChanged(const QFont &f)
{
	Q_UNUSED(f)

	this->Ajust_Sutra();
}

void MainWindow::on_spinBox_Sutra_Font_valueChanged(int arg1)
{
	Q_UNUSED(arg1)

	this->Ajust_Sutra();
}

void MainWindow::on_horizontalSlider_Sutra_FontSpace_valueChanged(int value)
{
	Q_UNUSED(value)

	this->Ajust_Sutra();
}

void MainWindow::on_horizontalSlider_Sutra_FontDistance_valueChanged(int value)
{
	Q_UNUSED(value)

	this->Ajust_Sutra();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
	if(Qt::Checked == arg1 )
		this->m_Background->hide();
	else
		this->m_Background->show();
}

void MainWindow::Background_Image(QImage image)
{
	qDebug() << "get Background_Image";

	this->m_Background->setPixmap(QPixmap::fromImage(image).copy());
}

void MainWindow::on_pushButton_clicked()
{
	this->m_Back_worker->Restart();
}
