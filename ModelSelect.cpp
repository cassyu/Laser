#include "ModelSelect.h"
#include <QListView>
#include <QStandardItemModel>
#include <QPainter>

ModelSelect::ModelSelect(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//QWidget#widget{ border - bottom:1px solid gray; }
	InitLayout();

}

void ModelSelect::InitLayout()
{
	//ui.widget->setStyleSheet("QWidget#widget{background-color:white;border-bottom:1px solid gray;}");
	ui.label->setStyleSheet("QLabel{background-color:white;border:none;}");	
	//QMargins aa = ui.comboBox->contentsMargins();
	//ui.comboBox->setContentsMargins(10,0,0,0);
	//int a = 10;
	QListView* listview = new QListView();
	ui.comboBox->setView(listview);
	int num = ui.comboBox->count();
	/*QString listStyle = "QListView{border:none;}QListWidget::item{height:30px;border:none;\
		  QListWidget::item:hover{color:rgb(29,119,204);background-color:rgb(241,248,254);}}";
	listview->setStyleSheet(listStyle);*/
	//for (int i = 0; i < num; ++i)
	//{
	//	static_cast<QStandardItemModel*>(ui.comboBox->model())->item(i)->setTextAlignment(Qt::AlignCenter);
	//}

	ui.comboBox->setStyleSheet("QComboBox{background-color:rgb(251,248,246);border:none;}QComboBox::drop-down{width:40px;background:transparent;}\
     QComboBox::down-arrow{image:url(:/Resourse/pic/arrow.png);}\
     QComboBox::down-arrow:on{image:url(:/Resourse/pic/down.png)}\
     QComboBox QAbstractItemView{border:none;outline:none;}\
     QComboBox QAbstractItemView::item{height:30px;}");
}

void ModelSelect::paintEvent(QPaintEvent * event)
{
	QStyleOption  opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

ModelSelect::~ModelSelect()
{
}
