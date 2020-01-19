#include "SSToolBoxItem.h"
#include "SSToolBox.h"

#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>

SSToolBoxItem::SSToolBoxItem(QWidget * parent, QString name)
	:QWidget(parent)
{
	m_pName = nullptr;
	m_pIcon = nullptr;
	m_pBar  = nullptr;
	m_pContent = nullptr;
	m_pToolBox = nullptr;
	m_pToolBox = (SSToolBox *)parent;

	m_bExpand = false;
	m_sName = name;

	createCtrl();
	initLayout();
}

void SSToolBoxItem::setContentExpend(bool expand)
{
	m_bExpand = expand;
}

void SSToolBoxItem::setFlag(int flag)
{
	m_iCurrentFlag = flag;
}

void SSToolBoxItem::paintEvent(QPaintEvent * event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

int SSToolBoxItem::getFlag() const
{
	return m_iCurrentFlag;
}

void SSToolBoxItem::createCtrl()
{
	//去掉标题栏的部分
	/*m_pBar = new QWidget();
	m_pBar->setFixedHeight(BAR_HEIGHT);
	m_pBar->setStyleSheet("background: rgb(170, 170, 170)");

	m_pName = new QLabel(m_sName, m_pBar);
	m_pName->adjustSize();

	m_pIcon = new QLabel(m_pBar);
	setIcon();
	m_pIcon->adjustSize();*/

	m_pContent = new QWidget(this);
	m_pContent->setStyleSheet("background: rgb(218, 222, 225)");
	m_pContent->hide();
}

void SSToolBoxItem::initLayout()
{
	//去掉标题栏的部分
	/*QHBoxLayout* pBarLayout = new QHBoxLayout;
	pBarLayout->addWidget(m_pName);
	pBarLayout->addStretch();
	pBarLayout->addWidget(m_pIcon);
	pBarLayout->setSpacing(0);
	pBarLayout->setContentsMargins(10, 0, 4, 0);
	m_pBar->setLayout(pBarLayout);*/

	//整体布局
	QVBoxLayout *pMainLayout = new QVBoxLayout(this);//方法1 
	//pMainLayout->addWidget(m_pBar);
	pMainLayout->addWidget(m_pContent);
	pMainLayout->addStretch();
	pMainLayout->setSpacing(0);
	pMainLayout->setContentsMargins(0, 0, 0, 0);
	// this->setLayout(pMainLayout);   方法2 

}

void SSToolBoxItem::setIcon()
{
	if (nullptr != m_pIcon)
	{
		QPixmap pixmap(m_bExpand ? ":/Others/Icon/Others/top.png" : ":/Others/Icon/Others/bottom.png");
		m_pIcon->setPixmap(pixmap);
	}
}

void SSToolBoxItem::stateSwitch()
{
	m_bExpand = !m_bExpand;

	setIcon();

	m_pContent->setVisible(m_bExpand);

	//新加的  看来是不需要 
	/*QList<SSToolBoxItem*>childlist = this->findChildren<SSToolBoxItem*>();
	if (m_bExpand)
	{
		for (SSToolBoxItem* item : childlist)
		{
			item->setContentExpend(m_bExpand);
			QWidget* childwidget = item->findChild<QWidget*>("Content");
			if (childwidget)
			{
				childwidget->setVisible(true);
			}
		}
	}*/
	adjustSize();
	m_pToolBox->adjustScrollAreaWidgetSize();
}

void SSToolBoxItem::mousePressEvent(QMouseEvent * event)
{
	/*QPoint point = event->pos();
	if (m_pBar->geometry().contains(point))
	{
		stateSwitch();
	}*/
	QWidget::mousePressEvent(event);
}

