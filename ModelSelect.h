#pragma once

#include <QWidget>
#include "ui_ModelSelect.h"

class ModelSelect : public QWidget
{
	Q_OBJECT

public:
	ModelSelect(QWidget *parent = nullptr);

	void InitLayout();
	void paintEvent(QPaintEvent *event);

	~ModelSelect();

private:
	Ui::ModelSelect ui;
};
