#include "SelectToolBox.h"
#include "SSToolBoxItem.h"
#include "SelectWidget.h"
#include "Transformation.h"
#include "drawscene.h"
#include "WorkingWid.h"
#include "ThresholdW.h"
#include "ExportWidget.h"
#include "TextWidget.h"
#include "GrayDiffuse.h"
#include "OutLineImage.h"
#include "SGcodeGenerate.h"
#include "ModelSelect.h"

#include <QToolButton>
#include <QStackedWidget>
#include <QButtonGroup>
#include <QPainter>


SelectToolBox::SelectToolBox(QWidget *parent)
	: SSToolBox(parent)
{
	tem_Trans = nullptr;
	m_WorkingWid = nullptr;
	//创建控件
	createScrollArea();
	createResolutionBar();
	createExposureBar();
	//createWhiteBalanceBar();
	//createImageAdjustmentBar();
	//createHistogramBar();

	//初始化
	initLayout();

    
}

SelectToolBox::~SelectToolBox()
{
}

void SelectToolBox::ResetBox()
{
	QList<SSToolBoxItem*>  childlist = m_pResolutionBar->findChildren<SSToolBoxItem*>();
	
	for (SSToolBoxItem* item : childlist)
	{
		item->setVisible(false);
		//item->updateGeometry();
		item->setContentExpend(true);
		item->stateSwitch();
		item->updateGeometry();
		item->adjustSize();
	}
	m_pResolutionBar->updateGeometry();
	m_pResolutionBar->adjustSize();
	upDataG();
}

//void SelectToolBox::paintEvent(QPaintEvent * event)
//{
//	QStyleOption  opt;
//	opt.initFrom(this);
//	QPainter p(this);
//	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//}

void SelectToolBox::buttonGroupClicked(int id)
{
	//adjectSelfSize(id);	
	emit LevelButtonClicked(id);
}

void SelectToolBox::gcodeButtonClicked()
{
	emit CreateGcode(1);
}

void SelectToolBox::adjectSelfSize(int id)
{
	if (nullptr != m_pResolutionBar)
	{
		QList<SSToolBoxItem*>   childlist = m_pResolutionBar->findChildren<SSToolBoxItem*>();
		for (SSToolBoxItem* item : childlist)
		{
			item->setVisible(false);
			if (item->getFlag() == id)
			{
				item->setVisible(true);
			}
		}
	}
	//adjustScrollAreaWidgetSize();	
}

void SelectToolBox::SelectFromFile(bool selected, const SelectData & data)
{
	QList<SSToolBoxItem*>  childlist = m_pResolutionBar->findChildren<SSToolBoxItem*>();
	SSToolBoxItem * trans = m_pResolutionBar->findChild<SSToolBoxItem*>("Transformation");
	SSToolBoxItem* work = m_pResolutionBar->findChild<SSToolBoxItem*>("WorkingResolution");
	SSToolBoxItem* yuzhi = m_pResolutionBar->findChild<SSToolBoxItem*>("ThresholdResolution");
	
	if (selected)
	{
		QVariant data1 = QVariant::fromValue(data);
		for (SSToolBoxItem* item : childlist)
		{
			item->setVisible(false);
		}
		if (data.m_Type == BLACKANDWHITE)
		{
			SetBlackWhitePart(data1);
		}
		else if (data.m_Type == GRAY)
		{
			setGrayPart(data1);
		}
		else if (data.m_Type == VECTORS)
		{
			SetVectorPart(data1);
		}
		else if (data.m_Type == TEXT)
		{
			SetTextPart(data1);
		}
		if (nullptr != tem_Trans)
		{
			tem_Trans->setWidthEnable(true);
			tem_Trans->setHeightEnable(true);
			tem_Trans->setWidth(data.m_TransData.width);
			tem_Trans->setHeight(data.m_TransData.height);
			tem_Trans->rotateValue(data.m_TransData.rotate);
			tem_Trans->moveXValue(data.m_TransData.movex);
			tem_Trans->moveYValue(data.m_TransData.movey);
			if (data.m_Type == TEXT)
			{
				tem_Trans->setWidthEnable(false);
				tem_Trans->setHeightEnable(false);
			}
		}
		if (m_WorkingWid != nullptr)
		{
			m_WorkingWid->setPrintingOrder(data.m_WorkingData.printOrder);
			m_WorkingWid->setInchingSpeed(data.m_WorkingData.inchingSpeed);
			m_WorkingWid->setWorkingSpeed(data.m_WorkingData.workingSpeed);
		}

	}
	else
	{
		for (SSToolBoxItem* item : childlist)
		{
	        item->setVisible(false);
		}
	}
	//adjustScrollAreaWidgetSize();
}

