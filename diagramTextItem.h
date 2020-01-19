﻿#pragma once
#ifndef DIAGRAMTEXTITEM_H
#define DIAGRAMTEXTITEM_H

#include <QGraphicsTextItem>
#include <QPen>
QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class DiagramTextItem : public QGraphicsTextItem
{
	Q_OBJECT
public:
	enum
	{
		Type = UserType + 3
	};
public:
	DiagramTextItem(QGraphicsItem* parent = 0);
	int type() const override
	{
		return Type;
	}

signals:
	void lostFocus(DiagramTextItem* item);
signals:
	void selectedChange(QGraphicsItem * item);

protected:
	QVariant itemChange(GraphicsItemChange change,const QVariant &value) override;
	void     focusOutEvent(QFocusEvent * event)override;
	void     mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)override;
};

#endif 

