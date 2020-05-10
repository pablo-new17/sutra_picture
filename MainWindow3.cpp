#include <QDebug>
#include <QImage>
#include <QPainter>

#include "MainWindow3.h"
#include "ui_MainWindow3.h"

const QSize resultSize(8, 8);

MainWindow3::MainWindow3(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow3)
{
	ui->setupUi(this);


//	QPainter::CompositionMode mode = currentMode();

//	   QPainter painter(&resultImage);
//	   painter.setCompositionMode(QPainter::CompositionMode_Source);
//	   painter.fillRect(resultImage.rect(), Qt::transparent);
//	   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
//	   painter.drawImage(0, 0, destinationImage);
//	   painter.setCompositionMode(mode);
//	   painter.drawImage(0, 0, sourceImage);
//	   painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
//	   painter.fillRect(resultImage.rect(), Qt::white);
//	   painter.end();

//	   resultLabel->setPixmap(QPixmap::fromImage(resultImage));
}

MainWindow3::~MainWindow3()
{
	delete ui;
}

void MainWindow3::on_pushButton_clicked()
{
	QImage image = ui->label_source->pixmap()->toImage();
	image = image.scaled(resultSize, Qt::KeepAspectRatio);
	QPoint s_p((resultSize.width() - image.width()) / 2,
			  (resultSize.height() - image.height()) / 2);

	QImage fixedImage(resultSize, QImage::Format_Grayscale8);
	QPainter painter(&fixedImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(fixedImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawImage(s_p, image);
	painter.end();

//	ui->label_source->setPixmap(QPixmap::fromImage(image));

	fixedImage.setPixel(2, 0, 0xFF000000);
	fixedImage.setPixel(3, 0, 0xFF000000);

	ui->label_source->setPixmap(QPixmap::fromImage(fixedImage));

	qDebug() << "depth" << fixedImage.depth();
	qDebug() << "bitPlaneCount" << fixedImage.bitPlaneCount();
	qDebug() << "bytesPerLine" << fixedImage.bytesPerLine();
	qDebug() << "colorCount" << fixedImage.colorCount();
	qDebug() << "height" << fixedImage.height();
	qDebug() << "pixel(2,0)" << (fixedImage.pixel(2,0) & 0xFF);
	qDebug() << "pixel(3,0)" << (fixedImage.pixel(3,0) & 0xFF);
//	qDebug() << "pixelIndex(0,0)" << fixedImage.pixelIndex(0,0);
//	qDebug() << "pixelIndex(3,0)" << fixedImage.pixelIndex(3,0);
	qDebug() << "sizeInBytes" << fixedImage.sizeInBytes();
	qDebug() << "width" << fixedImage.width();


}

void MainWindow3::on_pushButton_2_clicked()
{
	QImage image = ui->label_Destination->pixmap()->toImage();
	image = image.scaled(resultSize, Qt::KeepAspectRatio);
	QPoint s_p((resultSize.width() - image.width()) / 2,
			  (resultSize.height() - image.height()) / 2);

	QImage fixedImage(resultSize, QImage::Format_Grayscale8);
	QPainter painter(&fixedImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(fixedImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawImage(s_p, image);
	painter.end();

	ui->label_Destination->setPixmap(QPixmap::fromImage(fixedImage));
}

void MainWindow3::on_pushButton_3_clicked()
{
	QImage resultImage(resultSize, QImage::Format_Grayscale8);


	QImage sourceImage = ui->label_source->pixmap()->toImage();
	//sourceImage = sourceImage.scaled(resultSize, Qt::KeepAspectRatio);

	QImage destinationImage = ui->label_Destination->pixmap()->toImage();
	//destinationImage = destinationImage.scaled(resultSize, Qt::KeepAspectRatio);

	QPainter painter(&resultImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawImage(0, 0, sourceImage);

	painter.setCompositionMode(QPainter::CompositionMode_Xor);
	painter.drawImage(0, 0, destinationImage);

	painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
	painter.fillRect(resultImage.rect(), Qt::white);

	painter.end();

	ui->label_Target->setPixmap(QPixmap::fromImage(resultImage));
}
