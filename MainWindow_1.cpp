#include <QDebug>
#include <QFontMetrics>

#include "PLable.h"
#include "MainWindow_1.h"
#include "ui_MainWindow_1.h"


//QPointSize實際是磅，也就是 1/72 inch
//QPixelSize = DPI * Point / 72

MainWindow_1::MainWindow_1(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

//	ui->Font->setText(ui->lineEdit->text());
//	ui->Font->setFont(ui->fontComboBox->currentFont());
//	ui->spinBox->setValue(ui->Font->font().pointSize());

//	Font_Information();
}

MainWindow_1::~MainWindow_1()
{
	delete ui;
}


void MainWindow_1::on_lineEdit_textEdited(const QString &arg1)
{
//	Font_Information();
}

void MainWindow_1::on_fontComboBox_currentFontChanged(const QFont &f)
{
//	Font_Information();
}

void MainWindow_1::Font_Information()
{
//	QString msg = ui->lineEdit->text();
//	QFont f = ui->fontComboBox->currentFont();
//	f.setPointSize(ui->spinBox->value());
//	QFontMetrics fm(f);
//	QRect r = fm.boundingRect(msg);
//	QRect t = fm.tightBoundingRect(msg);
//	QSize s = fm.size(Qt::TextSingleLine, msg);


//	ui->Font->setText(msg);
//	ui->Font->setFont(f);

//	//font height
//	ui->height->setText(QString::number(fm.height())); // = ascent() + descent()

//	//font width
//	ui->leftBearing->setText(QString::number(fm.leftBearing(msg.at(0))));
//	ui->rightBearing->setText(QString::number(fm.rightBearing(msg.at(0))));
//	ui->horizontalAdvance->setText(QString::number(fm.horizontalAdvance(msg.at(0))));

//	//font box
//	ui->boundingRect->setText(QString("%1, %2, %3x%4").arg(r.left()).arg(r.top()).arg(r.width()).arg(r.height()));
//	ui->tightBoundingRect->setText(QString("%1, %2, %3x%4").arg(t.left()).arg(t.top()).arg(t.width()).arg(t.height()));
//	ui->size->setText(QString("%1, %2").arg(s.width()).arg(s.height()));

//	//font space
//	ui->leftBearing->setText(QString::number(fm.leftBearing(msg.at(0))));
//	ui->rightBearing->setText(QString::number(fm.rightBearing(msg.at(0))));

//	//font line
//	ui->lineSpacing->setText(QString::number(fm.lineSpacing()));
//	ui->lineWidth->setText(QString::number(fm.lineWidth()));

//	ui->fontDpi->setText(QString::number(fm.fontDpi()));

}

void MainWindow_1::on_spinBox_valueChanged(int arg1)
{
//	Font_Information();
}

void MainWindow_1::on_pushButton_clicked()
{
//	ui->crop->setPixmap(*ui->Font->getImage());
}

void MainWindow_1::on_pushButton_2_clicked()
{
//	ui->Font->set_Box(ui->X->text().toInt(), ui->Y->text().toInt(), ui->W->text().toInt(), ui->H->text().toInt());
//	ui->crop->setPixmap(*ui->Font->getImage());
}
