#pragma once

#include <QWidget>
#include "ui_LaserWidget.h"

class LaserWidget : public QWidget
{
	Q_OBJECT

public:
	LaserWidget(QWidget *parent = nullptr);
	~LaserWidget();

private:
	Ui::LaserWidget ui;
};
