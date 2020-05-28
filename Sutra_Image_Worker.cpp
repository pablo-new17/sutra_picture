#include <QDebug>
#include <QThread>
#include <QFontMetrics>
#include <QtGlobal>

#include "Sutra_Image_Worker.h"
#include "Sutra_Word_Data.h"

Sutra_Image_Worker::Sutra_Image_Worker(QObject *parent)
	: QObject(parent)
{
	qDebug() << "Start Sutra_Image_Worker";

	this->m_Stop = false;
	this->m_Restart = false;
	this->m_Sutra = nullptr;

	this->m_Font = QFont("新細明體", 14);
	this->m_Font_Space = 0;
	this->m_Font_Distance = 0;
	this->m_Overlap_Value = 0;
}

Sutra_Image_Worker::~Sutra_Image_Worker()
{
//	this->m_Condition.wakeOne();

	this->m_Mutex.lock();
	this->m_Stop = true;
	this->m_Mutex.unlock();

	this->m_Condition.wakeAll();

	qDebug() << "end of Sutra_Image_Worker";
}

void Sutra_Image_Worker::run()
{
	qDebug() << "run Sutra_Image_Worker";

	forever
	{
		this->m_Mutex.lock();
		QImage Background_Image = this->m_Background_Image.copy();
		QPoint Background_Position = this->m_Background_Position;
		QFont font = this->m_Font;
		QFontMetrics fm(font);
		int Offset_X = this->m_Border_Position.x();
		int Offset_Y = this->m_Border_Position.y();
		int Width = this->m_Border.width();
		int Height = this->m_Border.height();
		int Word_Height = fm.height() + this->m_Font_Space;
		int Word_Width = fm.height() + this->m_Font_Distance;
		int Max_Word_Y = ( Height - 1)/ Word_Height - 1;
		int Overlap_Value = this->m_Overlap_Value;

		qDebug()  << Width << Height << Word_Height << Word_Width;
//		qDebug()  << "back image:" << Background_Position << Background_Image.rect();
		this->m_Mutex.unlock();

		if(!Background_Image.isNull())
		{
			int Word_X = 0;
			int Word_Y = 0;
			bool Overlap = true;

			foreach(QGraphicsItem* Item, this->m_Sutra->childItems())
			{
				if(this->m_Stop)	return;
				if(this->m_Restart)	break;

				QGraphicsSimpleTextItem* Text = qgraphicsitem_cast<QGraphicsSimpleTextItem *>(Item);
				if(Text)
				{
					do
					{
						if(this->m_Stop)	return;
						if(this->m_Restart)	break;

						int X = Width - Word_Width * (Word_X + 1) + Offset_X;
						int Y = Word_Height * Word_Y + Offset_Y;
						Overlap = false;

						Text->setData(Sutra_Data_Location_X, X);
						Text->setData(Sutra_Data_Location_Y, Y);

						Word_Y ++;
						if(Word_Y > Max_Word_Y )
						{
							Word_Y = 0;
							Word_X ++;
						}

						if(this->collision(Background_Position, Background_Image.size(),
								   QPoint(X, Y), QSize(Word_Width, Word_Height)))
						{
							int Start_X = qMax(X, Background_Position.x()) - Background_Position.x();
							int Start_Y = qMax(Y, Background_Position.y()) - Background_Position.y();
							int Copy_Width = (X < Background_Position.x())?
										X + Word_Width - Background_Position.x()
									: (X + Word_Width > Background_Position.x() + Background_Image.size().width()) ?
										Background_Position.x() + Background_Image.size().width() - X :
										Word_Width;
							int Copy_Height = (Y < Background_Position.y())?
										Y + Word_Height - Background_Position.y()
									: (Y + Word_Height > Background_Position.y() + Background_Image.size().height()) ?
										Background_Position.y() + Background_Image.size().height() - Y :
										Word_Height;
//							qDebug() << "Copy" << Start_X << Start_Y << Copy_Width << Copy_Height;

//							QImage Overlap_Image = Background_Image.copy(X - Background_Position.x(), Y - Background_Position.y(), Word_Width, Word_Height);
							QImage Overlap_Image = Background_Image.copy(Start_X, Start_Y, Copy_Width, Copy_Height);
							int Count = 0;

							for(int Overlap_y = 0; Overlap_y < Overlap_Image.size().height(); Overlap_y++)
							{
								if(this->m_Stop)	return;
								if(this->m_Restart)	break;

								for(int Overlap_x = 0; Overlap_x < Overlap_Image.size().width(); Overlap_x++)
								{
									if(this->m_Stop)	return;
									if(this->m_Restart)	break;

									if((Overlap_Image.pixel(Overlap_x, Overlap_y) & 0xFF)==0) Count++;
								}
							}
//							qDebug() << "Count" << Count << "Overlap_Value" << Overlap_Value << Overlap_Image.size();

							Overlap = (Count > Overlap_Value);
						}

						emit this->Sutra_word(Text);
						qDebug() << Overlap << Text->pos() << Word_X << Word_Y << Text->data(2) << Text->data(1) <<" ============ ";
					}
					while (Overlap);

					if(!this->m_Restart)
						emit this->Sutra_word(Text);
				}
			}
		}


		this->m_Mutex.lock();
		if(!this->m_Restart)
			this->m_Condition.wait(&this->m_Mutex);
		this->m_Restart = false;
		this->m_Mutex.unlock();
	}

	qDebug() << "done Sutra_Image_Worker";
}

