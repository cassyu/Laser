#include "ExportWidget.h"
#include <QButtonGroup>
#include <QDebug>
#include <QPushButton>
#include <QStyleOption>
#include <QPainter>
//#include <QOverload>

ExportWidget::ExportWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Createconnect();
}

ExportWidget::~ExportWidget()
{

}

void ExportWidget::Createconnect()
{
	buttonGroup = new QButtonGroup(this);
	buttonGroup->setExclusive(false);
	auto button = new QPushButton();
	buttonGroup->addButton(ui.GenButton, BUTTON_GEN);
	//buttonGroup->addButton(ui.ExportButton, BUTTON_EXPORT);
	buttonGroup->addButton(button, BUTTON_TEXT);
	connect(buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonClicked(int)));

	ui.label->setVisible(false);
	ui.spinBox->setVisible(false);
	ui.GenButton->setStyleSheet("QPushButton{border-image:url(:/Resourse/pic/3.png);}");
}

void ExportWidget::paintEvent(QPaintEvent * event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ExportWidget::buttonClicked(int id)
{
	int array[] = {1,1,2,2,3,4};
	for (auto i : array)
	{
		i = 3;
		qDebug() << "num 1 is " << i;
	}
	for (auto&i : array)
	{
		qDebug() << "num2 is " << i;
		i = 5;
		qDebug() << "num3 is " << i;
	}
	for (auto&i : array)
	{
		qDebug() << "num 4 is " << i;
	}

	emit GcodeButtonClicked(id);
}
