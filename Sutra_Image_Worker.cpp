#include <QDebug>
#include <QThread>

#include "Sutra_Image_Worker.h"

Sutra_Image_Worker::Sutra_Image_Worker(QObject *parent)
	: QObject(parent)
{
	qDebug() << "Start Worker";

	this->m_Stop = false;
	this->m_Restart = false;
}

Sutra_Image_Worker::~Sutra_Image_Worker()
{
//	this->m_Condition.wakeOne();

	this->m_Mutex.lock();
	this->m_Stop = true;
	this->m_Mutex.unlock();

	this->m_Condition.wakeAll();

	qDebug() << "end of Worker";
}

void Sutra_Image_Worker::run()
{
	qDebug() << "run Worker";

	forever
	{
		this->m_Mutex.lock();
		QImage img = this->m_image.copy();
		int Count = 0;		//PABLO test
		this->m_Mutex.unlock();


		for(int Y=0; Y<img.height(); Y++)
		{
//			QString Output;

			for(int X=0; X<img.width(); X++)
			{
				if(this->m_Stop)	return;
				if(this->m_Restart)	break;

//				qDebug() << "(" << X << "," << Y << ") =" << QString::number(img.pixel(X, Y), 16).toUpper();

				if(img.pixel(X, Y) != 0xFFFFFFFF)
//				{
					Count++;
//					Output.append("*");
//				}
//				else
//					Output.append(" ");
			}

//			qDebug() << Output;
		}

		qDebug() << "send signal" << Count << img.width() << img.height();	//PABLO test
		emit Send_Signal(Count);	//PABLO test

		this->m_Mutex.lock();
		if(!this->m_Restart)
			this->m_Condition.wait(&this->m_Mutex);
		this->m_Restart = false;
		this->m_Mutex.unlock();
	}

	qDebug() << "done Worker";
}

void Sutra_Image_Worker::Stop()
{
	qDebug() << "Stop Worker";

	this->m_Mutex.lock();
	this->m_Stop = true;
	this->m_Mutex.unlock();

	qDebug() << "Stop Worker 2";

	this->m_Condition.wakeOne();
}

void Sutra_Image_Worker::Restart()
{
	qDebug() << "Restart Worker";

	this->m_Mutex.lock();
	this->m_Stop = false;
	this->m_Restart = true;
	this->m_Mutex.unlock();

	qDebug() << "Restart Worker 2";

	this->m_Condition.wakeOne();
}

void Sutra_Image_Worker::setImage(const QImage& image)
{
	qDebug() << "setImage";

	this->m_Mutex.lock();
	m_image = image;
	this->m_Mutex.unlock();

	qDebug() << "setImage done";
}
