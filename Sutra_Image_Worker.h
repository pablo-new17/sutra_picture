#ifndef SUTRA_IMAGE_WORKER_H
#define SUTRA_IMAGE_WORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QImage>
#include <QGraphicsSimpleTextItem>
#include <QFont>

class Sutra_Image_Worker : public QObject
{
	Q_OBJECT
public:
	explicit Sutra_Image_Worker(QObject *parent = nullptr);
	~Sutra_Image_Worker();

	QImage getBackground_Image() const;
	void setBackground_Image(const QImage& Background_Image);

	QPoint getBackground_Position() const;
	void setBackground_Position(const QPoint& Background_Position);

	QFont getFont() const;
	void setFont(const QFont& Font);

	int getOverlap_Value() const;
	void setOverlap_Value(int Overlap_Value);

	QSize getBorder() const;
	void setBorder(const QSize& Border);

	int getFont_Space() const;
	void setFont_Space(int Font_Space);

	int getFont_Distance() const;
	void setFont_Distance(int Font_Distance);

	QGraphicsItemGroup* getSutra() const;
	void setSutra(QGraphicsItemGroup* Sutra);

	QPoint getBorder_Position() const;
	void setBorder_Position(const QPoint& Border_Position);

public slots:
	void run();
	void Stop();
	void Restart();

signals:
	void Sutra_word(QGraphicsSimpleTextItem* Word);

private:
	QImage			m_Background_Image;
	QPoint			m_Background_Position;
	QGraphicsItemGroup*	m_Sutra;
	QFont			m_Font;
	QSize			m_Border;
	QPoint			m_Border_Position;
	int			m_Font_Space;
	int			m_Font_Distance;
	int			m_Overlap_Value;

private:	//common for Worker
	bool		m_Stop;
	bool		m_Restart;

	QMutex		m_Mutex;
	QWaitCondition	m_Condition;

private slots:
	bool collision(QPoint ObjA_XY, QSize ObjA_Size, QPoint Checked_Obj_XY, QSize Checked_Obj_Size);
};


#endif // SUTRA_IMAGE_WORKER_H
