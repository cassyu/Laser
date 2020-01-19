
//#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
//#include <opencv/cv.h>
#include <QTextCodec>
#include <fstream>  
#include <iostream>
#include <QDateTime>
#include <string>  
#include <iostream>  
#include "CustomTabStyle.h"
#include <QGraphicsSimpleTextItem>
#include <opencv2/imgproc/types_c.h>

//#include <Qt3DRender/qcamera.h>
//#include <Qt3DCore/qentity.h>
//#include <Qt3DRender/qcameralens.h>

//#include <Qt3DInput/QInputAspect>

#include <QtGui/QScreen>
/*
#include <Qt3DExtras/qtorusmesh.h>
#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qeffect.h>
#include <Qt3DRender/qtexture.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qpointlight.h>

#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/qaspectengine.h>

#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>

#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>
*/
#include <QFileDialog>

#include <QButtonGroup>
#include <QToolButton>
#include <QAbstractButton>

#include <QtWidgets>
#include <QLabel>

//#include "SceneModifier.h"
#include "Arrow.h"
#include "dragramitem.h"
#include "diagramScene.h"
#include "diagramTextItem.h"
#include "drawscene.h"
#include "drawtool.h"
#include "GraphWidget.h"
#include "StatusWidget.h"
#include "ShrinkAnimation.h"
#include "LaserWidget.h"
#include "SelectWidget.h"
#include "SSTestToolBox.h"
#include <QStackedWidget>
#include <QScrollArea>
#include <ExportWidget.h>
#include "QtOpencvConvert.h"
#include "GrayDiffuse.h"
#include "OutLineImage.h"
//#include <QtSerialPort>
#include "drawobj.h"
#include "SGcodeGenerate.h"
#include <QToolBox>
#include "SelectToolBox.h"
//#include <QToolBox>

#include "SelectWidget.h"

#include <QGraphicsDropShadowEffect>
#include <QTranslator>



using namespace std;
//using namespace cv;

#define MAX_CMD_SIZE 96

const int InsertTextButton = 10;

MainWindow*  MainWindow::m_pMainWindow;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	this->setObjectName("Mainwindow");

	itemMenu = nullptr;
    //查找可用的串口
 /*   foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		QSerialPort serial;
		serial.setPort(info);
		if(serial.open(QIODevice::ReadWrite))
		{
			ui->PortBox->addItem(serial.portName());
			serial.close();
		}
	}*/
    //设置波特率下拉菜单默认显示第三项
    ui->BaudBox->setCurrentIndex(3);
    //关闭发送按钮的使能
    ui->sendButton->setEnabled(false);
    //qDebug() << tr("jjj");


	ui->pushButton_2->setEnabled(true);
	//connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::RenderPic);
	//connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::RenderPic);

	ui->Chuankou->setVisible(false);

	ui->tabWidget->setTabPosition(QTabWidget::West);
	//ui->tabWidget->tabBar()->setStyle(new CustomTabStyle);
	ui->tabWidget->setTabText(0,QObject::tr("激光 图片G代码生成器"));//CNC G代码生成器
	ui->tabWidget->setTabText(1,QObject::tr("激光 文字G代码生成器"));
	ui->tabWidget->setTabText(2,QObject::tr("3d打印 G代码生成器"));
	ui->tabWidget->setTabText(3,QObject::tr("test"));
	ui->tabWidget->setTabText(4,QObject::tr("激光 图片G代码生成器"));
	ui->tabWidget->setTabText(5,QObject::tr("osg view"));

	ui->tabWidget->removeTab(0);
	ui->tabWidget->removeTab(0);
	ui->tabWidget->removeTab(0);
	ui->tabWidget->removeTab(0);
	ui->tabWidget->removeTab(1);

	//CreateToolbar();
	Create3dView();
//////////////////////创建3d部分完结////////////////////////////////
    //CreategrapView();

	//CreateCenterWidget();
	createActions();
	createMenus();
	CreateFifthgrapView();

	//QWidget* centerwidget = new QWidget();
	//centerwidget->setLayout(ui->verticalLayout_4);
	//setCentralWidget(centerwidget);

	//CreateOsgView();

	ui->menuBar->setVisible(false);

	QSize MAIN_SIZE_MAX = QSize(16777215, 16777215);
	this->setMaximumSize(MAIN_SIZE_MAX);
	//this->setWindowFlag(Qt::WindowMaximizeButtonHint, true);
	//ui->widget_6->setVisible(true);
	//ui->tabWidget->setVisible(false);
	//setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::FramelessWindowHint| Qt::Tool);//| Qt::Tool

	m_pMainWindow = this;

	//QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
	//effect->setOffset(0,0);
	//effect->setColor(Qt::red);
	//effect->setBlurRadius(10);
	////ui->f
	//this->setGraphicsEffect(effect);
	QTranslator* trans = new QTranslator();
	trans->load("test_en.qm");
	qApp->installTranslator(trans);
}
//UI界面
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::CreategrapView()
{
	m_scene = new QGraphicsScene;
	ui->graphicsView->setScene(m_scene);
	m_scene->setSceneRect(QRect(0,0,300,300));
	ui->graphicsView->setBackgroundBrush(QBrush(Qt::black));

	QGraphicsSimpleTextItem* simpleTextItem = new  QGraphicsSimpleTextItem;
	simpleTextItem->setBrush(QColor(0,55,246));
	simpleTextItem->setText(u8"wwwww");
	simpleTextItem->setFont(QFont("微软雅黑",30));
	simpleTextItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);


	QGraphicsSimpleTextItem* simpleTextItem1 = new  QGraphicsSimpleTextItem;
	simpleTextItem1->setBrush(QColor(99,56, 246));
	simpleTextItem1->setText(u8"lfgdfgdfgdfg");
	simpleTextItem1->setFont(QFont("微软雅黑", 20));
	simpleTextItem1->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);


	m_scene->addItem(simpleTextItem);
	m_scene->addItem(simpleTextItem1);
}
void MainWindow::CreateCenterWidget()
{
	/*
	createActions();
	createMenus();
	scene = new  DiagramScene(itemMenu, ui->widget_4);
	scene->setSceneRect(QRectF(0,0,2000,2000));
	connect(scene, &DiagramScene::itemInserted,this, &MainWindow::itemInserted);
	connect(scene, &DiagramScene::textInserted,this, &MainWindow::textInserted);
	connect(scene, &DiagramScene::itemSelected,this, &MainWindow::itemSelected);

	//createToolBox();

	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(toolBox);
	view = new QGraphicsView(scene);
	layout->addWidget(view);
	ui->widget_4->setLayout(layout);
	*/

	/*QVBoxLayout* vlayout = new QVBoxLayout;
	vlayout->addWidget(ui->widget_6);
	vlayout->addWidget(ui->widget_4);

	QWidget* widget = new QWidget();
	widget->setLayout(vlayout);*/
	//setCentralWidget(widget);
	//int w = this->width();
	//int h = this->height();
	//int n = 10;
	//setFixedSize(QSize(w,h));
	//只有设置了这个值  才能启用最大化的按钮
	//this->toolBox()->setVisible(false);
	//dumpStructture(this, 0);
}

