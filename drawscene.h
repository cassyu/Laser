#pragma once
#ifndef DRAWSCENE
#define DRAWSCENE

#include <QGraphicsScene>
#include "sizehandle.h"
#include "LaserGcodeGenerate.h"
#include "Transformation.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
class QMenu;
QT_END_NAMESPACE

enum  SelectType
{
	BLACKANDWHITE,
	GRAY,
	VECTORS,
	TEXT,
	NONE_SEL,
};

enum   ParamType
{
	WIDTH_VALUE,
	HEIGHT_VALUE,
	ROATET_VALUE,
	MOVEX_VALUE,
	MOVEY_VALUE,
};

struct SceneEvent
{
	int type;
};


class DrawScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit DrawScene(QObject *parent = 0);
	void mouseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void setParamValue(int value, ParamType  type);
	Qt::CursorShape getCursor(SizeHandleRect::Direction dir);
	void setContextMenu(QMenu *menu);
	QMenu* getContextMenu();
	void TestEvent();
	void itemsChanged(QGraphicsItem::GraphicsItemChange change, const QVariant &value);

signals:
	void itemSelectedFromFile(bool selected, const SelectData& data = SelectData());
	//void sceneEventNotify(SceneEvent* event);
	void sceneEventNotify(QGraphicsItem::GraphicsItemChange change, const QVariant &value);


public slots:
	void setTextAndFont(QString& text, QString &font);
	void handleStateChangeEvent(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
	void selectFile(int type);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
	bool   m_bPressed;
	QMenu *m_ContextMenu;

	LaserGcodeGenerate m_LaserWork;

};

#endif 

