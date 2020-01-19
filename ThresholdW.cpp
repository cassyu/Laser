#include "ThresholdW.h"
#include <QStyleOption>
#include <QPainter>

ThresholdW::ThresholdW(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	createInit();
}

ThresholdW::~ThresholdW()
{
}

void ThresholdW::createInit()
{
	m_type = 0;
	ui.lineEdit->setVisible(false);
	connect(ui.spinBox, SIGNAL(valueChanged(int)),this, SLOT(thresholdFinished(int)));
	ui.label_6->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.label->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.spinBox->setStyleSheet("QSpinBox{background-color:rgb(251,248,246);border:none;}\
    QSpinBox::up-button{border-image:url(:/Resourse/pic/down.png);}\
    QSpinBox::down-button{border-image:url(:/Resourse/pic/arrow.png);}");
	//connect(ui.lineEdit, &QLineEdit::returnPressed, this, &ThresholdW::thresholdChangedFinished);
}

void ThresholdW::paintEvent(QPaintEvent * event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

int ThresholdW::getThreshold() const
{
	//return ui.lineEdit->text().toInt();
	return ui.spinBox->value();
}

void ThresholdW::setThreshold(int h, uint w)
{
	//ui.lineEdit->setText(QString::number(h));
	m_type = w;
	ui.spinBox->setValue(h);
}

void ThresholdW::thresholdFinished(int n)
{
	emit  ThresholdChanged(n,m_type);
}

void ThresholdW::thresholdChangedFinished()
{
	int n = ui.lineEdit->text().toInt();
	emit ThresholdChanged(n,m_type);
}
