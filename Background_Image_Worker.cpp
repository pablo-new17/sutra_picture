#include <QDebug>
#include <QThread>
#include <QPainter>

#include "Background_Image_Worker.h"

Background_Image_Worker::Background_Image_Worker(QObject *parent) : QObject(parent)
{
	qDebug() << "Start Background_Image_Worker";

	this->m_Stop = false;
	this->m_Restart = false;

	this->m_isText = true;
	this->m_Image_Scale_Factor = 100;
	this->m_Threadhold = 127;
	this->m_Font = QFont("新細明體", 900);
}

Background_Image_Worker::~Background_Image_Worker()
{
	this->m_Mutex.lock();
	this->m_Stop = true;
	this->m_Mutex.unlock();

	this->m_Condition.wakeAll();

	qDebug() << "end of Background_Image_Worker";
}

void Background_Image_Worker::run()
{
	qDebug() << "run Background_Image_Worker";

	forever
	{
		this->m_Mutex.lock();
			bool isText = this->m_isText;
			QString Text = this->m_Text;
			QPixmap Background_Image = this->m_Background_Image;
			int Scale_Factor = this->m_Image_Scale_Factor;
			QSize Boder_Size = this->m_Boder_Size;
			quint32 Threadhold = this->m_Threadhold;
			QFont Font = this->m_Font;
			int Font_Space = this->m_Font_Space;
		this->m_Mutex.unlock();

		QImage Grayscale_Image;

		if(isText && !Text.isEmpty())
		{
			qDebug() << "run Background_Image_Worker for text";

			QFontMetrics fm(Font);
			int Y = fm.ascent();
			int Font_Total_Height = fm.height() + (fm.height() + Font_Space ) * ( Text.size() - 1);

			QPixmap Scaled_Image = QPixmap(fm.horizontalAdvance(Text.at(0)), Font_Total_Height);
			Scaled_Image.fill(Qt::transparent);
			QPainter painter(&Scaled_Image);
			painter.setFont(Font);
			painter.setPen(Qt::black);

			for(int Index = 0; Index < Text.size(); Index++)
			{
				painter.drawText(0, Y, Text.at(Index));
				Y += (fm.height() + Font_Space);
			}
			Grayscale_Image = Scaled_Image.toImage();
		}
		else if(!Background_Image.isNull())
		{
			qDebug() << "run Background_Image_Worker for image";

			int Width = Boder_Size.width() / 100 * Scale_Factor;
			int Height = Boder_Size.height() / 100 * Scale_Factor;

			qDebug() << Width << Height;

			QPixmap Scaled_Image = this->m_Background_Image.scaled(Width, Height,
								       Qt::KeepAspectRatio,
								       Qt::SmoothTransformation);
			Grayscale_Image = this->Convert_To_Gray(Scaled_Image.toImage());
		}

		if(!Grayscale_Image.isNull())
		{
			for(int y = 0; y < Grayscale_Image.size().height(); y++)
			{
				if(this->m_Restart)	break;

				for(int x = 0; x < Grayscale_Image.size().width(); x++)
				{
					if(this->m_Stop)	return;
					if(this->m_Restart)	break;

					bool SetPixel_Black = false;
					if(isText)
						SetPixel_Black = (Grayscale_Image.pixel(x, y) != 0);
					else
						SetPixel_Black = ((Grayscale_Image.pixel(x, y) & 0xFF) < Threadhold);

					if(SetPixel_Black)
						Grayscale_Image.setPixel(x, y, 0xFF000000);
					else
						Grayscale_Image.setPixel(x, y, 0xFFFFFFFF);
				}
			}

			if(!this->m_Restart)
				emit this->Background_Image(Grayscale_Image);
		}




		this->m_Mutex.lock();
		if(!this->m_Restart)
			this->m_Condition.wait(&this->m_Mutex);
		this->m_Restart = false;
		this->m_Mutex.unlock();
		if(this->m_Stop)	return;

	}

	qDebug() << "done Background_Image_Worker";
}

void Background_Image_Worker::Stop()
{
	qDebug() << "Stop Background_Image_Worker";

	this->m_Mutex.lock();
	this->m_Stop = true;
	this->m_Mutex.unlock();

	qDebug() << "Stop Background_Image_Worker 2";

	this->m_Condition.wakeOne();
}

void Background_Image_Worker::Restart()
{
	qDebug() << "Restart Background_Image_Worker";

	this->m_Mutex.lock();
	this->m_Stop = false;
	this->m_Restart = true;
	this->m_Mutex.unlock();

	qDebug() << "Restart Background_Image_Worker 2";

	this->m_Condition.wakeOne();
}


void Background_Image_Worker::Load_Image(QString Filename)
{
	this->m_Background_Image = QPixmap(Filename);
}

QImage Background_Image_Worker::Convert_To_Gray(QImage image)
{
	QImage Grayscale_Image(image.size(), QImage::Format_Grayscale8);
	QPainter painter(&Grayscale_Image);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(Grayscale_Image.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawImage(0, 0, image);
	painter.end();

	return Grayscale_Image;
}


int Background_Image_Worker::getFont_Space() const
{
	return m_Font_Space;
}
void Background_Image_Worker::setFont_Space(int Font_Space)
{
	m_Font_Space = Font_Space;
}

QFont Background_Image_Worker::getFont() const
{
	return m_Font;
}
void Background_Image_Worker::setFont(const QFont& Font)
{
	m_Font = Font;
}

QSize Background_Image_Worker::getBoder_Size() const
{
	return m_Boder_Size;
}
void Background_Image_Worker::setBoder_Size(const QSize& Boder_Size)
{
	m_Boder_Size = Boder_Size;
}

unsigned int Background_Image_Worker::getThreadhold() const
{
	return m_Threadhold;
}
void Background_Image_Worker::setThreadhold(unsigned int Threadhold)
{
	m_Threadhold = Threadhold;
}

int Background_Image_Worker::getImage_Scale_Factor() const
{
	return m_Image_Scale_Factor;
}
void Background_Image_Worker::setImage_Scale_Factor(int Image_Scale_Factor)
{
	m_Image_Scale_Factor = Image_Scale_Factor;
}

QString Background_Image_Worker::getFilename() const
{
	return m_Filename;
}
void Background_Image_Worker::setFilename(const QString& Filename)
{
	if(m_Filename != Filename)
	{
		m_Filename = Filename;
		this->Load_Image(m_Filename);
	}

}

QString Background_Image_Worker::getText() const
{
	return m_Text;
}
void Background_Image_Worker::setText(const QString& Text)
{
	m_Text = Text;
}

bool Background_Image_Worker::getIsText() const
{
	return m_isText;
}
void Background_Image_Worker::setIsText(bool isText)
{
	m_isText = isText;
}