void MainWindow::GcodeClicked(int id)
{
	if (m_newscene != nullptr)
	{

		//之前的生成gcode的方法 
		//m_newscene->clearSelection();
		//QPixmap pixmap(400,400);
		//pixmap.fill();

		//QPainter painter(&pixmap);
		////painter.setBackground(QBrush(Qt::black));
		////painter.setBrush(QBrush(Qt::black));
		//painter.setRenderHint(QPainter::Antialiasing);
		////painter.begin();
		//m_newscene->render(&painter);
		//painter.end();
		////在这里生成gcode 代码  
		//QImage im = pixmap.toImage();
		//CreateGcode(im);
		//ui->label_3->setPixmap(pixmap);

		// new 
		// 找到工程中所有的item 
		//选择文件的保存路径 
		QString name = GetSelectFilePath();
		if (!name.isNull())
		{
			CreateGcodeNew(name);
		}
	}
}

void MainWindow::CreateFifthgrapView()
{
	m_newscene = new DrawScene(this);
	if (itemMenu)
	{
		m_newscene->setContextMenu(itemMenu);
	}
	m_newscene->setSceneRect(QRectF(-800 / 2, -800 / 2, 800, 800));

	//ui->graphicsView_2->setScene(m_newscene);
	//ui->graphicsView_2->setRenderHint(QPainter::Antialiasing);

	//m_scene->setBackgroundBrush(QPixmap(":/Resourse/u3.png"));

	//创建右侧的按钮选项组 
	//newbuttonGroup = new  QButtonGroup(this);
	//newbuttonGroup->setExclusive(false);
	//connect(newbuttonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &MainWindow::newbuttonGroupClicked);

	//QGridLayout* layout = new QGridLayout;
	////layout->addWidget(createCellWidget(tr("Conditional"), Dragramitem::Conditional),0,0);
	//layout->addWidget(createCellWidgetNew(tr(u8"黑与白"),tr("11"), BLACKANDWHITE), 0, 0);
	//layout->addWidget(createCellWidgetNew(tr(u8"灰度"), tr("22"), GRAY), 0, 1);
	//layout->addWidget(createCellWidgetNew(tr(u8"矢量"), tr("33"), VECTORS), 1, 0);
	//layout->addWidget(createCellWidgetNew(tr(u8"文本"), tr("44"), TEXT), 1, 1);

	//layout->setRowStretch(3, 10);
	//layout->setColumnStretch(2, 10);


	//QWidget* itemWidget = new QWidget;
	//itemWidget->setLayout(layout);

	//newtoolBox = new QToolBox;
	//newtoolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
	//newtoolBox->setMinimumWidth(itemWidget->sizeHint().width());
	//newtoolBox->addItem(itemWidget, tr("Basic  Shapes"));


	QHBoxLayout* layout1 = new QHBoxLayout;

	//CreateToolBoxA();
	//CretteToolBoxB();
	CreateToolBoxC();


	//newView = new QGraphicsView(m_newscene);
	//newView->setRenderHint(QPainter::Antialiasing);
	newViewA = new GraphWidget(this);
	newViewA->setScene(m_newscene);
	//newViewA->setFixedSize(825,660);
	newViewA->setRenderHint(QPainter::Antialiasing);
	newViewA->setObjectName("newViewA");
	newViewA->setStyleSheet("QWidget#newViewA{border:none;}\
    QScrollArea{background-color:white;}\
    QScrollBar{width:16px;background-color:white;}\
    QScrollBar:vertical{width:10px;background:white;border:none;}\
    QScrollBar::handle:vertical{width:8px;background-color:gray;margin-top:0px;margin-bottom:0px;border-radius:4px;}\
    QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:transparent;}\
    QScrollBar::sub-line:vertical{background:transparent;}\
    QScrollBar::add-line:vertival{background:transparent;}");

	layout1->addWidget(newViewA);
	//m_pToolBoxA->setFixedWidth(350);
	m_pToolBoxC->setFixedWidth(TOOLBOX_WIDTH);
	layout1->setSpacing(0);
	layout1->addWidget(m_pToolBoxC);
	m_pToolBoxC->setObjectName("ToolBoxC");
	//m_pToolBoxC->setStyleSheet("QWidget#ToolBoxC{background-color:red;border:none;}");
	layout1->setMargin(0);
	//ui->scrollArea->widget()->setLayout(layout1);
	//layout1->addWidget(ui->scrollArea);

	ui->widget_5->setStyleSheet("QWidget{border:1px solid rgb(242,242,242);background-color:white;}");
	//ui->widget_5->setContentsMargins(0,0,0,0);
	ui->widget_5->setLayout(layout1);

	QVBoxLayout* vlayout = new QVBoxLayout;
	vlayout->addWidget(ui->widget_6);
	vlayout->setSpacing(0);
	vlayout->addWidget(ui->widget_5);

	//int w = -1, h = -1, a = -1, d = -1;
	//vlayout->getContentsMargins(&w, &h, &a, &d);
	//vlayout->setMargin(0);

	QWidget* widget = new QWidget();
	widget->setLayout(vlayout);
	//widget->setContentsMargins(0,0,0,0);
	int w = -1, h = -1, a = -1, d = -1;
	//widget->getContentsMargins(&w,&h,&a,&d);


	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
	effect->setOffset(0,0);
	effect->setColor(Qt::gray);
	effect->setBlurRadius(14);
	//ui->f
	//this->setGraphicsEffect(effect);

	widget->setGraphicsEffect(effect);
	w = widget->height();
	h = widget->width();

	setCentralWidget(widget);
	w = widget->height();
	h = widget->width();
	QWidget* pw = centralWidget();
	w = pw->width();
	h = pw->height();
	//  1296   736  

	m_newscene->update();
	newViewA->update();


}

void MainWindow::dumpStructture(QObject * obj, int spacecount)
{
	qDebug() << QString("%1%2 : %3")
		.arg("", spacecount)
		.arg(obj->metaObject()->className())
		.arg(obj->objectName());
	qDebug() << QString("%1%2 : %3")
		.arg("", spacecount)
		.arg(obj->metaObject()->className())
		.arg(obj->objectName());
}