void SelectToolBox::TextAndFontChanged(QString & text, QString & font)
{
	QGraphicsItem::GraphicsItemChange  data = QGraphicsItem::ItemToolTipChange;//文字的改变 
	TextData wdata;
	wdata.text = text;
	wdata.font = font;
	QVariant value = QVariant::fromValue(wdata);
	emit NotifySceneStateChange(data, value);
}

void SelectToolBox::handleOutlineChange(OutLineStruct & data)
{
	QGraphicsItem::GraphicsItemChange  data1 = QGraphicsItem::ItemToolTipHasChanged;//用作outline算法的改变
	OutLineStruct vdata = data;
	QVariant value = QVariant::fromValue(vdata);
	emit NotifySceneStateChange(data1, value);
}

void SelectToolBox::handleSceneEvent(QGraphicsItem::GraphicsItemChange change, const QVariant & value)
{

	QList<SSToolBoxItem*>  childlist = m_pResolutionBar->findChildren<SSToolBoxItem*>();
	//扎到trans组件 
	SSToolBoxItem * trans = m_pResolutionBar->findChild<SSToolBoxItem *>("Transformation");

	switch (change)
	{
	case QGraphicsItem::ItemPositionChange:
		break;
	case QGraphicsItem::ItemMatrixChange:
		break;
	case QGraphicsItem::ItemVisibleChange:
		break;
	case QGraphicsItem::ItemEnabledChange:
		break;
	case QGraphicsItem::ItemSelectedChange:
		break;
	case QGraphicsItem::ItemParentChange:
		break;
	case QGraphicsItem::ItemChildAddedChange:
		break;
	case QGraphicsItem::ItemChildRemovedChange:
		break;
	case QGraphicsItem::ItemTransformChange:
		break;
	case QGraphicsItem::ItemPositionHasChanged:
	{
		handlePositionChange(childlist, trans, value);
	}
		break;
	case QGraphicsItem::ItemTransformHasChanged:
		break;
	case QGraphicsItem::ItemSceneChange:
		break;
	case QGraphicsItem::ItemVisibleHasChanged:
		break;
	case QGraphicsItem::ItemEnabledHasChanged:
		break;
	case QGraphicsItem::ItemSelectedHasChanged:
	{
		QVariant value1 = value;
		handleSelectedChanged(value1);
	}
		break;
	case QGraphicsItem::ItemParentHasChanged:
		break;
	case QGraphicsItem::ItemSceneHasChanged:
		break;
	case QGraphicsItem::ItemCursorChange:
		break;
	case QGraphicsItem::ItemCursorHasChanged:
		break;
	case QGraphicsItem::ItemToolTipChange://暂时用作文字改变的消息 
	{

	}
		break;
	case QGraphicsItem::ItemToolTipHasChanged:
		break;
	case QGraphicsItem::ItemFlagsChange:
		break;
	case QGraphicsItem::ItemFlagsHaveChanged:
		break;
	case QGraphicsItem::ItemZValueChange:
		break;
	case QGraphicsItem::ItemZValueHasChanged:
		break;
	case QGraphicsItem::ItemOpacityChange:
		break;
	case QGraphicsItem::ItemOpacityHasChanged:
		break;
	case QGraphicsItem::ItemScenePositionHasChanged:
		break;
	case QGraphicsItem::ItemRotationChange:
		break;
	case QGraphicsItem::ItemRotationHasChanged:
	{
		handleRotationChange(childlist, trans, value);
	}
		break;
	case QGraphicsItem::ItemScaleChange:
		break;
	case QGraphicsItem::ItemScaleHasChanged:
	{
		handleScaleChanged(childlist, trans, value);
	}
		break;
	case QGraphicsItem::ItemTransformOriginPointChange:
		break;
	case QGraphicsItem::ItemTransformOriginPointHasChanged:
		break;
	default:
		break;
	}
}

