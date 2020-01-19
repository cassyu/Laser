#pragma once
#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>

#include "dragramitem.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class Arrow : public QGraphicsLineItem
{
public:
	enum 
	{
		Type = UserType + 4
	};
	Arrow(Dragramitem* startItem, Dragramitem* endItem, QGraphicsItem* parent = 0);
	int type() const override
	{
		return Type;
	}
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void setColor(const QColor& color)
	{
		myColor = color;
	}
	Dragramitem* startItem()const
	{
		return myStartItem;
	}

	Dragramitem* endItem() const
	{
		return myEndItem;
	}

	void updatePosition();

protected:
	void paint(QPainter* painter,const QStyleOptionGraphicsItem* option,QWidget* widget = 0) override;

private:
	Dragramitem * myStartItem;
	Dragramitem*  myEndItem;
	QColor        myColor;
	QPolygonF     arrowHead;

};


#endif

