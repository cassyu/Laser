#pragma once
#ifndef DRAWTOOL
#define DRAWTOOL

#include <QGraphicsScene>
#include <QList>
#include <QCursor>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
class QAbstractGraphicsShapeItem;
class QGraphicsObject;
QT_END_NAMESPACE

class DrawScene;
class GraphicsItem;

//需要绘制的形状 
enum DrawShape
{
	selection,
	rotation,
	line,
	rectangle,
	roundrect,
	ellipse,
	poly
};

class DrawTool
{
public:
	DrawTool(DrawShape shape);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent * event, DrawScene * scene);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event, DrawScene * scene);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event, DrawScene * scene);

	DrawShape m_drawShape;

	//在整个场景中只会有一份的数据 
	static DrawTool* findTool(DrawShape drawShape);
	static QList<DrawTool*>   c_tools;
	static QPointF  c_down;
	static quint32  c_nDownFlags;
	static QPointF  c_last;
	static DrawShape c_drawShape;
	//
	static QGraphicsItem*  c_Item;
};

class SelectTool : public DrawTool
{
public:
	SelectTool();
	virtual void mousePressEvent(QGraphicsSceneMouseEvent * event, DrawScene * scene);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event, DrawScene * scene);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event, DrawScene * scene);

	QSizeF  m_lastSize;
	QGraphicsPathItem* dashRect;
	QGraphicsLineItem* dashLine;

	bool m_hoverSizer;

};

class RotationTool : public DrawTool
{
public:
	RotationTool();
	virtual void mousePressEvent(QGraphicsSceneMouseEvent * event, DrawScene * scene);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event, DrawScene * scene);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event, DrawScene * scene);

	qreal lastAngle;
	QGraphicsPathItem* dashRect;

};


class RectTool : public DrawTool
{
public:
	RectTool(DrawShape drawShape);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent * event, DrawScene * scene);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event, DrawScene * scene);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event, DrawScene * scene);

	QAbstractGraphicsShapeItem * item;
	//QGraphicsObject*   item;

};
#endif 

