#include "LfontBox.h"
#include <QPainter>
#include <QDebug>

LfontBox::LfontBox(QWidget *parent)
	: QFontComboBox(parent)
{
	dumpStructture(this,0);
}

LfontBox::~LfontBox()
{
}

void LfontBox::dumpStructture(QObject * obj, int spacecount)
{
	qDebug() << QString("%1%2 : %3")
		.arg("", spacecount)
		.arg(obj->metaObject()->className())
		.arg(obj->objectName());

	QObjectList list = obj->children();
	foreach (QObject * child, list)
	{
		dumpStructture(child, spacecount+4);
	}
}

void LfontBox::paintEvent(QPaintEvent * e)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void LfontBox::wheelEvent(QWheelEvent * e)
{

}