void SelectToolBox::handlePositionChange(QList<SSToolBoxItem*>& childlist, SSToolBoxItem * trans, const QVariant & value)
{
    if (tem_Trans)
	{
		tem_Trans->moveXValue(value.toPointF().x());
		tem_Trans->moveYValue(value.toPointF().y());
		//transchild->setVisible(true);
	}
}

void SelectToolBox::handleRotationChange(QList<SSToolBoxItem*>& childlist, SSToolBoxItem * trans, const QVariant & value)
{
	if (tem_Trans)
	{
		tem_Trans->rotateValue(value.toDouble());
	}
}

void SelectToolBox::handleScaleChanged(QList<SSToolBoxItem*>& childlist, SSToolBoxItem * trans, const QVariant & value)
{
	if (tem_Trans)
	{
		tem_Trans->setWidth(value.toPointF().x());
		tem_Trans->setHeight(value.toPointF().y());
	}
}

void SelectToolBox::handleSelectedChanged(QVariant & value)
{
	QList<SSToolBoxItem*>  childlist = m_pResolutionBar->findChildren<SSToolBoxItem*>();
	if (!value.canConvert<SelectData>())
	{
		for (SSToolBoxItem* item : childlist)
		{
			item->setVisible(false);
			//item->updateGeometry();
			item->setContentExpend(true);
			item->stateSwitch();
			item->updateGeometry();
			item->adjustSize();
		}
		m_pResolutionBar->updateGeometry();
		m_pResolutionBar->adjustSize();
		upDataG();
	}
	else
	{
		SelectData  data = value.value<SelectData>();
		if (data.m_Type == BLACKANDWHITE)
		{
			SetBlackWhitePart(value);
		}
		else if (data.m_Type == GRAY)
		{
			setGrayPart(value);
		}
		else if (data.m_Type == VECTORS)
		{
			SetVectorPart(value);
		}
		else if (data.m_Type == TEXT)
		{
			SetTextPart(value);
		}
		//upDataG();
		m_pResolutionBar->updateGeometry();
		int n = m_pResolutionBar->height();
		int p = n;
	}	
}

void SelectToolBox::setGrayPart(const QVariant& value)
{
	SSToolBoxItem * trans = m_pResolutionBar->findChild<SSToolBoxItem*>("Transformation");
	SSToolBoxItem* work = m_pResolutionBar->findChild<SSToolBoxItem*>("WorkingResolution");
	SSToolBoxItem* gray = m_pResolutionBar->findChild<SSToolBoxItem*>("GrayResolution");
	if (trans != nullptr)
	{
		trans->setVisible(true);
		trans->setContentExpend(false);
		trans->stateSwitch();
	}
	if (work != nullptr)
	{
		work->setVisible(true);
		work->setContentExpend(false);
		work->stateSwitch();
	}
	if (gray != nullptr)
	{
		gray->setVisible(true);
		gray->setContentExpend(false);
		gray->stateSwitch();
	}
	if (nullptr != m_graywidget)
	{
		SelectData  idata = value.value<SelectData>();
		m_graywidget->SetGrayDiff(idata.m_GrayDiff);
	}
	updateGeometry();
	adjustSize();
}

void SelectToolBox::SetBlackWhitePart(const QVariant& value)
{
		SSToolBoxItem * trans = m_pResolutionBar->findChild<SSToolBoxItem*>("Transformation");
		SSToolBoxItem* work = m_pResolutionBar->findChild<SSToolBoxItem*>("WorkingResolution");
		SSToolBoxItem* yuzhi = m_pResolutionBar->findChild<SSToolBoxItem*>("ThresholdResolution");
		if (trans != nullptr)
		{
			trans->setVisible(true);
			trans->setContentExpend(false);
			trans->stateSwitch();
		}
		if (work != nullptr)
		{
			work->setVisible(true);
			work->setContentExpend(false);
			work->stateSwitch();
		}
		if (yuzhi != nullptr)
		{
			yuzhi->setVisible(true);
			yuzhi->setContentExpend(false);
			yuzhi->stateSwitch();
			SelectData  data = value.value<SelectData>();
			if (m_ThresholdW)
			{
				m_ThresholdW->setThreshold(data.m_Threlod);
			}

		}

		updateGeometry();
		adjustSize();
}

