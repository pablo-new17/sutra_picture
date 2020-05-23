#ifndef SUTRA_IMAGE_WORKER_H
#define SUTRA_IMAGE_WORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QImage>
#include <QGraphicsSimpleTextItem>

class Sutra_Image_Worker : public QObject
{
	Q_OBJECT
public:
	explicit Sutra_Image_Worker(QObject *parent = nullptr);
	~Sutra_Image_Worker();

	void setImage(const QImage& image);

public slots:
	void run();
	void Stop();
	void Restart();

signals:
	void Sutra_word(QGraphicsSimpleTextItem Word);

private:
	QImage			m_Background_Image;
	QPoint			m_Background_Position;
	QGraphicsItemGroup*	m_Sutra;


	bool		m_Stop;
	bool		m_Restart;

	QMutex		m_Mutex;
	QWaitCondition	m_Condition;
};


#endif // SUTRA_IMAGE_WORKER_H
