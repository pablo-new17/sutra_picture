#include <QDebug>
#include <QMouseEvent>

#include "PLable.h"

PLable::PLable(QWidget* parent, Qt::WindowFlags f)
	: QLabel(parent, f)
{
	Drag = false;
	setMouseTracking(true);
	rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
}

void PLable::mousePressEvent(QMouseEvent* ev)
{
	Drag = true;
	start.setX(ev->x());
	start.setY(ev->y());
}

void PLable::mouseReleaseEvent(QMouseEvent* ev)
{
	Drag = false;
	end.setX(ev->x());
	end.setY(ev->y());

	rubberBand->hide();
	image = new QPixmap(end.x()-start.x(), end.y()-start.y());
	QLabel::render(image, QPoint(0,0), QRegion(start.x(), start.y(), end.x()-start.x(), end.y()-start.y())); //copy the selected part into "image"
	rubberBand->show();
}

void PLable::mouseMoveEvent(QMouseEvent* ev)
{
	if(Drag == true)
	{
		rubberBand->setGeometry(start.x(), start.y(), ev->x()-start.x(), ev->y()-start.y());
		rubberBand->show();
	}
}

QPixmap* PLable::getImage() const
{
	return image;
}

void PLable::set_Box(int X, int Y, int W, int H)
{
	rubberBand->hide();
	image = new QPixmap(W, H);
	QLabel::render(image, QPoint(0,0), QRegion(X, Y, W, H)); //copy the selected part into "image"

	rubberBand->setGeometry(X, Y, W, H);
	rubberBand->show();
}
