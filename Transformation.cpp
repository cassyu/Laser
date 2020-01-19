#include "Transformation.h"
#include <QRegExp>
#include <QIntValidator>
#include <QPainter>
#include <QListView>

Transformation::Transformation(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	CreateLayout();
}

Transformation::~Transformation()
{

}

void Transformation::CreateLayout()
{
	//还是正则表达式好用 
	QRegExp  jx("(^?180$)|(^?1[0-7]\\d$)|(^?[1-9]\\d$)|(^?[1-9]$)|^0$");
	QRegExpValidator *latitude = new QRegExpValidator(jx);
    
	QIntValidator* iv = new QIntValidator(0,125,this);
	//iv->setRange(0,125);
	ui.WidthEdit->setValidator(latitude);
	ui.heightEdit->setValidator(latitude);

	connect(ui.WidthEdit,&QLineEdit::returnPressed,this,&Transformation::widthEditingFinished);
	connect(ui.heightEdit,&QLineEdit::returnPressed,this,&Transformation::heightEditingFinished);
	connect(ui.RotateEdit,&QLineEdit::returnPressed,this, &Transformation::rotateChangedFinished);
	connect(ui.RotateSlider,&QSlider::valueChanged,this,&Transformation::rotateSliderChanged);
	connect(ui.MoveX_2,&QLineEdit::returnPressed,this,&Transformation::movexChangedFinished);
	connect(ui.MoveYEdit, &QLineEdit::returnPressed,this,&Transformation::moveyChangedFinished);
	connect(ui.MoveXSlider,&QSlider::valueChanged,this,&Transformation::moveXSliderChanged);
	connect(ui.MoveYSlider,&QSlider::valueChanged,this,&Transformation::moveYSliderChanged);
	connect(ui.ModelBox,SIGNAL(currentIndexChanged(int)),this,SLOT(flipModelChanged(int)));
	connect(ui.upButton,&QPushButton::clicked,this,&Transformation::upMove);
	connect(ui.downButton,&QPushButton::clicked,this,&Transformation::downMove);
	connect(ui.LineButton,&QPushButton::clicked,this,&Transformation::lineClicked);
   
	ui.ModelBox->setFocusPolicy(Qt::NoFocus);

	//ui.FlipModel->setVisible(false);
	//ui.ModelBox->setVisible(false);
	//ui.chicun->setVisible(false);
	ui.WidthEdit->setVisible(false);
	ui.label_6->setVisible(false);
	ui.heightEdit->setVisible(false);
	ui.RotateSlider->setVisible(false);
	ui.MoveXSlider->setVisible(false);
	ui.MoveYSlider->setVisible(false);
	
	ui.label->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.Rotate->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.MoveX->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.MoveY->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.FlipModel->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.upButton->setStyleSheet("QPushButton{border-image:url(:/Resourse/pic/6.png);}");
	ui.downButton->setStyleSheet("QPushButton{border-image:url(:/Resourse/pic/7.png);}");
	ui.LineButton->setStyleSheet("QPushButton{border-image:url(:/Resourse/pic/8.png);}");
	ui.label_2->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.label_3->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.label_4->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.RotateEdit->setStyleSheet("QLineEdit{border:none;background-color:rgb(251,248,246);}");
	ui.MoveX_2->setStyleSheet("QLineEdit{border:none;background-color:rgb(251,248,246);}");
	ui.MoveYEdit->setStyleSheet("QLineEdit{border:none;background-color:rgb(251,248,246);}");


	QListView* listview = new QListView();
	ui.ModelBox->setView(listview);
	//int num = ui.comboBox->count();

	ui.ModelBox->setStyleSheet("QComboBox{background-color:rgb(251,248,246);border:none;}QComboBox::drop-down{width:40px;background:transparent;}\
     QComboBox::down-arrow{image:url(:/Resourse/pic/arrow.png);}\
     QComboBox::down-arrow:on{image:url(:/Resourse/pic/down.png)}\
     QComboBox QAbstractItemView{border:none;outline:none;}\
     QComboBox QAbstractItemView::item{height:30px;}");
	ui.ModelBox->setCurrentIndex(-1);
	updateGeometry();
	adjustSize();

}

void Transformation::setWidth(int width)
{
	ui.WidthEdit->setText(QString::number(width));
}

void Transformation::setHeight(int height)
{
	ui.heightEdit->setText(QString::number(height));
}

void Transformation::rotateValue(int value)
{
	ui.RotateSlider->setValue(value);
	ui.RotateEdit->setText(QString::number(value));
}

void Transformation::moveXValue(int value)
{
	ui.MoveXSlider->setValue(value);
	ui.MoveX_2->setText(QString::number(value));
}

void Transformation::moveYValue(int value)
{
	ui.MoveYSlider->setValue(value);
	ui.MoveYEdit->setText(QString::number(value));
}

void Transformation::setWidthEnable(bool en)
{
	ui.WidthEdit->setEnabled(en);
}

void Transformation::setHeightEnable(bool en)
{
	ui.heightEdit->setEnabled(en);
}

void Transformation::paintEvent(QPaintEvent * event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void Transformation::widthEditingFinished()
{
	int width = ui.WidthEdit->text().toInt();
	emit  widthChanged(width);
}

void Transformation::heightEditingFinished()
{
	int height = ui.heightEdit->text().toInt();
	emit heightChanged(height);
}

void Transformation::rotateChangedFinished()
{
	int value = ui.RotateEdit->text().toInt();
	emit rotateChanged(value);
}

void Transformation::movexChangedFinished()
{
	int value = ui.MoveX_2->text().toInt();
	emit xChanged(value);
}

void Transformation::moveyChangedFinished()
{
	int value = ui.MoveYEdit->text().toInt();
	emit yChanged(value);
}

void Transformation::rotateSliderChanged(int value)
{
	emit rotateChanged(value);
}

void Transformation::moveXSliderChanged(int value)
{
	emit xChanged(value);
}

void Transformation::moveYSliderChanged(int value)
{
	emit yChanged(value);
}

void Transformation::flipModelChanged(int value)
{
	int n = value;
	emit flipChanged(value);
}

void Transformation::upMove()
{
	emit upChanged();
}

void Transformation::downMove()
{
	emit downChanged();
}

void Transformation::lineClicked()
{
	emit  lineChanged();
}
