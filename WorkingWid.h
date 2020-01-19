#pragma once

#include <QWidget>
#include "ui_WorkingWid.h"

class WorkingWid : public QWidget
{
	Q_OBJECT

public:
	WorkingWid(QWidget *parent = nullptr);
	~WorkingWid();
	void initLayout();
	void setPrintingOrder(int n);
	void setInchingSpeed(int n);
	void setWorkingSpeed(int n);

	void paintEvent(QPaintEvent *event);

signals:
	void  printOrderChanged(int n);
	void  inchingSpeedChanged(int n);
	void  workingSpeedChanged(int n);

public slots:
    void  printOrderEditingFinished();
    void  printOrderSliderChanged(int value);
	void  inchingEditingFinished();
	void  workingEditingFinished();

	void  printOrderFinished(int n);
	void  inchingEditFinished(int n);
	void  workingEditFinished(int n);

private:
	Ui::WorkingWid ui;
};
