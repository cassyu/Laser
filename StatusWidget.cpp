#include "StatusWidget.h"

StatusWidget::StatusWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setStyleSheet("background-color:red;");
}

StatusWidget::~StatusWidget()
{
}

void StatusWidget::setCusfixSizeHeight(int h)
{
	this->setFixedHeight(h);
}

int StatusWidget::GetFixedHeight()
{
	return 50;
}

int StatusWidget::fixSizeHeight()
{
	return this->height();
}