void SelectToolBox::SetVectorPart(const QVariant& value)
{
	SSToolBoxItem * trans = m_pResolutionBar->findChild<SSToolBoxItem*>("Transformation");
	SSToolBoxItem* work = m_pResolutionBar->findChild<SSToolBoxItem*>("WorkingResolution");
	SSToolBoxItem* outline = m_pResolutionBar->findChild<SSToolBoxItem*>("OutLineResolution");
	SSToolBoxItem* yuzhi = m_pResolutionBar->findChild<SSToolBoxItem*>("ThresholdResolution");
	if (trans != nullptr)
	{
		trans->setVisible(true);
		trans->setContentExpend(false);
		trans->stateSwitch();
	}
	if (work != nullptr)
	{
		work->setVisible(true);
		work->setContentExpend(false);
		work->stateSwitch();
	}
	if (VECTOR_SELECT)
	{
		if (outline != nullptr)
		{
			outline->setVisible(true);
			outline->setContentExpend(false);
			outline->stateSwitch();
		}
	}
	else
	{

		if (yuzhi != nullptr)
		{
			yuzhi->setVisible(true);
			yuzhi->setContentExpend(false);
			yuzhi->stateSwitch();
			SelectData  data = value.value<SelectData>();
			if (m_ThresholdW)
			{
				m_ThresholdW->setThreshold(data.m_Threlod,1);
			}

		}
	}
	updateGeometry();
	adjustSize();
}

void SelectToolBox::SetTextPart(const QVariant& value)
{
	SSToolBoxItem * trans = m_pResolutionBar->findChild<SSToolBoxItem*>("Transformation");
	SSToolBoxItem* work = m_pResolutionBar->findChild<SSToolBoxItem*>("WorkingResolution");
	SSToolBoxItem* yuzhi = m_pResolutionBar->findChild<SSToolBoxItem*>("ThresholdResolution");
	SSToolBoxItem* ptext = m_pResolutionBar->findChild<SSToolBoxItem*>("TextResolution");
	if (trans != nullptr)
	{
		trans->setVisible(true);
		trans->setContentExpend(false);
		trans->stateSwitch();
	}
	if (work != nullptr)
	{
		work->setVisible(true);
		work->setContentExpend(false);
		work->stateSwitch();
	}
	if (ptext != nullptr)
	{
		ptext->setVisible(true);
		ptext->setContentExpend(false);
		ptext->stateSwitch();

		if (nullptr != m_outline)
		{
			OutLineStruct  idata = value.value<OutLineStruct>();
			m_outline->setOutLineState(idata);
		}
	}
	updateGeometry();
	adjustSize();
}

void SelectToolBox::ItemwidthChanged(int width)
{
	QGraphicsItem::GraphicsItemChange  data = QGraphicsItem::ItemScaleHasChanged;
	QVariant value(width);
	emit NotifySceneStateChange(data, value);
}

void SelectToolBox::ItemheightChanged(int height)
{
	QGraphicsItem::GraphicsItemChange  data = QGraphicsItem::ItemScaleChange;
	QVariant value(height);
	emit NotifySceneStateChange(data, value);
}

void SelectToolBox::ItemrotateChanged(int r)
{
	QGraphicsItem::GraphicsItemChange  data = QGraphicsItem::ItemRotationHasChanged;
	QVariant value(r);
	emit NotifySceneStateChange(data, value);
}

void SelectToolBox::ItemxChanged(int x)
{
	QGraphicsItem::GraphicsItemChange  data = QGraphicsItem::ItemPositionChange;//x的改变 
	QVariant value(x);
	emit NotifySceneStateChange(data, value);
}

void SelectToolBox::ItemyChanged(int y)
{
	QGraphicsItem::GraphicsItemChange  data = QGraphicsItem::ItemPositionHasChanged;//y的改变 
	QVariant value(y);
	emit NotifySceneStateChange(data, value);
}