void MainWindow::CreateToolBoxA()
{
	
		//创建第一部分
		ShrinkAnimation*  shwidget = new ShrinkAnimation(QObject::tr("预览设置"),this);
		shwidget->setMinimumHeight(FIXHEIGHT_wid);
		//shwidget->setFixedWidth(200);
		SelectWidget* stawidget = new SelectWidget(this);
		//connect(stawidget, &SelectWidget::SelectModeClicked, this, &MainWindow::newbuttonGroupClicked);
		connect(stawidget,SIGNAL(SelectModeClicked(int)),this,SLOT(newbuttonGroupClicked(int)));
		//stawidget->setWindowFlag(Qt::Popup);
		//stawidget->setCusfixSizeHeight(50);
		stawidget->setFixedSize(350, 180);
		//stawidget->setFixedHeight(180);
		shwidget->initControl(stawidget);
		//创建第一部分的第二部分
		QStackedWidget* m_StackWidget = new  QStackedWidget();
		m_StackWidget->setObjectName("StackWidget");
		SSTestToolBox*  testbox = new SSTestToolBox();
		GrayDiffuse*  graywidget = new GrayDiffuse();
		OutLineImage* outline = new OutLineImage();
		m_StackWidget->addWidget(graywidget);
		m_StackWidget->addWidget(outline);
		m_StackWidget->addWidget(testbox);
		//QString & text, QString & font
		//connect(testbox, &SSTestToolBox::TextAndFontTrans, this, &MainWindow::TextAndFontChanged);
		connect(testbox,SIGNAL(TextAndFontTrans(QString &,QString &)),this,SLOT(TextAndFontChanged(QString &, QString &)));

		//connect(graywidget, &GrayDiffuse::SelectModeClicked, this, &MainWindow::newbuttonGroupClicked);
		m_StackWidget->setVisible(false);
		shwidget->addStackWidget(m_StackWidget);

		//创建第二部分
		ShrinkAnimation*  shwidget1 = new ShrinkAnimation(QObject::tr("G代码设置"),this);
		shwidget1->setMinimumHeight(FIXHEIGHT_wid);
		ExportWidget*  exportw = new ExportWidget();
		exportw->setFixedSize(350, 155);
		shwidget1->initControl(exportw);
		//connect(exportw, &ExportWidget::GcodeButtonClicked, this, &MainWindow::GcodeClicked);
		connect(exportw,SIGNAL(GcodeButtonClicked(int)),this,SLOT(GcodeClicked(int)));


		QVBoxLayout* vlayout = new QVBoxLayout();
		vlayout->setMargin(0);
		vlayout->addWidget(shwidget);
		vlayout->addWidget(shwidget1);
		vlayout->addStretch();

		ui->scrollArea->widget()->setLayout(vlayout);
	
}

void MainWindow::CretteToolBoxB()
{
	m_pToolBoxA = new SSTestToolBox(this);
}

void MainWindow::CreateToolBoxC()
{
	m_pToolBoxC = new SelectToolBox(this);
	connect(m_pToolBoxC,&SelectToolBox::LevelButtonClicked, m_newscene,&DrawScene::selectFile);
	//connect(m_pToolBoxC,SIGNAL(LevelButtonClicked(int)), m_newscene,SLOT(selectFile(int)));
	//QGraphicsItem::GraphicsItemChange change, const QVariant & value
	connect(m_newscene,&DrawScene::sceneEventNotify, m_pToolBoxC,&SelectToolBox::handleSceneEvent);
	//connect(m_newscene,SIGNAL(sceneEventNotify(QGraphicsItem::GraphicsItemChange , const QVariant & )),this,SLOT(handleSceneEvent(QGraphicsItem::GraphicsItemChange, const QVariant &)));
	//bool selected, const SelectData & data
	connect(m_newscene,&DrawScene::itemSelectedFromFile, m_pToolBoxC,&SelectToolBox::SelectFromFile);
	//connect(m_newscene,SIGNAL(itemSelectedFromFile(bool , const SelectData & )),this,SLOT(SelectFromFile(itemSelectedFromFile(bool, const SelectData &))));
	//QGraphicsItem::GraphicsItemChange change, const QVariant &value
	connect(m_pToolBoxC,&SelectToolBox::NotifySceneStateChange, m_newscene, &DrawScene::handleStateChangeEvent);
	//connect(m_pToolBoxC,SIGNAL(NotifySceneStateChange(QGraphicsItem::GraphicsItemChange , const QVariant &)), m_newscene,SLOT(handleStateChangeEvent(QGraphicsItem::GraphicsItemChange, const QVariant &)));
	connect(m_pToolBoxC,&SelectToolBox::CreateGcode,this, &MainWindow::GcodeClicked);
	//connect(m_pToolBoxC,SIGNAL(CreateGcode(int)),this,SLOT(GcodeClicked(int)));
	//connect();

	//DataStreamTest();

}

void MainWindow::DataStreamTest()
{
	QFile  file("test.test");
	file.open(QIODevice::WriteOnly);
	//file.open(QIODevice::ReadOnly);
	QDataStream out(&file);
	/*quint32 a,b,c,d;
	out>> a;*/
	//out << QString("the answer is") << (quint8)0x0D << (quint8)0x0A;
	out << (qint32)1 << (qint32)2 << (qint32)3 << (quint8)0x0D << (quint8)0x0A;
	out << (quint8)1 << (quint8)1 << (quint8)1 << (quint8)0x0D << (quint8)0x0A;
	out << (quint32)1 << (quint8)0x0D << (quint8)0x0A;
	out << (quint16)1 << (quint16)1 << (quint16)1 << (quint8)0x0D << (quint8)0x0A;
	out << (qint32)1 << (quint8)0x0D << (quint8)0x0A;
	out << (qint32)1 << (quint8)0x0D << (quint8)0x0A;
	int n = 10;
	file.close();
}

