#include "TitleBar.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QApplication>
#include <QStyleOption>
#include <QPainter>
#include "mainwindow.h"

#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif

TitleBar::TitleBar(QWidget *parent)
	: QWidget(parent)
{
	this->setObjectName("TitleWidget");

	m_pIconLabel = new QPushButton(this);
	m_pIconLabel->setFixedSize(120,20);
	//m_pIconLabel->setStyleSheet("QWidget{background-color:red;}");
	//m_pIconLabel->setPixmap(QPixmap(":/Resourse/pic/Title_p.png"));
	m_pIconLabel->setStyleSheet("QPushButton{border-image:url(:/Resourse/pic/18.png);}");
	m_pTitleLabel = new QLabel(this);
	//m_pTitleLabel->setStyleSheet("QWidget{background-color:blue;}");
	m_pTitleLabel->setFixedSize(109, 16);
	QFont font;
	font.setPixelSize(15);
	font.setFamily("黑体");
	m_pTitleLabel->setFont(font);
	//m_pTitleLabel->setText("PICASSO3D");

	m_pMaximizeButton = new QPushButton(this);
	m_pMaximizeButton->setFixedSize(20,20);
	//m_pMaximizeButton->setStyleSheet("QPushButton{width:20px;height:20px;background:transparent;background-position:center;background-image:url(:/Resourse/pic/Max.png);}");
	m_pMaximizeButton->setStyleSheet("QPushButton{border-image:url(:/Resourse/pic/Max.png);}");
	m_pMinimizeButton = new QPushButton(this);
	m_pMinimizeButton->setFixedSize(25,6);
	//m_pMinimizeButton->setStyleSheet("QPushButton{background:transparent;background-repeat:none;background-position:center;background-image:url(:/Resourse/pic/Min.png);}");
	m_pMinimizeButton->setStyleSheet("QPushButton{border-image:url(:/Resourse/pic/Min.png)}");
	//m_pMinimizeButton->setScaledContents(true);
	m_pCloseButton = new QPushButton(this);
	m_pCloseButton->setFixedSize(20,20);
	m_pCloseButton->setStyleSheet("QPushButton{background:transparent;background-repeat:none;background-position:center;background-image:url(:/Resourse/pic/Close.png);}");
	//m_pCloseButton->setStyleSheet("QPushButton{border-image:url(:/Resourse/pic/Close.png) 4 4 4 4 stretch stretch;");
	//图形的尺寸  139  28 
	//m_pIconLabel->setFixedSize(20, 20);
	//m_pIconLabel->setScaledContents(true);

	//m_pTitleLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	//m_pMinimizeButton->setFixedSize(27,22);
	//m_pMaximizeButton->setFixedSize(27,22);

	//m_pCloseButton->setFixedSize(27, 22);

	m_pTitleLabel->setObjectName("whiteLabel");
	m_pMinimizeButton->setObjectName("minimizeButton");
	m_pMaximizeButton->setObjectName("maximizeButton");
	m_pCloseButton->setObjectName("closeButton");

	m_pMinimizeButton->setToolTip("Minimize");
	m_pMaximizeButton->setToolTip("Maximize");
	m_pCloseButton->setToolTip("Close");

	QHBoxLayout* pLayout = new QHBoxLayout();
	pLayout->addSpacing(10);
	pLayout->addWidget(m_pIconLabel);
	pLayout->addSpacing(5);
	pLayout->addWidget(m_pTitleLabel);
	pLayout->addStretch();
	pLayout->addWidget(m_pMinimizeButton);
	pLayout->addSpacing(10);
	pLayout->addWidget(m_pMaximizeButton);
	pLayout->addSpacing(10);
	pLayout->addWidget(m_pCloseButton);

	pLayout->setSpacing(0);
	//pLayout->setContentsMargins(5, 0, 5, 0);

	setLayout(pLayout);

	connect(m_pMinimizeButton,SIGNAL(clicked(bool)),this,SLOT(onClicked()));
	connect(m_pMaximizeButton,SIGNAL(clicked(bool)),this,SLOT(onClicked()));
	connect(m_pCloseButton,SIGNAL(clicked(bool)),this,SLOT(onClicked()));

	setAttribute(Qt::WA_StyledBackground);

	//setFixedHeight(200);
	//setFixedWidth(900);
    
}

