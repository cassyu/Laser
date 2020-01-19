#pragma once

#include <QWidget>
#include "ui_ShrinkAnimationA.h"

class ShrinkAnimationA : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(int fixSizeHeight READ fixSizeHeight WRITE setCusfixSizeHeight)

public:
	ShrinkAnimationA(QWidget *parent = nullptr);
	~ShrinkAnimationA();

public:
	void setCusfixSizeHeight(int height);
	int  fixSizeHeight();

private:
	Ui::ShrinkAnimationA ui;
};
