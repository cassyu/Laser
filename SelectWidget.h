#pragma once

#include <QWidget>
#include "ui_SelectWidget.h"

class QButtonGroup;

class SelectWidget : public QWidget
{
	Q_OBJECT

public:
	enum
	{
		BALCKTYPE,
		GARYTYPE,
		SVGTYPE,
		TEXTTYPE
	};

public:
	SelectWidget(QWidget *parent = Q_NULLPTR);
	~SelectWidget();
	void Connect();
	void paintEvent(QPaintEvent *event);
 signals:
	 void SelectModeClicked(int id );

public slots:
	void buttonGroupClicked(int id);

private:
	Ui::SelectWidget ui;
	QButtonGroup *  buttonGroup;
};