TitleBar::~TitleBar()
{
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent * event)
{
	Q_UNUSED(event);

	emit m_pMaximizeButton->clicked(false);
}

void TitleBar::onClicked()
{
	QPushButton* pButton = qobject_cast<QPushButton*>(sender());
	QWidget* pWindow = this->window();
	if (pWindow->isTopLevel())
	{
		if (pButton == m_pMinimizeButton)
		{
			pWindow->showMinimized();
		}
		else if (pButton == m_pMaximizeButton)
		{
			pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
		}
		else if (pButton == m_pCloseButton)
		{
			pWindow->close();
 			QApplication* app;
 			app->exit(0);
		}
	}
}

void TitleBar::mousePressEvent(QMouseEvent * event)
{
////#ifdef Q_OS_WIN
//		if (ReleaseCapture())
//		{
//			QWidget* pWindow = this->window();
//			if (pWindow->isTopLevel())
//			{
//				SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
//			}
//		}
//
//	event->ignore();
////#else
//#endif 

	if (event->button() == Qt::LeftButton)
	{
		m_mousePress = true;
		//event->accept();
	}
	//dragPosition = event->globalPos() - frameGeometry().topLeft();//鼠标位置减去左上角的左边
	dragPosition = event->globalPos() - MainWindow::GetMainWindow()->frameGeometry().topLeft();//鼠标位置减去左上角的左边
}

void TitleBar::mouseMoveEvent(QMouseEvent * event)
{
	//if (event->buttons() & Qt::LeftButton)
	//{
	//	move(event->globalPos() - dragPosition);//鼠标位置减去按下点的坐标
	//	event->accept();
	//}
	if (m_mousePress)
	{
		QPoint movePos = event->globalPos();
		//parentWidget()->move(movePos - dragPosition);
		MainWindow::GetMainWindow()->move(movePos - dragPosition);
	}
}

void TitleBar::mouseReleaseEvent(QMouseEvent * event)
{
	m_mousePress = false;
}

void TitleBar::paintEvent(QPaintEvent * event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	//QWidget::paintEvent(event);
}

bool TitleBar::eventFilter(QObject * obj, QEvent * event)
{
	switch (event->type())
	{
	case QEvent::WindowTitleChange:
	{
		QWidget* pWidget = qobject_cast<QWidget*>(obj);
		if (pWidget)
		{
			m_pTitleLabel->setText(pWidget->windowTitle());
			return true;
		}
	}
	case QEvent::WindowIconChange:
	{
		QWidget* pWidget = qobject_cast<QWidget*>(obj);
		if (pWidget)
		{
			QIcon icon = pWidget->windowIcon();
			//m_pIconLabel->setPixmap(icon.pixmap(m_pIconLabel->size()));
			return true;
		}
	}
	case QEvent::WindowStateChange:
	case QEvent::Resize:
	{
		updateMaximize();
		return true;
	}
	default:
		break;
	}

	return QWidget::eventFilter(obj, event);
}

void TitleBar::updateMaximize()
{
	QWidget* pWindow = this->window();
	if (pWindow->isTopLevel())
	{
		bool bMaximize = pWindow->isMaximized();
		if (bMaximize)
		{
			m_pMaximizeButton->setToolTip(tr("Restore"));
			m_pMaximizeButton->setProperty("maximizeProperty","restore");
		}
		else
		{
			m_pMaximizeButton->setProperty("maximizeProperty","maximize");
			m_pMaximizeButton->setToolTip("Maximize");
		}

		m_pMaximizeButton->setStyle(QApplication::style());
	}
}