void  MainWindow::GrayDiffuseClicked(int id)
{
	//G_global = id;
}
void MainWindow::TextAndFontChanged(QString & text, QString & font)
{
	if (NULL != m_newscene)
	{
		m_newscene->setTextAndFont(text, font);
	}
}
void MainWindow::Create3dView()
{
	//创建3d的部分 
	//Qt3DExtras::Qt3DWindow* view = new Qt3DExtras::Qt3DWindow();
	//view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
	//QWidget *container = QWidget::createWindowContainer(view);
	//QHBoxLayout *hLayout = new QHBoxLayout(ui->widget_3);
	//hLayout->addWidget(container);

	//ShrinkAnimation*  shwidget = new ShrinkAnimation(QObject::tr("1111"));
	//StatusWidget* stawidget = new StatusWidget();
	//stawidget->setWindowFlag(Qt::Popup);
	//stawidget->setCusfixSizeHeight(50);
	//shwidget->setFixedHeight(FIXHEIGHT_wid);
	//shwidget->initControl(stawidget);

	//ShrinkAnimation*  shwidget1 = new ShrinkAnimation(QObject::tr("222"));
	//StatusWidget* stawidget1 = new StatusWidget();
	//stawidget1->setWindowFlag(Qt::Popup);
	//stawidget1->setCusfixSizeHeight(50);
	//shwidget1->setFixedHeight(FIXHEIGHT_wid);
	//shwidget1->initControl(stawidget1);

	//QVBoxLayout* vlayout = new QVBoxLayout();
	//vlayout->addWidget(shwidget);
	//vlayout->setSpacing(0);
	//vlayout->addWidget(shwidget1);
	////vlayout->addStretch();
	//QWidget* widgetA = new QWidget();
	//widgetA->setLayout(vlayout);
	//int n = widgetA->width();
	//int y = widgetA->height();
	////LaserWidget* widgetA = new LaserWidget();

 //   
	//hLayout->addWidget(widgetA);

	//Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
	//view->registerAspect(input);

	////root entity 
	//Qt3DCore::QEntity * rootEntity = new Qt3DCore::QEntity();
	//Qt3DRender::QCamera* cameraEntity = view->camera();
	//cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	//cameraEntity->setPosition(QVector3D(0, 0, 20.0f));
	//cameraEntity->setUpVector(QVector3D(0, 1, 0));
	//cameraEntity->setViewCenter(QVector3D(0, 0, 0));

	//Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
	//Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
	//light->setColor("white");
	//light->setIntensity(1);
	//lightEntity->addComponent(light);
	//Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
	//lightTransform->setTranslation(cameraEntity->position());
	//lightEntity->addComponent(lightTransform);

	//// For camera controls
	//Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
	//camController->setCamera(cameraEntity);

	//// Scenemodifier
	//SceneModifier *modifier = new SceneModifier(rootEntity);

	//// Set root object of the scene
	//view->setRootEntity(rootEntity);
	//////////////////////创建3d部分完结////////////////////////////////
}
void MainWindow::CreateToolbar()
{
	/*
	QToolBar *fileToolBar  = addToolBar(QObject::tr(u8"lotmax"));
	const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
	QAction *newAct = new QAction(newIcon, QObject::tr("&New"), this);
	newAct->setShortcuts(QKeySequence::New);
	newAct->setStatusTip(QObject::tr("Create a new file"));

	const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
	QAction *openAct = new QAction(openIcon, QObject::tr("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(QObject::tr("Open an existing file"));

	fileToolBar->addAction(newAct);
	fileToolBar->addAction(openAct);

	addToolBar(QObject::tr(u8"用户和常用问题"));
	addToolBar(QObject::tr(u8"论坛"));
	/*addToolBar(tr("lotmax"));
	addToolBar(tr("lotmax"));
	*/

}
void MainWindow::CreateOsgView()
{
	/**
	ShrinkAnimation*  shwidget = new ShrinkAnimation(QObject::tr("333"));
	shwidget->setFixedHeight(FIXHEIGHT_wid);


	SelectWidget* stawidget = new SelectWidget();
	//connect(stawidget, &SelectWidget::SelectModeClicked, this, &MainWindow::newbuttonGroupClicked);
	connect(stawidget,SIGNAL(SelectModeClicked(int)),this,SLOT(newbuttonGroupClicked(int)));
	//stawidget->setWindowFlag(Qt::Popup);
	//stawidget->setCusfixSizeHeight(50);
	stawidget->setFixedSize(350, 200);
	//hwidget->setFixedHeight(FIXHEIGHT_wid);
	shwidget->initControl(stawidget);

	
	
	QVBoxLayout* vlayout = new QVBoxLayout();
	//vlayout->setSpacing(2);
	vlayout->addWidget(shwidget);
	vlayout->addStretch();
    
	//ui->scrollArea->widget()->setLayout(vlayout);
	*/
}
void MainWindow::CreateGcode(QImage image,int id)
{
	//将qimage 转为 CV::mat
	cv::Mat matimage =  QImage2cvMat(image);
	QDateTime current_time = QDateTime::currentDateTime();
	//显示时间，格式为：年-月-日 时：分：秒 周几
	QString StrCurrentTime = current_time.toString("yyyy-MM-dd");
	QString name = QFileDialog::getSaveFileName(this,tr("Save gcode"), StrCurrentTime,tr("(*.gcode)"));
	if (!name.isNull())
	{
		m_LaserWork.SetFilename(name);
		id = G_global_ButtonType;
		m_LaserWork.getGcodeWithId(matimage,id);
	}
}
void MainWindow::CreateGcodeNew(QString name)
{
	QList<QGraphicsItem *> items = m_newscene->items();

	//这个就是需要用到的矩形 
	QRectF scenrect = m_newscene->sceneRect();
	//需要对qlist进行排序 
	QList<GcodeStruct> outLine;
	for (QGraphicsItem *item : items)
	{ 
		//GraphicsPixmapItem *PixmapItem =  qgraphicsitem_cast<GraphicsPixmapItem *>(item);
		GraphicsPixmapItem *PixmapItem = dynamic_cast<GraphicsPixmapItem *>(item);
		GraphicsTextItem*  textitem = dynamic_cast<GraphicsTextItem*>(item);
		if (PixmapItem)
		{
			QPointF  scenepos = PixmapItem->scenePos();
			QRectF  rect1 = PixmapItem->rect();
       		rect1.translate(scenepos);
			//是否相交判断 
			if (scenrect.intersects(rect1))
			{
				rect1 = scenrect.intersected(rect1);
				QPointF pos = PixmapItem->pos();
				/*QPointF  scenepos = PixmapItem->scenePos();*/
				//计算偏移量 
				//QPointF  offect = scenepos + QPointF(PixmapItem->rect().left(), PixmapItem->rect().top()) - QPointF(scenrect.left(), scenrect.top());
				//根据新矩形计算偏移量 
			    QPointF  offect = scenepos + QPointF(rect1.left(),rect1.top()) - QPointF(scenrect.left(), scenrect.top());
				qreal lscale = PixmapItem->scale();
				qreal lrotation = PixmapItem->rotation();
				qreal lcale = PixmapItem->getScale();
				QImage limage = PixmapItem->getFinalImage();
				SelectType  ltype = PixmapItem->SelectTypetype();
				QVariant dtype;
				if (ltype == GRAY)
				{
					dtype = PixmapItem->getGray();
				}
				else if (ltype == VECTORS)
				{
					OutLineStruct mdata;
					mdata = PixmapItem->getOutline();
					dtype.setValue(mdata);
				}
				//开始根据位置生成gcode 
				cv::Point  offecta(offect.x(), offect.y());
				cv::Mat matimage = QImage2cvMat(limage);
				GcodeStruct  vector = getGcodeWithId(matimage, ltype, name, dtype, offecta, lscale);
				vector.diandongSpeed = PixmapItem->getInSpeed();
				vector.diaokeSpeed = PixmapItem->getWorkSpeed();
				outLine.append(vector);
			}
		}
		if (textitem)
		{
			QPointF  scenepos = textitem->scenePos();
			QRectF  rect1 = textitem->rect();
			rect1.translate(scenepos);
			//是否相交判断 
			if (scenrect.intersects(rect1))
			{
				rect1 = scenrect.intersected(rect1);
				QPointF pos = textitem->pos();
				
				QPointF  offect = scenepos + QPointF(rect1.left(), rect1.top()) - QPointF(scenrect.left(), scenrect.top());
				qreal lscale = textitem->scale();
				qreal lrotation = textitem->rotation();
				//qreal lcale = textitem->getScale();
				QImage limage = textitem->getImage();

			     
				//ui->label_3->setPixmap(QPixmap::fromImage(limage));
				//SelectType  ltype = PixmapItem->SelectTypetype();
				//开始根据位置生成gcode 
				cv::Point  offecta(offect.x(), offect.y());
				cv::Mat matimage = QImage2cvMat(limage);
				GcodeStruct  vector = getGcodeWithId(matimage,3, name, 3,offecta, lscale);
				vector.diandongSpeed = textitem->getInSpeed();
				vector.diaokeSpeed = textitem->getWorkSpeed();
				outLine.append(vector);
			}
		}
	}
	if (!outLine.isEmpty())
	{
		GenerateGcode(outLine, name);

		QMessageBox msgBox(MainWindow::GetMainWindow());
		msgBox.setWindowTitle(QObject::tr(u8"提示"));
		msgBox.setText(QObject::tr(u8"Gcode生成成功"));
		msgBox.exec();
	}
}
QString MainWindow::GetSelectFilePath()
{
	QDateTime current_time = QDateTime::currentDateTime();
	//显示时间，格式为：年-月-日 时：分：秒 周几
	QString StrCurrentTime = current_time.toString("yyyy-MM-dd");
	QString name = QFileDialog::getSaveFileName(this, tr("Save gcode"), StrCurrentTime, tr("(*.gcode)"));
	return name;
}
MainWindow * MainWindow::GetMainWindow()
{
	return m_pMainWindow;
}
//打开测试文件
void MainWindow::on_openLenaJpg_triggered()
{
    srcImage = cv::imread("E:\\yuan.png");
    if(!srcImage.data)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(QObject::tr(u8"警告"));
        msgBox.setText(QObject::tr(u8"默认的测试文件不存在，可以用以下两种方式的一种：1）复制一个文件到当前目录下，并命名为lena.jpg. 2)使用自定义方式打开一个自定义文件。"));
        msgBox.exec();
    }
    else
    {
        cv::cvtColor(srcImage,srcImage,CV_BGR2RGB);
        img = QImage((const unsigned char*)(srcImage.data),srcImage.cols,srcImage.rows, srcImage.cols*srcImage.channels(), QImage::Format_RGB888);
        ui->label1->clear();
        img=  img.scaled(ui->label1->width(), ui->label1->height());
        ui->label1->setPixmap(QPixmap::fromImage(img));
        //ui->processPushButton->setEnabled(true);
        //   ui->label1->resize(ui->label1->pixmap()->size());//设置当前标签为图像大小
        // ui->label1->resize(img.width(),img.height());

        //this->setWidget(label1);
    }
}
//退出系统
void MainWindow::on_exitSystem_triggered()
{
    exit(0);
}
//打开自定义文件
void MainWindow::on_openCustomeFile_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Image"),"",tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(filename).data();
    srcImage = cv::imread(name);
    if(!srcImage.data)
    {
        QMessageBox msgBox;
        msgBox.setText(QObject::tr("未找到数据"));
        msgBox.exec();
    }
    else
    {
        cv::cvtColor(srcImage,srcImage,CV_BGR2RGB);
        img = QImage((const unsigned char*)(srcImage.data),srcImage.cols,srcImage.rows, srcImage.cols*srcImage.channels(), QImage::Format_RGB888);
        ui->label1->clear();
        img =  img.scaled(ui->label1->width(), ui->label1->height());
        ui->label1->setPixmap(QPixmap::fromImage(img));
        //ui->processPushButton->setEnabled(true);
        //   ui->label1->resize(ui->label1->pixmap()->size());//设置当前标签为图像大小
        // ui->label1->resize(img.width(),img.height());

        //this->setWidget(label1);
    }
}
//还原
void MainWindow::on_restoreFile_triggered()
{
    srcImage.copyTo(dstImage);
    img = QImage((const unsigned char*)
                 (dstImage.data),dstImage.cols,dstImage.rows,dstImage.cols*dstImage.channels(),
                 QImage::Format_RGB888);
    img=img.scaled(ui->label1->size());
    ui->label2->setPixmap(QPixmap::fromImage(img));
}
//版权信息
void MainWindow::on_copyright_triggered()
{
    QMessageBox::information(this,"版权",QObject::tr("本软件版权所有者为:胜马智教"));
}
//关于软件
void MainWindow::on_about_triggered()
{
    QMessageBox::information(this,"关于",QObject::tr("本软件当前版本为1.0"));
    return;
}
//清除图像
void MainWindow::on_Clear_triggered()
{
    //菜单：文件=>清除
    //清除标签1的内容。
    ui->label1->clear();
    //清除标签2的内容。
    ui->label2->clear();
}

