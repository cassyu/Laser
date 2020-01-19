#include "WorkingWid.h"
#include <QRegExp>
#include <QIntValidator>
#include <QStyleOption>
#include <QPainter>

WorkingWid::WorkingWid(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initLayout();
}

WorkingWid::~WorkingWid()
{

}

void WorkingWid::initLayout()
{
	//对输入进行限制  
	//限制字符的输入 
	//QRegExp  jx("^[1-11]\d{0,10}$");
    QRegExp  jx("10|[1-9]");
	QRegExpValidator *latitude = new QRegExpValidator(jx);

	//限制

	ui.lineEdit->setValidator(latitude);
	
	QRegExp  jx1("^2\d{2,3}|^1\d{3}|^3\d{2,3}|^4\d{2,3}|5000");
	QRegExpValidator* latitude1 = new QRegExpValidator(jx1);
	ui.lineEdit_2->setValidator(latitude1);
	ui.lineEdit_3->setValidator(new QIntValidator(200,5000));

	connect(ui.lineEdit, &QLineEdit::returnPressed,this, &WorkingWid::printOrderEditingFinished);
	connect(ui.lineEdit_2,&QLineEdit::returnPressed,this,&WorkingWid::inchingEditingFinished);
	connect(ui.lineEdit_3,&QLineEdit::returnPressed,this,&WorkingWid::workingEditingFinished);
	connect(ui.horizontalSlider, &QSlider::valueChanged, this, &WorkingWid::printOrderSliderChanged);

	connect(ui.spinBox,SIGNAL(valueChanged(int)),this,SLOT(printOrderFinished(int)));
	connect(ui.spinBox_2,SIGNAL(valueChanged(int)),this,SLOT(inchingEditFinished(int)));
	connect(ui.spinBox_3, SIGNAL(valueChanged(int)),this,SLOT(workingEditFinished(int)));

	//ui.lineEdit_2->setEnabled(false);
	//ui.lineEdit_3->setEnabled(false);
	ui.horizontalSlider->setVisible(false);
	ui.lineEdit->setVisible(false);
	ui.lineEdit_2->setVisible(false);
	ui.lineEdit_3->setVisible(false);
	ui.label_6->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.label->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.label_2->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.label_4->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.spinBox->setStyleSheet("QSpinBox{background-color:rgb(251,248,246);border:none;}\
    QSpinBox::up-button{border-image:url(:/Resourse/pic/down.png);}\
    QSpinBox::down-button{border-image:url(:/Resourse/pic/arrow.png);}");
	ui.spinBox_2->setStyleSheet("QSpinBox{background-color:rgb(251,248,246);border:none;}\
    QSpinBox::up-button{border-image:url(:/Resourse/pic/down.png);}\
    QSpinBox::down-button{border-image:url(:/Resourse/pic/arrow.png)}");
	ui.spinBox_3->setStyleSheet("QSpinBox{background-color:rgb(251,248,246);border:none;}\
    QSpinBox::up-button{border-image:url(:/Resourse/pic/down.png);}\
    QSpinBox::down-button{border-image:url(:/Resourse/pic/arrow.png);}");
	ui.label_3->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.label_5->setStyleSheet("QLabel{background-color:white;border:none;}");
}

void WorkingWid::setPrintingOrder(int n)
{
	ui.lineEdit->setText(QString::number(n));
	ui.horizontalSlider->setValue(n);
}

void WorkingWid::setInchingSpeed(int n)
{
	ui.lineEdit_2->setText(QString::number(n));
}

void WorkingWid::setWorkingSpeed(int n)
{
	ui.lineEdit_3->setText(QString::number(n));
}

void WorkingWid::paintEvent(QPaintEvent * event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void WorkingWid::printOrderSliderChanged(int value)
{
	ui.lineEdit->setText(QString::number(value));
	emit  printOrderChanged(value);
}

void WorkingWid::inchingEditingFinished()
{
	int value = ui.lineEdit_2->text().toInt();
	emit  inchingSpeedChanged(value);
}

void WorkingWid::workingEditingFinished()
{
	int value = ui.lineEdit_3->text().toInt();
	emit workingSpeedChanged(value);
}

void WorkingWid::printOrderFinished(int n)
{
	emit printOrderChanged(n);
}

void WorkingWid::inchingEditFinished(int n)
{
	emit inchingSpeedChanged(n);
}

void WorkingWid::workingEditFinished(int n)
{
	emit workingSpeedChanged(n);
}

void WorkingWid::printOrderEditingFinished()
{
	int value = ui.lineEdit->text().toInt();
	ui.horizontalSlider->setValue(value);
	emit printOrderChanged(value);
}

