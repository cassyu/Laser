#include "SSToolBox.h"
#include "SSToolBoxItem.h"

#include <QEvent>
#include <QScrollBar>

SSToolBox::SSToolBox(QWidget *parent)
	:QWidget(parent)
{
	initVar();
	this->installEventFilter(this);
}

void SSToolBox::addBar(SSToolBoxItem * boxItem)
{
	if (boxItem == nullptr) return;

	m_listBar << boxItem;
}

void SSToolBox::setBarHeight(int index, int height)
{
	int iCount = m_listBar.count();
	if (index < 0 || index >= iCount)
		return;

	m_listBar[index]->setBarHeight(height);
}

void SSToolBox::setContentLayout(int index, QLayout * layout)
{
	int iCount = m_listBar.count();
	if (index < 0 || index >= iCount)
		return;

	m_listBar[index]->setContentLayout(layout);
}

void SSToolBox::upDataG()
{
	bool b = m_pScrollArea->verticalScrollBar()->isVisible();
	int iScrollBarWidth = m_pScrollArea->verticalScrollBar()->width();
	if (m_pScrollAreaWidget)
	{
		m_pScrollAreaWidget->updateGeometry();
		m_pScrollAreaWidget->adjustSize();
	}
	if (m_pScrollArea)
	{
		m_pScrollArea->updateGeometry();
		m_pScrollArea->adjustSize();
	}
	//int tol_height = 0;
	//foreach(SSToolBoxItem *pItem, m_listBar)
	//{
	//	if (pItem->isVisible())
	//	{
	//		tol_height += pItem->height();
	//		int n = tol_height;
	//	}
	//}

	
}

void SSToolBox::initVar()
{
	m_pScrollArea = nullptr;
	m_pScrollAreaWidget = nullptr;
}

void SSToolBox::initLayout()
{
	QVBoxLayout* pMainLayout = new QVBoxLayout;
	pMainLayout->setSpacing(SPACE);
	pMainLayout->setContentsMargins(SPACEL, SPACET, SPACER, SPACEB);

	foreach(SSToolBoxItem *pItem, m_listBar)
	{
		pMainLayout->addWidget(pItem);
		//pMainLayout->addStretch();
	}
	pMainLayout->addStretch();
	m_pScrollAreaWidget->setLayout(pMainLayout);
	adjustScrollAreaWidgetSize();
}

void SSToolBox::createScrollArea()
{
	m_pScrollAreaWidget = new QWidget;
	m_pScrollAreaWidget->setContentsMargins(QMargins(0,0,0,0));
	m_pScrollAreaWidget->setStyleSheet("QWidget{background-color:white;}");
	m_pScrollArea = new QScrollArea(this);
	m_pScrollArea->setContentsMargins(0,0,0,0);
	m_pScrollArea->setWidget(m_pScrollAreaWidget);
	m_pScrollArea->setStyleSheet("QScrollArea{background-color:white;}\
    QScrollBar{width:16px;background-color:white;}\
    QScrollBar:vertical{width:10px;background:transparent;border:none;}\
    QScrollBar::handle:vertical{width:8px;background-color:gray;margin-top:0px;margin-bottom:0px;border-radius:4px;}\
    QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:transparent;}\
    QScrollBar::sub-line:vertical{background:transparent;}\
    QScrollBar::add-line:vertical{background:transparent;}");
	m_pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//不开启横向滚动
	m_pScrollArea->verticalScrollBar()->installEventFilter(this);//给竖向滚动安装过滤器，因为在滚动条出现或者消失的时候需要调节m_pScrollAreaWidget大小
    
}

bool SSToolBox::eventFilter(QObject * watched, QEvent * event)
{
	switch ((int)event->type())
	{
		case QEvent::Resize:
			if (watched->inherits("SSToolBox"))//进行类型检查 
			{
				bool b = m_pScrollArea->verticalScrollBar()->isVisible();
				int iScrollBarWidth = m_pScrollArea->verticalScrollBar()->width();
				QSize n;
				if (m_pScrollArea)
				{
					n = size();
					m_pScrollArea->setFixedSize(size());
					m_pScrollArea->updateGeometry();
					n = m_pScrollArea->size();
					int u = 10;
				}			    
				if (m_pScrollAreaWidget)
				{
					n = m_pScrollAreaWidget->size();
					m_pScrollAreaWidget->setFixedWidth(width() - (b ? iScrollBarWidth : 0));
					m_pScrollAreaWidget->updateGeometry();
					n = m_pScrollAreaWidget->size();
					m_SizeT = n;
					int u = 10;
				}
			}
			break;
		case QEvent::Show:
		case QEvent::Hide:
			if (watched->inherits("QScrollBar"))
			{
				//在点击Bar显示/隐藏Contents 根据当前是否有滚动条出现，如果有需要减去滚动条的宽度以免挡住Bar的图标
				int iScrollBarWidth = m_pScrollArea->verticalScrollBar()->width();
				int iWidth = width() - ((event->type() == QEvent::Show) ? iScrollBarWidth :0);
				if (m_pScrollAreaWidget)
					m_pScrollAreaWidget->setFixedWidth(iWidth);
			}
			break;
	    default:
		   break;
	}
	return QWidget::eventFilter(watched,event);
}

