#pragma once

#include <QWidget>
#include "ui_ThresholdW.h"

class ThresholdW : public QWidget
{
	Q_OBJECT

public:
	ThresholdW(QWidget *parent = nullptr);
	~ThresholdW();

	void createInit();
	void paintEvent(QPaintEvent *event);

	int getThreshold()const;
	void setThreshold(int h,uint w = 0);

signals:
	void ThresholdChanged(int n,uint type = 0);

public slots:
	void thresholdChangedFinished();
	void thresholdFinished(int n);

private:
	Ui::ThresholdW ui;
	uint           m_type;// 0 黑白阈值    1  矢量阈值
};