void SelectToolBox::ItemGrayDiffuseChanged(int n)
{
	QGraphicsItem::GraphicsItemChange  data = QGraphicsItem::ItemMatrixChange;//灰度算法的改变 
	QVariant value(n);
	emit NotifySceneStateChange(data, value);
}

void SelectToolBox::ItemFlipChanged(int flip)
{
	QGraphicsItem::GraphicsItemChange  data = QGraphicsItem::ItemParentChange;//FLIP改变 
	QVariant value(flip);
	emit NotifySceneStateChange(data, value);
}

void SelectToolBox::ItemUpChanged()
{
	QGraphicsItem::GraphicsItemChange  data = QGraphicsItem::ItemEnabledChange;//向上移 
	QVariant value(true);
	emit NotifySceneStateChange(data, value);
}

void SelectToolBox::ItemDownChanged()
{
	QGraphicsItem::GraphicsItemChange  data = QGraphicsItem::ItemEnabledChange;//向下移 
	QVariant value(false);
	emit NotifySceneStateChange(data, value);
}

void SelectToolBox::SceneLineChanged()
{
	QGraphicsItem::GraphicsItemChange  data = QGraphicsItem::ItemSceneChange;//辅助线改变
	QVariant value(false);
	emit NotifySceneStateChange(data, value);
}

void SelectToolBox::ItemprintOrderChanged(int n)
{
	QGraphicsItem::GraphicsItemChange  data = QGraphicsItem::ItemOpacityChange;//透明度的改变  可以先做order的改变用
	QVariant value(n);
	emit NotifySceneStateChange(data,value);
}

void SelectToolBox::IteminchingSpeedChanged(int n)
{
	QGraphicsItem::GraphicsItemChange  data = QGraphicsItem::ItemOpacityHasChanged;//inching  速度的改变 
	QVariant value(n);
	emit NotifySceneStateChange(data, value);
}

void SelectToolBox::ItemworkingSpeedChanged(int n)
{
	QGraphicsItem::GraphicsItemChange  data = QGraphicsItem::ItemFlagsHaveChanged;//工作速度的改变 
	QVariant value(n);
	emit NotifySceneStateChange(data, value);
}

void SelectToolBox::ItemThresholdChanged(int n, uint type)
{
	QGraphicsItem::GraphicsItemChange  data = QGraphicsItem::ItemFlagsChange;//阈值的改变
	YuzhiData  data1;
	data1.Threold = n;
	data1.type = type;
	QVariant value = QVariant::fromValue(data1);
	emit NotifySceneStateChange(data, value);
}

void SelectToolBox::buttonClicked()
{

}

