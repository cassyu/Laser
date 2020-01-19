#include "GrayDiffuse.h"
#include <QButtonGroup>
#include "LaserGcodeGenerate.h"
#include "SGcodeGenerate.h"
//#include <QStyleOption>
#include <QStyleOption>
#include <QPainter>

GrayDiffuse::GrayDiffuse(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Connect();
}

GrayDiffuse::~GrayDiffuse()
{
}

void GrayDiffuse::Connect()
{

	buttonGroup = new QButtonGroup(this);
	buttonGroup->setExclusive(true);
	ui.radioButton->setChecked(true);
	//QPalette p;
	//p.setBrush(QPalette::Button,QBrush(QPixmap(":/Resourse/black1.jpg")));
	//ui.pushButton->setIcon(QIcon(":/Resourse/black1.jpg"));
	//ui.pushButton->setIconSize(QSize(80, 72));
	buttonGroup->addButton(ui.radioButton, BUTTON::FloydSteinburg);
	buttonGroup->addButton(ui.radioButton_2, BUTTON::Burkers);
	buttonGroup->addButton(ui.radioButton_3, BUTTON::Sierra);
	buttonGroup->addButton(ui.radioButton_4, BUTTON::Stucki);
	buttonGroup->addButton(ui.radioButton_5, BUTTON::Jarvis);
	buttonGroup->addButton(ui.radioButton_6, BUTTON::Stevenson);
	//connect(buttonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &GrayDiffuse::buttonGroupClicked);
	connect(buttonGroup, SIGNAL(buttonClicked(int)),this, SLOT(buttonGroupClicked(int)));
	connect(ui.pushButton,SIGNAL(clicked(bool)),this,SLOT(buttonClicked(bool)));

	ui.label->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.radioButton->setStyleSheet("QRadioButton{background-color:white;}");
	ui.radioButton_2->setStyleSheet("QRadioButton{background-color:white;}");
	ui.radioButton_3->setStyleSheet("QRadioButton{background-color:white;}");
	ui.radioButton_4->setStyleSheet("QRadioButton{background-color:white;}");
	ui.radioButton_5->setStyleSheet("QRadioButton{background-color:white;}");
	ui.radioButton_6->setStyleSheet("QRadioButton{background-color:white;}");
	ui.pushButton->setStyleSheet("QPushButton{border-image:url(:/Resourse/pic/3.png);}");
}

void GrayDiffuse::paintEvent(QPaintEvent * event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void GrayDiffuse::SetGrayDiff(int id)
{
	if (nullptr != buttonGroup)
	{
		QRadioButton* rButton = dynamic_cast<QRadioButton*>(buttonGroup->button(id));
		if (rButton)
		{
			rButton->setCheckable(true);
		}
	}
}

void GrayDiffuse::buttonClicked(bool checked)
{
	emit  GrayDiffuseChanged(errorDiffusion);
}

void GrayDiffuse::buttonGroupClicked(int id)
{
	const QList<QAbstractButton *>  buttons = buttonGroup->buttons();
	for (QAbstractButton *button : buttons)
	{
		if (buttonGroup->button(id) == button)
		{
			//emit SelectModeClicked(id);
			G_global_errorDiffusion = id;
			errorDiffusion = id;
			return;
		}
	}
}