//
//void MainWindow::on_GcodeImg_triggered()
//{
//	cv::resize(srcImage, srcImage, cv::Size(ui->label1->width(), ui->label1->height()), 0, 0, 3);
//    //色彩空间转换
//
//    cv::Mat dstImage2= Gcode(srcImage);
//    //显示处理结果
//    img = QImage((const unsigned char*)(dstImage2.data),dstImage2.cols,dstImage2.rows,dstImage2.step,  QImage::Format_Indexed8);
//    //  img=img.scaled(ui->label2->size());  //说明：如果经过canny变换后再调整图像大小，显示时会显示全黑，无法观看。
//    ui->label2->setPixmap(QPixmap::fromImage(img));
//}

//参数预处理
void MainWindow::on_GcodePram_triggered()
{

}
//打印前预处理
void MainWindow::on_GcodePre_triggered()
{

}


//发送数据


//读取接收到的数据
void MainWindow::Read_Data()
{
   /* QByteArray buf;
    buf = serial->readAll();
    if(!buf.isEmpty())
    {
        QString str = ui->textEdit->toPlainText();
        str+=tr(buf);
        ui->textEdit->clear();
        ui->textEdit->append(str);
    }*/
   
}

void MainWindow::RenderPic()
{
	// a 方案  不成功  不知道啷个回事  
	//QPixmap pixmap;

	////m_scene->addRect(QRectF(0, 0, 300, 300), QPen(Qt::black), QBrush(Qt::green));
	//QPainter painter(&pixmap);
	////painter.setRenderHint(QPainter::Antialiasing);
	//m_scene->render(&painter);
	//painter.end();

	//bool is = pixmap.save("scene1234566.png");
	//if (is)
	//{
	//	int a = 10;
	//}
	//ui->label_4->setPixmap(pixmap);
	//update();

	// b 方案 
	QImage image(QSize(300,300),QImage::Format_RGB32);
	QPainter painter(&image);
	m_scene->render(&painter);
	//bool b = image.save("myfile.png");
	//ui->label_4->setPicture(QPixmap::fromImage(image));
	ui->label_4->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_openButton_clicked()
{
    //if(ui->openButton->text()==tr("打开串口"))
    //{
    //    serial = new QSerialPort;
    //    //设置串口名
    //    serial->setPortName(ui->PortBox->currentText());
    //    //打开串口
    //    serial->open(QIODevice::ReadWrite);
    //    //设置波特率
    //    serial->setBaudRate(ui->BaudBox->currentText().toInt());
    //    //设置数据位数
    //    switch (ui->BitNumBox->currentIndex())
    //    {
    //    case 8: serial->setDataBits(QSerialPort::Data8); break;
    //    default: break;
    //    }
    //    //设置奇偶校验
    //    switch (ui->ParityBox->currentIndex())
    //    {
    //    case 0: serial->setParity(QSerialPort::NoParity); break;
    //    default: break;
    //    }
    //    //设置停止位
    //    switch (ui->StopBox->currentIndex())
    //    {
    //    case 1: serial->setStopBits(QSerialPort::OneStop); break;
    //    case 2: serial->setStopBits(QSerialPort::TwoStop); break;
    //    default: break;
    //    }
    //    //设置流控制
    //    serial->setFlowControl(QSerialPort::NoFlowControl);

    //    //关闭设置菜单使能
    //    ui->PortBox->setEnabled(false);
    //    ui->BaudBox->setEnabled(false);
    //    ui->BitNumBox->setEnabled(false);
    //    ui->ParityBox->setEnabled(false);
    //    ui->StopBox->setEnabled(false);
    //    ui->openButton->setText(tr("关闭串口"));
    //    ui->sendButton->setEnabled(true);

    //    //连接信号槽
    //    QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::Read_Data);
    //}
}
   /* else
    {
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();

        //恢复设置使能
        ui->PortBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
        ui->BitNumBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->StopBox->setEnabled(true);
        ui->openButton->setText(tr("打开串口"));
        ui->sendButton->setEnabled(false);
    }
}
*/

void MainWindow::on_clearButton_clicked()
{
    ui->textEdit->clear();
}

//发送数据
void MainWindow::on_sendButton_clicked()
{
    //serial->write(ui->textEdit_2->toPlainText().toLatin1());
}


void MainWindow::on_pushButton_clicked()//打印按钮
{
	//char str_1[] = "ok/n";
	//int num = 1;
	//QByteArray buf;
	//buf = serial->readAll();
 //   ifstream in("gcode.txt");
	//string filename;
	//string line;
	//char com[3];
	//do
	//{
	//	char ch1[] = "";
	//	_itoa(num, ch1, 10);
	//	int checksum = 0;
	//	int count = 0;
	//	getline(in, line);//获取一行G代码
	//QByteArray byte;
	//const char* p = line.data();
	// char cmdbuffer[MAX_CMD_SIZE]="N";//加N 即第几行代码
	// char cmdbuffer1[MAX_CMD_SIZE];
	//strcat_s(cmdbuffer, 96, ch1);
	//strcpy(cmdbuffer1, p);
	//strcat_s(cmdbuffer, 96, cmdbuffer1);
	//strcat_s(cmdbuffer, 96, "*");
	//while (cmdbuffer[count] != '*') checksum = checksum^cmdbuffer[count++];//校验和计算
	//int i = checksum;
	//cout << i << endl;
	//char ch[] = "";
	//_itoa(i, ch, 10);
	//strcat_s(cmdbuffer, 96, ch);
	//strcat_s(cmdbuffer, 96, "\r\n");
	//byte = QByteArray(cmdbuffer);
	//buf.clear();
	//serial->write(byte);//发送G代码
	//QString str;
	//do
	//{
	//	
	//	
	//	msecSleep(1300);//延迟
	//	buf = serial->readAll();
	//	str += tr(buf);
	//	QByteArray ba = str.toLatin1();
	//	char *cstr = ba.data();
	//	int n = str.length();

	//	com[0] = cstr[n - 3];
	//	com[1] = cstr[n - 2];
	//	com[2] = cstr[n - 1];
	//} while (strcmp(com, str_1) == 0);//判断是否收到ok/n
	//
	///*	str += tr(buf);
	//}
	//while (str.compare("ok") buf.isEmpty());*/
	///*QString str;
	//char com[3];
	//do
	//{
	//	

	//	
	//	msecSleep(400);
	//	buf = serial->readAll();

	//	str += tr(buf);
	//	QByteArray ba = str.toLatin1();
	//	char *cstr = ba.data();
	//	int n = str.length();
	//	
	//	com[0] = cstr[n - 3];
	//	com[1] = cstr[n -2];
	//	com[2] = cstr[n-1];
	//} while (strcmp(com,str_1)==0);*/
	//
	//
	//num++;
	//}while (1);
	
}
//非阻塞延迟函数
void  MainWindow::msecSleep(int msec)
{
	QTime _Timer = QTime::currentTime().addMSecs(msec);
	while (QTime::currentTime() < _Timer)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::textInserted(QGraphicsTextItem * item)
{

}

void MainWindow::itemSelected(QGraphicsItem * item)
{
	DiagramTextItem *textItem =
		qgraphicsitem_cast<DiagramTextItem *>(item);
}

void MainWindow::verticalAction()
{
	if (m_newscene->selectedItems().isEmpty())
		return;
	QGraphicsItem* selectedItem = m_newscene->selectedItems().first();
    
	GraphicsPixmapItem* item = dynamic_cast<GraphicsPixmapItem*>(selectedItem);//setRotataZ
	GraphicsTextItem* textitem = dynamic_cast<GraphicsTextItem*>(selectedItem);
	if (item)
	{
		item->setRotataX(180);
	}
	if (textitem)
	{
		textitem->setRotataX(180);
	}
}

void MainWindow::horicalAction()
{
	if (m_newscene->selectedItems().isEmpty())
		return;
	QGraphicsItem* selectedItem = m_newscene->selectedItems().first();

	GraphicsPixmapItem* item = dynamic_cast<GraphicsPixmapItem*>(selectedItem);//setRotataZ
	GraphicsTextItem* textitem = dynamic_cast<GraphicsTextItem*>(selectedItem);
	if (item)
	{
		item->setRotateY(180);
	}
	if (textitem)
	{
		textitem->setRotateY(180);
	}
}

void MainWindow::itemInserted(Dragramitem *item)
{
    scene->setMode(DiagramScene::Mode::MoveItem);
}

void MainWindow::newbuttonGroupClicked(int id)
{
	QStackedWidget* stackwidget = ui->scrollArea->widget()->findChild<QStackedWidget*>("StackWidget");
	if (stackwidget)
	{
		if (id >=1)
		{
			stackwidget->setVisible(true);
			stackwidget->setCurrentIndex(id -1 );
		}
		else
		{
			stackwidget->setVisible(false);
		}
	}
	G_global_ButtonType = id;
	if (nullptr != m_newscene)
	    m_newscene->selectFile(SelectType(id));
}

void MainWindow::backgroundButtonGroupClicked(QAbstractButton * button)
{
	const  QList<QAbstractButton*>   buttons = backgroundButtonGroup->buttons();
	for (QAbstractButton* myButton : buttons)
	{
		if (myButton != button)
			button->setChecked(false);
	}

	QString text = button->text();
	if (text == tr("Blue Grid"))
		scene->setBackgroundBrush(QPixmap(":/images/background1.png"));
	else if (text == tr("White Grid"))
		scene->setBackgroundBrush(QPixmap(":/images/background2.png"));
	else if (text == tr("Gray Grid"))
		scene->setBackgroundBrush(QPixmap(":/images/background3.png"));
	else 
		scene->setBackgroundBrush(QPixmap(":/images/background4.png"));

	scene->update();
	view->update();

}

void MainWindow::createToolBox()
{
	buttonGroup = new  QButtonGroup(this);
	buttonGroup->setExclusive(false);
	//connect(buttonGroup,QOverload<int>::of(&QButtonGroup::buttonClicked),this,&MainWindow::buttonGroupClicked);
	connect(buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonGroupClicked(int)));

	QGridLayout* layout = new QGridLayout;
	//layout->addWidget(createCellWidget(tr("Conditional"), Dragramitem::Conditional),0,0);
	layout->addWidget(createCellWidget(tr("Process"), Dragramitem::Step),0,1);
	//layout->addWidget(createCellWidget(tr("Input/Output"), Dragramitem::Io),1,0);

	QToolButton* textButton = new QToolButton;
	textButton->setCheckable(true);
	buttonGroup->addButton(textButton, InsertTextButton);
	textButton->setIcon(QIcon(QPixmap("")));
	textButton->setIconSize(QSize(50,50));
	QGridLayout* textLayout = new QGridLayout;
	textLayout->addWidget(textButton,0,0,Qt::AlignHCenter);
	textLayout->addWidget(new QLabel(tr("Text")),1,0,Qt::AlignCenter);

	QWidget* textWidget = new QWidget;
	textWidget->setLayout(textLayout);
	layout->addWidget(textWidget,1,1);

	layout->setRowStretch(3,10);
	layout->setColumnStretch(2,10);

	QWidget* itemWidget = new QWidget;
	itemWidget->setLayout(layout);

	backgroundButtonGroup = new QButtonGroup(this);
	//connect(backgroundButtonGroup,QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),this, &MainWindow::backgroundButtonGroupClicked);
	connect(backgroundButtonGroup,SIGNAL(buttonClicked(QAbstractButton*)),this,SLOT(backgroundButtonGroupClicked(QAbstractButton*)));

	QGridLayout * backgroundLayout = new QGridLayout;
	backgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"),":/images/background1.png"),0,0);
	backgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"),":/images/background2.png"),0,1);
	backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"),
		":/images/background3.png"), 1, 0);
	backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"),
		":/images/background4.png"), 1, 1);

    
	backgroundLayout->setRowStretch(2,10);
	backgroundLayout->setColumnStretch(2,10);

	QWidget *backgroundWidget = new QWidget;
	backgroundWidget->setLayout(backgroundLayout);

	toolBox = new QToolBox;
	toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
	toolBox->setMinimumWidth(itemWidget->sizeHint().width());
	toolBox->addItem(itemWidget, tr("Basic Flowchart Shapes"));
	toolBox->addItem(backgroundWidget, tr("Backgrounds"));


}

