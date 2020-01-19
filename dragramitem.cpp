#include "dragramitem.h"
#include "Arrow.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
//#include <Math.h>


Dragramitem::Dragramitem(DiagramType diagramType, QMenu * contextMenu, QGraphicsItem * parent)
	:QGraphicsPolygonItem(parent)
{
	myDiagramType = diagramType;
	myContextMenu = contextMenu;

	QPainterPath path;
	switch (myDiagramType)
	{
	case StartEnd:
		path.moveTo(200,50);
		path.arcTo(150,0,50,50,0,90);
		path.arcTo(50,0,50,50,90,90);
		path.arcTo(50,50,50,50,180,90);
		path.arcTo(150,50,50,50,270,90);
		path.lineTo(200,25);
		myPolygon = path.toFillPolygon();
		break;
	case Conditional:
		myPolygon << QPointF(-100, 0) << QPointF(0, 100) << QPointF(100, 0) << QPointF(0, -100) << QPointF(-100,0);
		break;
	case Step:
		myPolygon << QPointF(-100, -100) << QPointF(100, -100)
			<< QPointF(100, 100) << QPointF(-100, 100)
			<< QPointF(-100, -100);
		break;
	default:
		myPolygon << QPointF(-120, -80) << QPointF(-70, 80) << QPointF(120, 80) << QPointF(70, -80) << QPointF(-120, -80);
		break;
	}
	setPolygon(myPolygon);
	setFlag(QGraphicsItem::ItemIsMovable,true);
	setFlag(QGraphicsItem::ItemIsSelectable,true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
}

void Dragramitem::removeArrow(Arrow * arrow)
{
	int index = arrows.indexOf(arrow);

	if (index != -1)
		arrows.removeAt(index);
}

void Dragramitem::removeArrows()
{
	for (Arrow* arrow : arrows)
	{
		arrow->startItem()->removeArrow(arrow);
		arrow->endItem()->removeArrow(arrow);
		scene()->removeItem(arrow);
		delete arrow;
	}
}

void Dragramitem::addArrow(Arrow * arrow)
{
	arrows.append(arrow);
}

QPixmap Dragramitem::image() const
{
	QPixmap  pixmap(250,250);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	painter.setPen(QPen(Qt::black,8));

	painter.translate(125,125);
	painter.drawPolyline(myPolygon);

	return  pixmap;
}

void Dragramitem::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
	scene()->clearSelection();
	setSelected(true);
	myContextMenu->exec(event->screenPos());
}

QVariant Dragramitem::itemChange(GraphicsItemChange change, const QVariant & value)
{
	if (change == QGraphicsItem::ItemPositionChange)
	{
		for (Arrow* arrow : arrows)
		{
			arrow->updatePosition();
		}
	}

	return value;
}

void Dragramitem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	//if (event->button() == Qt::LeftButton)
	//{
	//	if (event->modifiers() == Qt::ShiftModifier)
	//	{
	//		setSelected(true);
	//	}
	//	else if (event->modifiers() == Qt::AltModifier)
	//	{
	//		double radius = boundingRect().width() / 2.0;
	//		QPointF topLeft = boundingRect().topLeft();
	//		m_centerPointF = QPointF(topLeft.x() + pos().x() + radius, topLeft.y() + pos().y() + radius);
	//		QPointF pos = event->scenePos();
	//		double dist = sqrt(pow(m_centerPointF.x() - pos.x(), 2) + pow(m_centerPointF.y() - pos.y(), 2));
	//		//也就是说大于20%边界的部分才会缩放
	//		if (dist / radius > 0.8)
	//		{
	//			m_bResizing = true;
	//		}
	//		else
	//		{
	//			m_bResizing = false;
	//		}

	//	}
	//	else
	//	{
	//		QGraphicsItem::mousePressEvent(event);
	//		event->accept();
	//	}
	//}
	//else if (event->button() == Qt::RightButton)
	//{
	//	event->ignore();
	//}
}

void Dragramitem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	//if (event->modifiers() == Qt::AltModifier && m_bResizing)
	//{
	//	QPointF  pos = event->scenePos();
	//	double dist = sqrt(pow(m_centerPointF.x() - pos.x(), 2) + pow(m_centerPointF.y() - pos.y(), 2));
	//	//需要将继承关系改变   才能用setrect();
	//	//setRect(m_centerPointF.x() - this->pos().x() - dist, m_centerPointF.y() - this->pos().y() - dist, dist * 2, dist * 2);
	//}
	//else if (event->modifiers() != Qt::AltModifier)
	//{
	//	QGraphicsItem::mouseMoveEvent(event);
	//}
}

void Dragramitem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	if (event->modifiers() == Qt::AltModifier && m_bResizing)
	{
		m_bResizing = false;
	}
	else
	{
		QGraphicsItem::mouseReleaseEvent(event);
	}
}
