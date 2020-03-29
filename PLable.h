#ifndef PLABLE_H
#define PLABLE_H

#include <QLabel>
#include <QMouseEvent>
#include <QRubberBand>

class PLable : public QLabel
{
public:
	PLable(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

	void mousePressEvent(QMouseEvent *ev) override;
	void mouseReleaseEvent(QMouseEvent *ev) override;
	void mouseMoveEvent(QMouseEvent *ev) override;

	QPixmap* getImage() const;

	void set_Box(int X, int Y, int W, int H);

private:
	QPoint start;
	QPoint end;
	bool Drag;
	QRubberBand* rubberBand;
	QPixmap* image;
};

#endif // PLABLE_H