void MainWindow::createMenus()
{
	itemMenu = menuBar()->addMenu(tr("&Item"));
	itemMenu->addAction(deleteAction);
	itemMenu->addSeparator();
	itemMenu->addAction(toFrontAction);
	itemMenu->addAction(sendBackAction);

	QMenu*  filp = itemMenu->addMenu("Flip Model");
	QAction* vertical =  new QAction(this);
	vertical->setText("vertical");
    //connect(vertical,&QAction::triggered,this, &MainWindow::verticalAction);
	connect(vertical,SIGNAL(triggered(bool)),this,SLOT(verticalAction()));
	QAction* hortical = new QAction(this);
	hortical->setText("hortical");
	//connect(hortical, &QAction::triggered, this, &MainWindow::horicalAction);
	connect(hortical,SIGNAL(triggered(bool)),this,SLOT(horicalAction()));
	filp->addAction(vertical);
	filp->addAction(hortical);
	
	QFont font;
	font.setFamily("黑体");
	font.setPointSize(9);
	itemMenu->setFont(font);
	itemMenu->setStyleSheet("QMenu{border:none;background:rgb(69,69,73);}\
    QMenu::item:enabled{color:rgb(225,225,225);}\
    QMenu::item:!enabled{color:rgb(155,155,155);}\
    QMenu::item:enabled:selected{color:rgb(230,230,230);background:rgba(255,255,255,40);}\
    QMenu::separator{height:1px;background:rgb(100,100,100);}");
}

