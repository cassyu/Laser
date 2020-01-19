#include "TextWidget.h"
#include <QPainter>

TextWidget::TextWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Init();
}

TextWidget::~TextWidget()
{
}

void TextWidget::Init()
{
	/*ui.label_3->setVisible(false);
	ui.lineEdit->setVisible(false);

	ui.label_4->setVisible(false);
	ui.lineEdit_2->setVisible(false);

	ui.label_5->setVisible(false);
	ui.lineEdit_3->setVisible(false);*/


	//connect(ui.pushButton, &QPushButton::clicked, this, &TextWidget::TextChanged);
	connect(ui.pushButton,SIGNAL(clicked(bool)),this,SLOT(TextChanged()));
	ui.label->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.label_3->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.label_2->setStyleSheet("QLabel{background-color:white;border:none;}");
	ui.pushButton->setStyleSheet("QPushButton{border-image:url(:/Resourse/pic/3.png);}");

	//ui.fontComboBox->setStyleSheet("QFontComboBox{background-color:rgb(251,248,246);border:none;}QComboBox::drop-down{width:40px;background:transparent;}\
	// QFontComboBox::down-arrow{image:url(:/Resourse/pic/arrow.png);}\
	// QFontComboBox::down-arrow:on{image:url(:/Resourse/pic/down.png)}\
	// QFontComboBox QAbstractItemView{border:none;outline:none;}\
	// QFontComboBox QAbstractItemView::item{height:30px;}");

	ui.fontComboBox->setStyleSheet("QComboBox{background-color:rgb(251,248,246);border:none;}QComboBox::drop-down{width:40px;background:transparent;}\
	 QComboBox::down-arrow{image:url(:/Resourse/pic/arrow.png);}\
	 QComboBox::down-arrow:on{image:url(:/Resourse/pic/down.png)}\
	 QComboBox QAbstractItemView{border:none;outline:none;}\
	 QComboBox QAbstractItemView::item{height:30px;}");
	 /*ui.fontComboBox->setStyleSheet("LfontBox{background-color:rgb(251,248,246);border:none;}QComboBox::drop-down{width:40px;background:transparent;}\
	  LfontBox:down-arrow{image:url(:/Resourse/pic/arrow.png);}\
	  LfontBox:down-arrow:on{image:url(:/Resourse/pic/down.png)}\
	  LfontBox QAbstractItemView{border:none;outline:none;}\
	  LfontBox QAbstractItemView::item{height:30px;}");*/
}

void TextWidget::paintEvent(QPaintEvent * event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void TextWidget::TextChanged()
{
	QFont font = ui.fontComboBox->currentFont();
	QString  text = ui.textEdit->toPlainText();
	QString name = font.family();
	emit CurrentTextAndFont(text,name);
}
