#pragma once

#include <QWidget>
#include "ui_GrayDiffuse.h"

class QButtonGroup;

class GrayDiffuse : public QWidget
{
	Q_OBJECT
public :
	enum  BUTTON
	{
		FloydSteinburg,
		Burkers,
		Sierra,
		Stucki,
		Jarvis,
		Stevenson,
	};

public:
	GrayDiffuse(QWidget *parent = nullptr);
	~GrayDiffuse();

	void Connect();
	void paintEvent(QPaintEvent *event);
	void SetGrayDiff(int id);

signals:
	void SelectModeClicked(int id);
	void GrayDiffuseChanged(int n);

public slots:
    void buttonGroupClicked(int id);
	void buttonClicked(bool checked);

private:
	Ui::GrayDiffuse ui;
	QButtonGroup *  buttonGroup;
};