void MainWindow::createActions()
{
	toFrontAction = new  QAction(QIcon(":/Resourse/bringtofront.png"),
		tr("Bring to &Front"), this);
	//toFrontAction->setShortcut(tr("Ctrl+F"));
	toFrontAction->setStatusTip(tr("Bring item to front"));
	//connect(toFrontAction,&QAction::triggered,this, &MainWindow::bringToFront);
	connect(toFrontAction, SIGNAL(triggered(bool)),this,SLOT(bringToFront()));

	sendBackAction  = new QAction(QIcon(":/Resourse/sendtoback.png"), tr("Send to &Back"), this);
	//sendBackAction->setShortcut(tr("Ctrl+T"));
	sendBackAction->setStatusTip(tr("Send item to back"));
	//connect(sendBackAction, &QAction::triggered, this, &MainWindow::sendToBack);
	connect(sendBackAction,SIGNAL(triggered(bool)),this,SLOT(sendToBack(bool)));

	deleteAction  = new QAction(QIcon(":/Resourse/delete.png"), tr("&Delete"), this);
	//deleteAction->setShortcut(tr("Delete"));
	deleteAction->setStatusTip(tr("Delete item from diagram"));
	connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteItem);
	//connect(deleteAction,SIGNAL(triggered(bool)),this,SLOT(deleteItem(bool)));
}

