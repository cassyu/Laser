#include "OutLineImage.h"
#include "LaserGcodeGenerate.h"
#include <QButtonGroup>
#include "SGcodeGenerate.h"
#include <QStyleOption>
#include <QPainter>

OutLineImage::OutLineImage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Init();
}

OutLineImage::~OutLineImage()
{
}

void OutLineImage::Init()
{
	buttonGroup = new QButtonGroup(this);
	buttonGroup->setExclusive(true);

	buttonGroup_A =  new QButtonGroup(this);
	buttonGroup_A->setExclusive(true);
	//QPalette p;
	//p.setBrush(QPalette::Button,QBrush(QPixmap(":/Resourse/black1.jpg")));
	//ui.pushButton->setIcon(QIcon(":/Resourse/black1.jpg"));
	//ui.pushButton->setIconSize(QSize(80, 72));
	buttonGroup->addButton(ui.radioButton,0);
	buttonGroup->addButton(ui.radioButton_2,1);
	buttonGroup->addButton(ui.radioButton_3,2);

	ui.radioButton->setChecked(true);

	//connect(buttonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &OutLineImage::buttonGroupClicked);
	connect(buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonGroupClicked(int)));

	buttonGroup_A->addButton(ui.radioButton_4,0);
	buttonGroup_A->addButton(ui.radioButton_5,1);
	buttonGroup_A->addButton(ui.radioButton_6,2);

	ui.radioButton_4->setChecked(true);

	//connect(buttonGroup_A, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &OutLineImage::buttonGroupClicked_A);
	connect(buttonGroup_A,SIGNAL(buttonClicked(int)),this,SLOT(buttonGroupClicked_A(int)));
	//connect(ui.pushButton,&QPushButton::clicked,this, &OutLineImage::arithmeticChanged);
	connect(ui.pushButton,SIGNAL(clicked(bool)),this,SLOT(arithmeticChanged()));

	ui.label->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.label_2->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.radioButton->setStyleSheet("QRadioButton{background-color:white;}");
	ui.radioButton_2->setStyleSheet("QRadioButton{background-color:white;}");
	ui.radioButton_3->setStyleSheet("QRadioButton{background-color:white;}");
	ui.radioButton_4->setStyleSheet("QRadioButton{background-color:white;}");
	ui.radioButton_5->setStyleSheet("QRadioButton{background-color:white;}");
	ui.radioButton_6->setStyleSheet("QRadioButton{background-color:white;}");
	ui.pushButton->setStyleSheet("QPushButton{border-image:url(:/Resourse/pic/3.png);}");
}

void OutLineImage::setOutLineState(OutLineStruct& data)
{
	if (nullptr != buttonGroup)
	{
		QRadioButton* rButton = dynamic_cast<QRadioButton*>(buttonGroup->button(data.operar));
		if (rButton)
		{
			rButton->setCheckable(true);
		}
	}

	if (nullptr != buttonGroup_A)
	{
		QRadioButton* rButton = dynamic_cast<QRadioButton*>(buttonGroup_A->button(data.border));
		if (rButton)
		{
			rButton->setCheckable(true);
		}
	}
}

void OutLineImage::paintEvent(QPaintEvent * event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void OutLineImage::arithmeticChanged()
{
	OutLineStruct data;
	data.operar = getContours;
	data.border = ContoursDetect;
	emit  NotifyArithmeticChange(data);
}

void OutLineImage::buttonGroupClicked_A(int id)
{
	const QList<QAbstractButton *>  buttons = buttonGroup->buttons();
	for (QAbstractButton *button : buttons)
	{
		if (buttonGroup_A->button(id) == button)
		{
			G_global_ContourDetect = id;
			ContoursDetect = id;
			return;
		}
	}
}

void OutLineImage::buttonGroupClicked(int id)
{
	const QList<QAbstractButton *>  buttons = buttonGroup->buttons();
	for (QAbstractButton *button : buttons)
	{
		if (buttonGroup->button(id) == button)
		{
			G_global_GetContours = id;
			getContours = id;
			return;
		}
	}
}
