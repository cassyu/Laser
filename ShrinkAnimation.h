#pragma once

#include <QWidget>
#include "ui_ShrinkAnimation.h"

#define	 FIXHEIGHT_wid   50
#define  LEVEL_ONE_HEIGHT   180

class QStackedWidget;

class ShrinkAnimation : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(int fixSizeHeight READ fixSizeHeight WRITE setCusfixSizeHeight)

public:
	ShrinkAnimation(QString name,QWidget *parent = nullptr);
	~ShrinkAnimation();

	void initControl(QWidget * pwidget);
	public:
	void setCusfixSizeHeight(int height);
	int  fixSizeHeight();

	void testStackWidget();
	void addStackWidget(QStackedWidget* stackWidget);

private:
	Ui::ShrinkAnimation ui;
	int                 m_Height;

};
