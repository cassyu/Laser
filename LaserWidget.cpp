#include "LaserWidget.h"
#include "StatusWidget.h"

LaserWidget::LaserWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//ShrinkAnimation*  shwidget = new ShrinkAnimation();
	StatusWidget* stawidget = new StatusWidget();
	//stawidget->setWindowFlag(Qt::Popup);
	stawidget->setCusfixSizeHeight(0);
	//ui.widget->initControl(stawidget);

	//ShrinkAnimation*  shwidget1 = new ShrinkAnimation();
	/*StatusWidget* stawidget1 = new StatusWidget();
	stawidget1->setWindowFlag(Qt::Popup);
	stawidget1->setCusfixSizeHeight(0);
	ui.widget_2->initControl(stawidget1);*/

	//QVBoxLayout* vlayout = new QVBoxLayout();
	//vlayout->addWidget(shwidget);
	//vlayout->setSpacing(0);
	//vlayout->addWidget(shwidget1);
	//QWidget* widgetA = new QWidget();
	//widgetA->setLayout(vlayout);
}

LaserWidget::~LaserWidget()
{
}
