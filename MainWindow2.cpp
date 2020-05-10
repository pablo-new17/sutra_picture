#include <QDebug>
#include <QPaintDevice>
#include <QWindow>
#include <QScreen>
#include <QPixmap>

#include <QMouseEvent>
#include <QtMath>

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QStyleOptionGraphicsItem>
#include <QFontMetrics>

#include "MainWindow2.h"
#include "ui_MainWindow2.h"

MainWindow2::MainWindow2(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow2)
{
	ui->setupUi(this);
	ui->graphicsView->viewport()->installEventFilter(this);
	ui->graphicsView->setMouseTracking(true);

	_zoom_factor_base = 1.0015;

}

MainWindow2::~MainWindow2()
{
	delete ui;
}

void MainWindow2::gentle_zoom(double factor)
{
	ui->graphicsView->scale(factor, factor);
	ui->graphicsView->centerOn(target_scene_pos);
	QPointF delta_viewport_pos = target_viewport_pos -
				     QPointF(ui->graphicsView->viewport()->width() / 2.0,
					     ui->graphicsView->viewport()->height() / 2.0);
	QPointF viewport_center = ui->graphicsView->mapFromScene(target_scene_pos) - delta_viewport_pos;

	ui->graphicsView->centerOn(ui->graphicsView->mapToScene(viewport_center.toPoint()));
}

bool MainWindow2::eventFilter(QObject* object, QEvent* event)
{
	Q_UNUSED(object)

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
//		if (QApplication::keyboardModifiers() == _modifiers)
		{
			if (wheel_event->orientation() == Qt::Vertical)
			{
				double angle = wheel_event->angleDelta().y();
				double factor = qPow(_zoom_factor_base, angle);
				gentle_zoom(factor);
				return true;
			}
		}
	}

	return false;
}


void MainWindow2::on_pushButton_clicked()
{
	QGraphicsScene *pScene = new QGraphicsScene();
	pScene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
	ui->graphicsView->setScene(pScene);

	QString Message = QString("中文字，直式書寫");
	QFont f = ui->fontComboBox->currentFont();
	f.setPointSize(ui->spinBox->value());
	QFontMetrics fm(f);

	QPointF point(100, 0);
	for(int Index = 0; Index < Message.length(); Index++)
	{
		QGraphicsSimpleTextItem *textItem = new QGraphicsSimpleTextItem(Message.at(Index));

		textItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemAcceptsInputMethod);
		textItem->setFont(f);
		pScene->addItem(textItem);

		point.setX(100);
		point.setY(0 + Index * fm.height());

		QPointF point_item = textItem->mapFromScene(point);
		qDebug() << point_item;
		textItem->setPos(point_item);

//		textItem->setPos(100, 0 + Index * fm.height());

	}

	QGraphicsRectItem *rectItem = new QGraphicsRectItem(0, 0, fm.height(), fm.height()*Message.length());
//	QGraphicsRectItem *rectItem = new QGraphicsRectItem(100, 0, 200, 200);
	rectItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);
//	rectItem->setPos(100, 0);

	QPen pen = rectItem->pen();
	pen.setWidth(2);
	pen.setColor(QColor(255, 0, 0));
	rectItem->setPen(pen);
	rectItem->setBrush(QColor(128, 255, 0, 128));




	pScene->addItem(rectItem);

	point.setX(100);
	point.setY(0);
	QPointF point_item = rectItem->mapFromScene(point);
	qDebug() << point_item;
	rectItem->setPos(point_item);

//	pScene->addItem(textItem);
//	pScene->addItem(textItem2);


	qDebug() << Pixel_to_mm(1080);
	qDebug() << mm_to_Pixel(1800);
}


int MainWindow2::Pixel_to_mm(int Pixel)
{
	QWindow *window=windowHandle();
	QScreen *screen=window->screen();
	double len_mm = Pixel * screen->physicalSize().width() / screen->geometry().width();

	qDebug() << "Pixel :" << Pixel << " = " << len_mm << "mm";

	return static_cast<int>(len_mm);
}

int MainWindow2::mm_to_Pixel(int mm)
{
	QWindow *window=windowHandle();
	QScreen *screen=window->screen();
	double Pixel = mm * screen->geometry().width() / screen->physicalSize().width();

	qDebug() << "Pixel :" << Pixel << " = " << mm << "mm";

	return static_cast<int>(Pixel);
}

QPixmap MainWindow2::QPixmapFromItem(QGraphicsItem* item)
{
	QPixmap pixmap(item->boundingRect().size().toSize());
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing);
	QStyleOptionGraphicsItem opt;
	painter.translate (-1 * (item->boundingRect ().topLeft ()));//THIS IS MANDATORY IN SOME CASES.
	item->paint(&painter, &opt);
	return pixmap;
}

void MainWindow2::on_pushButton_2_clicked()
{
	foreach( QGraphicsItem* item, ui->graphicsView->scene()->items())
	{
		qDebug() << item->mapToScene(item->pos());

		if( QGraphicsSimpleTextItem* TextItem = qgraphicsitem_cast<QGraphicsSimpleTextItem *>(item) )
		{

			qDebug() << "text" << TextItem->scenePos();

			ui->label->setPixmap(QPixmapFromItem(TextItem));
		}
	}

}

void MainWindow2::on_pushButton_3_clicked()
{
	QGraphicsScene* scene = ui->graphicsView->scene();
	pic = new QGraphicsPixmapItem(QPixmap("C:/Users/pablo/Downloads/S__2826273.jpg"));
	pic->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);
	pic->hide();
	scene->addItem(pic);

	pic2 = new QGraphicsPixmapItem(QPixmap("C:/Users/pablo/Downloads/S__2826273.jpg"));
	pic2->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);
//	pic2->hide();
	scene->addItem(pic2);

	this->on_horizontalSlider_valueChanged(127);
}

void MainWindow2::on_horizontalSlider_valueChanged(int value)
{
	QImage img = pic->pixmap().toImage();
//	img = img.scaled(100, 100, Qt::KeepAspectRatio);

	QImage fixedImage(img.size(), QImage::Format_Grayscale8);
	QPainter painter(&fixedImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(fixedImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawImage(0, 0, img);
	painter.end();

	for(int y=0; y<fixedImage.size().height(); y++)
		for(int x=0; x<fixedImage.size().width(); x++)
		{
//			qDebug() << x << y << (fixedImage.pixel(x,y) & 0xFF);

			if((fixedImage.pixel(x,y) & 0xFF) < value)
				fixedImage.setPixel(x,y,0xFF000000);
			else
				fixedImage.setPixel(x,y,0xFFFFFFFF);
		}

//	ui->label->setPixmap(QPixmap::fromImage(fixedImage));

	pic2->setPixmap(QPixmap::fromImage(fixedImage));
}
