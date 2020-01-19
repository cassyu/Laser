#pragma once

#include <QWidget>
#include "ui_StatusWidget.h"
#include "FixHeight.h"

class StatusWidget : public QWidget,public FixHeight
{
	Q_OBJECT
	Q_PROPERTY(int fixSizeHeight READ fixSizeHeight WRITE setCusfixSizeHeight)

public:
	StatusWidget(QWidget *parent = nullptr);
	~StatusWidget();
	virtual void  setCusfixSizeHeight(int h);
	virtual int   GetFixedHeight();
	virtual int   fixSizeHeight();

private:
	Ui::StatusWidget ui;
};
