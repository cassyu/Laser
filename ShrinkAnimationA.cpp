#include "ShrinkAnimationA.h"

ShrinkAnimationA::ShrinkAnimationA(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

ShrinkAnimationA::~ShrinkAnimationA()
{
}

void ShrinkAnimationA::setCusfixSizeHeight(int height)
{
	this->setFixedHeight(height);
}

int ShrinkAnimationA::fixSizeHeight()
{
	return this->height();
}
