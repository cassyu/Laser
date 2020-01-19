#include "SelectWidget.h"
#include <QButtonGroup>
#include <QToolButton>
#include <QAbstractButton>
#include <QStyleOption>
#include <QPainter>

SelectWidget::SelectWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Connect();
}

SelectWidget::~SelectWidget()
{

}

void SelectWidget::Connect()
{

	buttonGroup = new QButtonGroup(this);
	buttonGroup->setExclusive(false);
	//QPalette p;
	//p.setBrush(QPalette::Button,QBrush(QPixmap(":/Resourse/black1.jpg")));
	//ui.pushButton->setIcon(QIcon(":/Resourse/black1.jpg"));
	//ui.pushButton->setIconSize(QSize(80, 72));
	buttonGroup->addButton(ui.pushButton, BALCKTYPE);
	buttonGroup->addButton(ui.pushButton_2, GARYTYPE);
	buttonGroup->addButton(ui.pushButton_3, SVGTYPE);
	buttonGroup->addButton(ui.pushButton_4, TEXTTYPE);
	//connect(buttonGroup,QOverload<int>::of(&QButtonGroup::buttonClicked),this,&SelectWidget::buttonGroupClicked);
	connect(buttonGroup,SIGNAL(buttonClicked(int)), this,SLOT(buttonGroupClicked(int)));

	/*ui.pushButton->setStyleSheet("QPushButton{background:transparent;background-image:url(:/Resourse/black.png);background-repeat:none;background-position:center;}");
	ui.pushButton_2->setStyleSheet("QPushButton{background-image:url(:/Resourse/huidu1.png);background-repeat:none;}");
	ui.pushButton_3->setStyleSheet("QPushButton{background-image:url(:/Resourse/shiliang3.jpg);background-repeat:none;}");
	ui.pushButton_4->setStyleSheet("QPushButton{background-image:url(:/Resourse/wenzi2.png);background-repeat:none;}")*/;
    
	ui.label->setStyleSheet("QLabel{border:none;background-color:white;}");
	//ui.pushButton->setStyleSheet("QPushButton{border:none;}");
	//ui.label_2->setStyleSheet("QLabel{border:none;}");
	//ui.label_3->setStyleSheet("QLabel{border:none;}");
}

void SelectWidget::paintEvent(QPaintEvent * event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}

void SelectWidget::buttonGroupClicked(int id)
{
	const QList<QAbstractButton *>  buttons = buttonGroup->buttons();
	for (QAbstractButton *button : buttons)
	{
		if (buttonGroup->button(id) == button)
		{
			emit SelectModeClicked(id);
			return;
		}
	}
}