void MainWindow::bringToFront()
{
	//if (scene->selectedItems().isEmpty())
	//	return;

	//QGraphicsItem *selectedItem = scene->selectedItems().first();
	//const QList<QGraphicsItem *>  overlapItems = selectedItem->collidingItems();

	//qreal zValue = 0;
	//for (const QGraphicsItem *item : overlapItems)
	//{
	//	if (item->zValue() >= zValue && item->type() == Dragramitem::Type)
	//	{
	//		zValue = item->zValue() + 0.1;
	//	}
	//}

	//selectedItem->setZValue(zValue);
	DbringToFront();
}

void MainWindow::sendToBack(bool del)
{
	/*if (scene->selectedItems().isEmpty())
		return;

	QGraphicsItem* selectedItem = scene->selectedItems().first();
	const  QList<QGraphicsItem *>  overlapItems = selectedItem->collidingItems();

	qreal zValue = 0;
	for (const QGraphicsItem *item : overlapItems)
	{
		if (item->zValue() <= zValue && item->type() == Dragramitem::Type)
		{
			zValue = item->zValue() - 0.1;
		}
	}

	selectedItem->setZValue(zValue);*/
	DsendToBack();
}

void MainWindow::deleteItem(bool del)
{
	/*QList<QGraphicsItem *> selectedItems = scene->selectedItems();
	for (QGraphicsItem *item : qAsConst(selectedItems))
	{
		if (item->type() == Arrow::Type)
		{
			scene->removeItem(item);
			Arrow * arrow = qgraphicsitem_cast<Arrow *>(item);
			arrow->startItem()->removeArrow(arrow);
			arrow->endItem()->removeArrow(arrow);
			delete item;
		}
	}

	selectedItems = scene->selectedItems();
	for (QGraphicsItem * item : qAsConst(selectedItems))
	{
		if (item->type() == Dragramitem::Type)
		{
			qgraphicsitem_cast<Dragramitem *>(item)->removeArrows();
		}
		scene->removeItem(item);
		delete item;
	}*/
	DdeleteItem();
	if (nullptr != m_pToolBoxC)
	     m_pToolBoxC->ResetBox();
}

void MainWindow::DsendToBack()
{
	if (m_newscene->selectedItems().isEmpty())
		return;
	QGraphicsItem* selectedItem = m_newscene->selectedItems().first();
    
	const  QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();
	qreal zValue = 0;
	for (const QGraphicsItem *item : overlapItems)
	{
		if (item->zValue() <= zValue)
		{
			zValue = item->zValue() - 0.1;
		}
	}

	selectedItem->setZValue(zValue);
}

void MainWindow::DbringToFront()
{
	if (m_newscene->selectedItems().isEmpty())
		return;

	QGraphicsItem* selectedItem = m_newscene->selectedItems().first();
	const QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

	qreal zValue = 0;
	for (const QGraphicsItem* item : overlapItems)
	{
		if (item->zValue() >= zValue)
		{
			zValue = item->zValue() + 0.1;
		}
	}

	selectedItem->setZValue(zValue);
}

void MainWindow::DdeleteItem()
{
	QList<QGraphicsItem *> selectedItems = m_newscene->selectedItems();
	for (QGraphicsItem* item : selectedItems)
	{
		m_newscene->removeItem(item);
		delete item;
		item = nullptr;
	}


}

QWidget * MainWindow::createCellWidget(const QString & text, Dragramitem::DiagramType type)
{
	Dragramitem item(type,itemMenu);
	QIcon icon(item.image());

	QToolButton*  button = new QToolButton;
	button->setIcon(icon);
	button->setIconSize(QSize(50, 50));
	button->setCheckable(true);
	buttonGroup->addButton(button,int(type));

	QGridLayout* layout = new QGridLayout;
	layout->addWidget(button,0,0,Qt::AlignHCenter);
	layout->addWidget(new QLabel(text),1,0,Qt::AlignCenter);

	QWidget * widget = new QWidget;
	widget->setLayout(layout);
	return widget;
}

QWidget * MainWindow::createBackgroundCellWidget(const QString & text, const QString & image)
{
	QToolButton* button = new QToolButton;
	button->setText(text);
	button->setIcon(QIcon(image));
	button->setIconSize(QSize(50,50));
	button->setCheckable(true);

	backgroundButtonGroup->addButton(button);
    
	QGridLayout* layout = new QGridLayout;
	layout->addWidget(button,0.0,Qt::AlignHCenter);
	layout->addWidget(new QLabel(text),1,0,Qt::AlignCenter);

	QWidget* widget = new QWidget;
	widget->setLayout(layout);

	return widget;

}

QWidget * MainWindow::createCellWidgetNew(const QString & text, const QString & pic, int id)
{

	QToolButton* button = new QToolButton;
	button->setText(text);
	button->setIcon(QIcon(pic));
	button->setIconSize(QSize(50, 50));
	button->setCheckable(true);

	newbuttonGroup->addButton(button,id);

	QGridLayout* layout = new QGridLayout;
	layout->addWidget(button, 0.0, Qt::AlignHCenter);
	layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

	QWidget* widget = new QWidget;
	widget->setLayout(layout);

	return widget;
}

void MainWindow::buttonGroupClicked(int id)
{
	const QList<QAbstractButton *>  buttons = buttonGroup->buttons();
	for (QAbstractButton * button : buttons)
	{
		if (buttonGroup->button(id) != button)
			button->setChecked(false);
	}

	if (id == InsertTextButton)
	{
		scene->setMode(DiagramScene::InsertText);
	}
	else
	{
		scene->setItemType(Dragramitem::DiagramType(id));
		scene->setMode(DiagramScene::InsertItem);
	}
}

