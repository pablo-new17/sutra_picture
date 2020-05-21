#ifndef BACKGROUND_IMAGE_WORKER_H
#define BACKGROUND_IMAGE_WORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QPixmap>
#include <QImage>
#include <QFont>

class Background_Image_Worker : public QObject
{
	Q_OBJECT
public:
	explicit Background_Image_Worker(QObject *parent = nullptr);
	~Background_Image_Worker();

	bool getIsText() const;
	void setIsText(bool isText);

	QString getText() const;
	void setText(const QString& Text);

	QString getFilename() const;
	void setFilename(const QString& Filename);

	int getImage_Scale_Factor() const;
	void setImage_Scale_Factor(int Image_Scale_Factor);

	QSize getBoder_Size() const;
	void setBoder_Size(const QSize& Boder_Size);

	unsigned int getThreadhold() const;
	void setThreadhold(unsigned int Threadhold);

	QFont getFont() const;
	void setFont(const QFont& Font);

	int getFont_Space() const;
	void setFont_Space(int Font_Space);

public slots:
	void run();
	void Stop();
	void Restart();

signals:
	void Send_Signal(int Count);
	void Background_Image(QImage image);

private:
	bool		m_isText;
	QString		m_Text;
	QString		m_Filename;
	QSize		m_Boder_Size;
	unsigned int	m_Threadhold;
	QPixmap		m_Background_Image;
	int		m_Image_Scale_Factor;
	QFont		m_Font;
	int		m_Font_Space;



	QImage		m_image;

private:	//common for Worker
	bool		m_Stop;
	bool		m_Restart;

	QMutex		m_Mutex;
	QWaitCondition	m_Condition;

private slots:
	void Load_Image(QString Filename);

	QImage Convert_To_Gray(QImage image);
};


#endif // BACKGROUND_IMAGE_WORKER_H