//创建第一个bar 
void SelectToolBox::createResolutionBar()
{

    m_pResolutionBar = new SSToolBoxItem(this, QObject::tr("预览设置"));
	m_pResolutionBar->setObjectName("m_pResolutionBar");
	m_listBar << m_pResolutionBar;

	//纵向列表框 
	QVBoxLayout *pLayout = new QVBoxLayout;
	pLayout->setContentsMargins(0,0,0,0);
	pLayout->setSpacing(0);
	//pLayout->addStretch();

	//
	ModelSelect* s_modelSelect = new ModelSelect();
	s_modelSelect->setObjectName("s_modelSelect");
	//s_modelSelect->setVisible(true);
    s_modelSelect->setStyleSheet("QWidget#s_modelSelect{background-color:white;border-bottom:1px solid rgb(242,242,242);}");
	//s_modelSelect->setContentsMargins(2, 2, 2, 2);
	//s_modelSelect->setFixedWidth(TOOLBOX_CHILD_WIDTH);
	//s_modelSelect->setFixedHeight(80);
	pLayout->addWidget(s_modelSelect);
	//int n = s_modelSelect->width();
	//int p = s_modelSelect->height();
	//pLayout->addSpacing(2);
	//pLayout->setSpacing(2);

	//组装四个按钮 
	SelectWidget* selectA = new SelectWidget(this);
	selectA->setObjectName("selectA");
	selectA->setStyleSheet("QWidget#selectA{background-color:white;border-bottom:1px solid rgb(242,242,242);}");
	//connect(selectA,&SelectWidget::SelectModeClicked,this,&SelectToolBox::buttonGroupClicked);
	connect(selectA,SIGNAL(SelectModeClicked(int)),this,SLOT(buttonGroupClicked(int)));
	selectA->setContentsMargins(0,0,0,0);
	selectA->setFixedWidth(TOOLBOX_CHILD_WIDTH);
	//selectA->setFixedHeight(128);
    pLayout->addWidget(selectA);
	int m = selectA->height();
	//pLayout->addStretch();

	//组装测试按钮
	SSToolBoxItem *m_pResolutionBarChild = new SSToolBoxItem(this, QObject::tr("Transformation"));
	m_pResolutionBarChild->setObjectName("Transformation");
	m_pResolutionBarChild->setFlag(0);
	m_pResolutionBarChild->setVisible(false);
	QHBoxLayout *pLayoutChild = new QHBoxLayout;
	pLayoutChild->setContentsMargins(0,0,0,0);
	//pLayoutChild->addStretch();

	tem_Trans = new Transformation(this);
	tem_Trans->setObjectName("transcomponent");
	tem_Trans->setStyleSheet("QWidget{background-color:white;border-bottom:1px solid rgb(242,242,242);}");
	
	/*
	connect(tem_Trans,&Transformation::widthChanged,this,&SelectToolBox::ItemwidthChanged); //int 
	connect(tem_Trans,&Transformation::heightChanged, this, &SelectToolBox::ItemheightChanged);//int  
	connect(tem_Trans,&Transformation::rotateChanged, this, &SelectToolBox::ItemrotateChanged);//int 
	connect(tem_Trans,&Transformation::xChanged, this, &SelectToolBox::ItemxChanged);//int 
	connect(tem_Trans,&Transformation::yChanged, this, &SelectToolBox::ItemyChanged);// int 
	connect(tem_Trans,&Transformation::flipChanged,this,&SelectToolBox::ItemFlipChanged);//int
	connect(tem_Trans,&Transformation::upChanged,this,&SelectToolBox::ItemUpChanged);//
	connect(tem_Trans,&Transformation::downChanged,this,&SelectToolBox::ItemDownChanged);// 
	connect(tem_Trans,&Transformation::lineChanged,this,&SelectToolBox::SceneLineChanged);// 
	*/
	connect(tem_Trans, SIGNAL(widthChanged(int)), this,SLOT(ItemwidthChanged(int))); //int 
	connect(tem_Trans, SIGNAL(heightChanged(int)), this,SLOT(ItemheightChanged(int)));//int  
	connect(tem_Trans, SIGNAL(rotateChanged(int)), this,SLOT(ItemrotateChanged(int)));//int 
	connect(tem_Trans, SIGNAL(xChanged(int)), this,SLOT(ItemxChanged(int)));//int 
	connect(tem_Trans, SIGNAL(yChanged(int)), this,SLOT(ItemyChanged(int)));// int 
	connect(tem_Trans, SIGNAL(flipChanged(int)), this,SLOT(ItemFlipChanged(int)));//int
	connect(tem_Trans, SIGNAL(upChanged()), this,SLOT(ItemUpChanged()));//
	connect(tem_Trans, SIGNAL(downChanged()), this,SLOT(ItemDownChanged()));// 
	connect(tem_Trans, SIGNAL(lineChanged()), this,SLOT(SceneLineChanged()));// 


	pLayoutChild->addWidget(tem_Trans);
	//pLayoutChild->addStretch();
	//pLayoutChild->setSpacing(4);
	m_pResolutionBarChild->setContentLayout(pLayoutChild);

	pLayout->addWidget(m_pResolutionBarChild);

	SSToolBoxItem *m_pResolutionBarChildS = new SSToolBoxItem(this, QObject::tr("WorkingResolution"));
	m_pResolutionBarChildS->setObjectName("WorkingResolution");
	m_pResolutionBarChildS->setFlag(1);
	m_pResolutionBarChildS->setVisible(false);
	QHBoxLayout* pLayoutChilds = new QHBoxLayout;
	pLayoutChilds->setContentsMargins(0,0,0,0);
	//pLayoutChilds->addStretch();
	
	m_WorkingWid = new WorkingWid(this);
	m_WorkingWid->setObjectName("WorkingWid");
	m_WorkingWid->setStyleSheet("QWidget#WorkingWid{background-color:white;border-bottom:1px solid rgb(242,242,242);}");

	//connect(m_WorkingWid,&WorkingWid::printOrderChanged,this, &SelectToolBox::ItemprintOrderChanged);
	//connect(m_WorkingWid,&WorkingWid::inchingSpeedChanged,this, &SelectToolBox::IteminchingSpeedChanged);
	//connect(m_WorkingWid,&WorkingWid::workingSpeedChanged,this,&SelectToolBox::ItemworkingSpeedChanged);

	connect(m_WorkingWid,SIGNAL(printOrderChanged(int )),this,SLOT(ItemprintOrderChanged(int )));
	connect(m_WorkingWid,SIGNAL(inchingSpeedChanged(int)),this,SLOT(IteminchingSpeedChanged(int)));
	connect(m_WorkingWid,SIGNAL(workingSpeedChanged(int)),this,SLOT(ItemworkingSpeedChanged(int)));

	pLayoutChilds->addWidget(m_WorkingWid);
	//pLayoutChilds->addStretch();
	//pLayoutChilds->setSpacing(4);
	m_pResolutionBarChildS->setContentLayout(pLayoutChilds);

	pLayout->addWidget(m_pResolutionBarChildS);

	SSToolBoxItem* m_pResolutionBarChildA = new SSToolBoxItem(this,QObject::tr("ThresholdResolution"));
	m_pResolutionBarChildA->setObjectName("ThresholdResolution");
	m_pResolutionBarChildA->setFlag(1);
	m_pResolutionBarChildA->setVisible(false);

	QHBoxLayout *pLayoutChildA = new QHBoxLayout;
	m_ThresholdW = new ThresholdW(this);
	m_ThresholdW->setObjectName("ThresholdW");
	m_ThresholdW->setStyleSheet("QWidget#ThresholdW{background-color:white;border:none;}");
	//int n, uint type
	//connect(m_ThresholdW,&ThresholdW::ThresholdChanged,this, &SelectToolBox::ItemThresholdChanged);
	connect(m_ThresholdW,SIGNAL(ThresholdChanged(int ,uint)),this,SLOT(ItemThresholdChanged(int, uint)));
	pLayoutChildA->setContentsMargins(0,0,0,0);
	pLayoutChildA->addWidget(m_ThresholdW);
	//pLayoutChildA->addStretch();
	//pLayoutChildA->setSpacing(4);
	m_pResolutionBarChildA->setContentLayout(pLayoutChildA);

	pLayout->addWidget(m_pResolutionBarChildA);

	SSToolBoxItem *m_pResolutionBarChildD = new SSToolBoxItem(this,QObject::tr("TextResolution"));
	m_pResolutionBarChildD->setObjectName("TextResolution");
	m_pResolutionBarChildD->setFlag(0);
	m_pResolutionBarChildD->setVisible(false);
	QHBoxLayout *pLayoutChildD = new QHBoxLayout;
	m_pTextWidget = new TextWidget(this);
	m_pTextWidget->setObjectName("m_pTextWidget");
	m_pTextWidget->setStyleSheet("QWidget#m_pTextWidget{background-color:white;border-bottom:1px solid rgb(242,242);}");
	pLayoutChildD->addWidget(m_pTextWidget);
	pLayoutChildD->setContentsMargins(0,0,0,0);
	//connect(m_pTextWidget,&TextWidget::CurrentTextAndFont, this, &SelectToolBox::TextAndFontChanged);
	connect(m_pTextWidget,SIGNAL(CurrentTextAndFont(QString &,QString&)),this,SLOT(TextAndFontChanged(QString &, QString&)));

	//pLayoutChildD->setSpacing(4);
	m_pResolutionBarChildD->setContentLayout(pLayoutChildD);

	pLayout->addWidget(m_pResolutionBarChildD);

	SSToolBoxItem *m_pResolutionBarChildE = new SSToolBoxItem(this, QObject::tr("GrayResolution"));
	m_pResolutionBarChildE->setObjectName("GrayResolution");
	m_pResolutionBarChildE->setFlag(0);
	m_pResolutionBarChildE->setVisible(false);
	QHBoxLayout *pLayoutChildE = new QHBoxLayout;
	m_graywidget = new GrayDiffuse();
	m_graywidget->setObjectName("graywidget");
	m_graywidget->setStyleSheet("QWidget#graywidget{background-color:white;border-bottom:1px solid rgb(242,242,242);}");
	pLayoutChildE->addWidget(m_graywidget);
	pLayoutChildE->setContentsMargins(0,0,0,0);
	//connect(m_graywidget,&GrayDiffuse::GrayDiffuseChanged,this,&SelectToolBox::ItemGrayDiffuseChanged);
	connect(m_graywidget,SIGNAL(GrayDiffuseChanged(int)),this,SLOT(ItemGrayDiffuseChanged(int)));

	//pLayoutChildE->setSpacing(4);
	m_pResolutionBarChildE->setContentLayout(pLayoutChildE);

	pLayout->addWidget(m_pResolutionBarChildE);

	SSToolBoxItem *m_pResolutionBarChildF = new SSToolBoxItem(this, QObject::tr("OutLineResolution"));
	m_pResolutionBarChildF->setObjectName("OutLineResolution");
	m_pResolutionBarChildF->setFlag(0);
	m_pResolutionBarChildF->setVisible(false);
	QHBoxLayout *pLayoutChildF= new QHBoxLayout;
	m_outline = new OutLineImage();
	m_outline->setObjectName("m_outline");
	m_outline->setStyleSheet("QWidget#m_outline{background-color:white;border-bottom:1px solid rgb(242,242,242);}");
	pLayoutChildF->setContentsMargins(0,0,0,0);
	pLayoutChildF->addWidget(m_outline);
	//OutLineStruct&
	//connect(m_outline, &OutLineImage::NotifyArithmeticChange, this, &SelectToolBox::handleOutlineChange);
	connect(m_outline,SIGNAL(NotifyArithmeticChange(OutLineStruct&)),this,SLOT(handleOutlineChange(OutLineStruct&)));

	//pLayoutChildE->setSpacing(4);
	m_pResolutionBarChildF->setContentLayout(pLayoutChildF);

	pLayout->addWidget(m_pResolutionBarChildF);

	//pLayout->addStretch();
	//pLayout->setSpacing(4);

	m_pResolutionBar->setContentLayout(pLayout);
	m_pResolutionBar->stateSwitch();
	//m_pResolutionBar->setStyleSheet("QWidget#m_pResolutionBar{border-bottom:1px solid green;}");
	//QSize N = m_pResolutionBar->size();
	//m_BaseSize  = N;
}

