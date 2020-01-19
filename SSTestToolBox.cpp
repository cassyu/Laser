#include "SSTestToolBox.h"
#include "SSToolBoxItem.h"
#include "Transformation.h"
#include "TextWidget.h"
#include <QToolButton>

SSTestToolBox::SSTestToolBox(QWidget *parent) : SSToolBox(parent)
{
	createScrollArea();
	//创建控件
	//createResolutionBar();
	createExposureBar();
	//createWhiteBalanceBar();
	//createImageAdjustmentBar();
	//createHistogramBar();

	//初始化
	initLayout();
}

void SSTestToolBox::buttonClicked()
{
	QToolButton *pButton = qobject_cast<QToolButton *>(sender());
	if (pButton == NULL) return;

	pButton->setText(QObject::tr("hello world"));
}

void SSTestToolBox::TextAndFontChanged(QString & text, QString & font)
{
	emit TextAndFontTrans(text,font);
}

void SSTestToolBox::createResolutionBar()
{
	//分辨率    
	m_pResolutionBar = new SSToolBoxItem(this, QObject::tr("Transformation"));
	m_listBar << m_pResolutionBar;

	//QHBoxLayout *pLayout = new QHBoxLayout;
	QVBoxLayout* pLayout = new QVBoxLayout();
	/*pLayout->addStretch();
	for (int i = 0; i < 4; i++)
	{
		QToolButton *pButton = new QToolButton;
		connect(pButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));
		pLayout->addWidget(pButton);
	}
	pLayout->addStretch();
	pLayout->setSpacing(4);*/
	m_pTransformation = new Transformation();
	m_pTransformation->setFixedSize(QSize(330,290));
	pLayout->addWidget(m_pTransformation);
	

	m_pResolutionBar->setContentLayout(pLayout);
}

void SSTestToolBox::createExposureBar()
{
	//曝光控制
	m_pExposureBar = new SSToolBoxItem(this, QObject::tr(u8"文本"));
	m_listBar << m_pExposureBar;

	/*QHBoxLayout *pLayout = new QHBoxLayout;
	pLayout->addStretch();
	for (int i = 0; i < 4; i++)
	{
		QToolButton *pButton = new QToolButton;
		pLayout->addWidget(pButton);
	}
	pLayout->addStretch();
	pLayout->setSpacing(4);*/
	QVBoxLayout* pLayout = new QVBoxLayout();
	m_pTextWidget = new TextWidget();
	//m_pTextWidget->setBaseSize(QSize(330,290));
	m_pTextWidget->setFixedSize(QSize(288, 274));
	// QString & text, QString & font
	//connect(m_pTextWidget, &TextWidget::CurrentTextAndFont,this,&SSTestToolBox::TextAndFontChanged);
	connect(m_pTextWidget, SIGNAL(CurrentTextAndFont(QString &, QString &)),this,SLOT(TextAndFontChanged(QString &,QString &)));
	pLayout->addWidget(m_pTextWidget);

	m_pExposureBar->setContentLayout(pLayout);
}

void SSTestToolBox::createWhiteBalanceBar()
{
	//曝光控制
	m_pWhiteBalanceBar = new SSToolBoxItem(this, QObject::tr("White Balance"));
	m_listBar << m_pWhiteBalanceBar;

	QHBoxLayout *pLayout = new QHBoxLayout;
	pLayout->addStretch();
	for (int i = 0; i < 4; i++)
	{
		QToolButton *pButton = new QToolButton;
		pLayout->addWidget(pButton);
	}
	pLayout->addStretch();
	pLayout->setSpacing(4);

	m_pWhiteBalanceBar->setContentLayout(pLayout);
}

void SSTestToolBox::createImageAdjustmentBar()
{
	//图像调整
	m_pImageAdjustment = new SSToolBoxItem(this, QObject::tr("Image Adjustment"));
	m_listBar << m_pImageAdjustment;

	QHBoxLayout *pLayout = new QHBoxLayout;
	pLayout->addStretch();
	for (int i = 0; i < 4; i++)
	{
		QToolButton *pButton = new QToolButton;
		pLayout->addWidget(pButton);
	}
	pLayout->addStretch();
	pLayout->setSpacing(4);

	m_pImageAdjustment->setContentLayout(pLayout);
}

void SSTestToolBox::createHistogramBar()
{
	//直方图
	m_pHistogram = new SSToolBoxItem(this, QObject::tr("Histogram"));
	m_listBar << m_pHistogram;

	QHBoxLayout *pLayout = new QHBoxLayout;
	pLayout->addStretch();
	for (int i = 0; i < 4; i++)
	{
		QToolButton *pButton = new QToolButton;
		pLayout->addWidget(pButton);
	}
	pLayout->addStretch();
	pLayout->setSpacing(4);

	m_pHistogram->setContentLayout(pLayout);
}
