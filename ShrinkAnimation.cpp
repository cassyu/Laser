#include "ShrinkAnimation.h"
#include <QPushButton>
#include <QPropertyAnimation>
#include "FixHeight.h"
#include <QStackedWidget>
#include "SSTestToolBox.h"

ShrinkAnimation::ShrinkAnimation(QString name,QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.titleWidget->setFixedHeight(FIXHEIGHT_wid);
	ui.label->setText(name);
	m_Height = 0;
	//testStackWidget();
	//this->setStyleSheet("QWidget{border:1px solid rgb(154,240,52)}");
}

ShrinkAnimation::~ShrinkAnimation()
{
}

void ShrinkAnimation::initControl(QWidget * pwidget)
{
	/*
	if (nullptr != pwidget)
	{
		//FixHeight* sizehanlerect = dynamic_cast<FixHeight*>(pwidget);
		//if (sizehanlerect)
		{
			pwidget->setParent(this);
			pwidget->move(0, ui.titleWidget->height());
			int n = pwidget->height();
			//pwidget->setFixedHeight(LEVEL_ONE_HEIGHT);
			//pwidget->setVisible(false);
			//this->adjustSize();
			connect(ui.pushButton, &QPushButton::clicked, [this, pwidget]()
			{
				QPropertyAnimation *animation = new QPropertyAnimation(this, "fixSizeHeight");
				animation->setDuration(500);
				animation->setEasingCurve(QEasingCurve::InQuad);
				if (this->height() > FIXHEIGHT_wid)
				{
					animation->setEndValue(FIXHEIGHT_wid);
				}
				else
				{
					int height = pwidget->height();
					animation->setEndValue(height + FIXHEIGHT_wid + m_Height);
					//this->adjustSize();
					//this->setFixedHeight(LEVEL_ONE_HEIGHT + FIXHEIGHT_wid+300);
				}
				animation->start(QAbstractAnimation::DeleteWhenStopped);
			});
		}
	}
	*/
}

void ShrinkAnimation::setCusfixSizeHeight(int height)
{
	this->setFixedHeight(height);
	/*QApplication::UnicodeUTF8;*/
}

int ShrinkAnimation::fixSizeHeight()
{
	return this->height();
}

void ShrinkAnimation::testStackWidget()
{
	//m_StackWidget = new  QStackedWidget(this);
	//m_StackWidget->move(10, FIXHEIGHT_wid+ LEVEL_ONE_HEIGHT -10 );
	////m_StackWidget->setStyleSheet("border:1px solid blue");
	//m_StackWidget->setFixedSize(width() -10 ,200);


	//SSTestToolBox*  testbox = new SSTestToolBox();

	//m_StackWidget->addWidget(testbox);
	//m_StackWidget->setCurrentIndex(0);

}

void ShrinkAnimation::addStackWidget(QStackedWidget * stackWidget)
{
	if (nullptr != stackWidget)
	{
		stackWidget->setParent(this);
		stackWidget->move(10, FIXHEIGHT_wid + LEVEL_ONE_HEIGHT - 10);
		//m_StackWidget->setStyleSheet("border:1px solid blue");
		stackWidget->setFixedSize(width() - 10, 200);
		m_Height += 200;
	}


	/*SSTestToolBox*  testbox = new SSTestToolBox();

	m_StackWidget->addWidget(testbox);
	m_StackWidget->setCurrentIndex(0);*/
}