void SelectToolBox::createExposureBar()
{
	//曝光控制
	m_pExposureBar = new SSToolBoxItem(this, QObject::tr(u8"输出G代码"));
	m_listBar << m_pExposureBar;

	QHBoxLayout *pLayout = new QHBoxLayout;
	ExportWidget* exportw = new ExportWidget(this);
	exportw->setObjectName("exportw");
	exportw->setStyleSheet("QWidget#exportw{background-color:white;border-bottom:1px solid rgb(242,242,242);}");
	//exportw->setFixedSize(350, 155);
	//connect(exportw,&ExportWidget::GcodeButtonClicked,this, &SelectToolBox::gcodeButtonClicked);
	connect(exportw,SIGNAL(GcodeButtonClicked(int)),this,SLOT(gcodeButtonClicked()));
	/*pLayout->addStretch();
	for (int i = 0; i < 4; i++)
	{
		QToolButton *pButton = new QToolButton;
		pLayout->addWidget(pButton);
	}
	pLayout->addStretch();*/
	pLayout->setContentsMargins(0,0,0,0);
	pLayout->addWidget(exportw);
	//pLayout->setSpacing(4);

	m_pExposureBar->setContentLayout(pLayout);
	m_pExposureBar->stateSwitch();
	//m_pExposureBar->setStyleSheet("QWidget{border:1px solid red;}");
}

void SelectToolBox::createWhiteBalanceBar()
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

void SelectToolBox::createImageAdjustmentBar()
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

void SelectToolBox::createHistogramBar()
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