void Sutra_Image_Worker::Stop()
{
	qDebug() << "Stop Sutra_Image_Worker";

	this->m_Mutex.lock();
	this->m_Stop = true;
	this->m_Mutex.unlock();

	qDebug() << "Stop Sutra_Image_Worker 2";

	this->m_Condition.wakeOne();
}

void Sutra_Image_Worker::Restart()
{
	qDebug() << "Restart Sutra_Image_Worker";

	this->m_Mutex.lock();
	this->m_Stop = false;
	this->m_Restart = true;
	this->m_Mutex.unlock();

	qDebug() << "Restart Sutra_Image_Worker 2";

	this->m_Condition.wakeOne();
}

QPoint Sutra_Image_Worker::getBorder_Position() const
{
	return m_Border_Position;
}

void Sutra_Image_Worker::setBorder_Position(const QPoint& Border_Position)
{
	m_Border_Position = Border_Position;
}

QGraphicsItemGroup* Sutra_Image_Worker::getSutra() const
{
	return m_Sutra;
}

void Sutra_Image_Worker::setSutra(QGraphicsItemGroup* Sutra)
{
	m_Sutra = Sutra;
}

int Sutra_Image_Worker::getFont_Distance() const
{
	return m_Font_Distance;
}

void Sutra_Image_Worker::setFont_Distance(int Font_Distance)
{
	m_Font_Distance = Font_Distance;
}

int Sutra_Image_Worker::getFont_Space() const
{
	return m_Font_Space;
}

void Sutra_Image_Worker::setFont_Space(int Font_Space)
{
	m_Font_Space = Font_Space;
}

QSize Sutra_Image_Worker::getBorder() const
{
	return m_Border;
}

void Sutra_Image_Worker::setBorder(const QSize& Border)
{
	m_Border = Border;
}

int Sutra_Image_Worker::getOverlap_Value() const
{
	return m_Overlap_Value;
}

void Sutra_Image_Worker::setOverlap_Value(int Overlap_Value)
{
	m_Overlap_Value = Overlap_Value;
}

bool Sutra_Image_Worker::collision(QPoint Source_Obj_XY, QSize Source_Obj_Size, QPoint Checked_Obj_XY, QSize Checked_Obj_Size)
{
	qDebug() << "collision check:" << Source_Obj_XY << Source_Obj_Size << Checked_Obj_XY << Checked_Obj_Size;
	int Source_Obj_X_min = Source_Obj_XY.x();
	int Source_Obj_X_max = Source_Obj_XY.x() + Source_Obj_Size.width();
	int Source_Obj_Y_min = Source_Obj_XY.y();
	int Source_Obj_Y_max = Source_Obj_XY.y() + Source_Obj_Size.height();

	int Checked_Obj_X_min = Checked_Obj_XY.x();
	int Checked_Obj_X_max = Checked_Obj_XY.x() + Checked_Obj_Size.width();
	int Checked_Obj_Y_min = Checked_Obj_XY.y();
	int Checked_Obj_Y_max = Checked_Obj_XY.y() + Checked_Obj_Size.height();

	if(Checked_Obj_X_max < Source_Obj_X_min)	return false;
	if(Checked_Obj_X_min > Source_Obj_X_max)	return false;
	if(Checked_Obj_Y_max < Source_Obj_Y_min)	return false;
	if(Checked_Obj_Y_min > Source_Obj_Y_max)	return false;

	return true;
}

QPoint Sutra_Image_Worker::getBackground_Position() const
{
	return m_Background_Position;
}
void Sutra_Image_Worker::setBackground_Position(const QPoint& Background_Position)
{
	m_Background_Position = Background_Position;
}

QImage Sutra_Image_Worker::getBackground_Image() const
{
	return m_Background_Image;
}
void Sutra_Image_Worker::setBackground_Image(const QImage& Background_Image)
{
	m_Background_Image = Background_Image;
}

QFont Sutra_Image_Worker::getFont() const
{
	return m_Font;
}

void Sutra_Image_Worker::setFont(const QFont& Font)
{
	m_Font = Font;
}
