#include "diagramScene.h"
#include "Arrow.h"

#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>

DiagramScene::DiagramScene(QMenu * itemMenu, QObject * parent)
	: QGraphicsScene(parent)
{
	if (nullptr != itemMenu)
	{
		myItemMenu = itemMenu;
	}
	myMode = MoveItem;
	myItemType = Dragramitem::Step;
	line = 0;
	textItem = 0;
	myItemColor = Qt::white;
	myTextColor = Qt::black;
	myLineColor = Qt::black;

}

void DiagramScene::setLineColor(const QColor & color)
{
	myLineColor = color;
	if (isItemChange(Arrow::Type))
	{
		Arrow* item = qgraphicsitem_cast<Arrow*>(selectedItems().first());
		item->setColor(myLineColor);
		update();
	}
}

void DiagramScene::setTextColor(const QColor & color)
{
	myTextColor = color;
	if (isItemChange(DiagramTextItem::Type))
	{
		DiagramTextItem* item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
		item->setDefaultTextColor(myTextColor);
	}
}

void DiagramScene::setItemColor(const QColor & color)
{
	myItemColor = color;
	if (isItemChange(DiagramTextItem::Type))
	{
		Dragramitem* item = qgraphicsitem_cast<Dragramitem *>(selectedItems().first());
		item->setBrush(myItemColor);
	}
}

void DiagramScene::setFont(const QFont & font)
{
	myFont = font;

	if (isItemChange(DiagramTextItem::Type))
	{
		QGraphicsTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
		if (item)
		{
			item->setFont(myFont);
		}
	}
}

void DiagramScene::setItemType(Dragramitem::DiagramType type)
{
	myItemType = type;
}

void DiagramScene::editorLostFocus(DiagramTextItem * item)
{
	QTextCursor cursor = item->textCursor();
	cursor.clearSelection();
	item->setTextCursor(cursor);

	if (item->toPlainText().isEmpty())
	{
		removeItem(item);
		item->deleteLater();
	}
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton)
	{
		return;
	}
	Dragramitem *item;
	switch (myMode)
	{
	case InsertItem:
		item = new Dragramitem(myItemType, myItemMenu);
		item->setBrush(myItemColor);
		addItem(item);
		item->setPos(mouseEvent->scenePos());
		emit itemInserted(item);
		break;
	case InsertLine:
		line = new  QGraphicsLineItem(QLineF(mouseEvent->scenePos(),mouseEvent->scenePos()));
		line->setPen(QPen(myLineColor,2));
		addItem(line);
		break;
	case InsertText:
		textItem = new DiagramTextItem();
		textItem->setFont(myFont);
		textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
		textItem->setZValue(1000);
		/*	connect(textItem, &DiagramTextItem::lostFocus,
				this, &DiagramScene::editorLostFocus);*/
		/*connect(textItem, &DiagramTextItem::selectedChange,
			this, &DiagramScene::itemSelected);*/
		addItem(textItem);
		textItem->setDefaultTextColor(myTextColor);
		textItem->setPos(mouseEvent->scenePos());
		emit textInserted(textItem);
		break;
	default :
		break;
	}
	QGraphicsScene::mousePressEvent(mouseEvent);
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	if (myMode == InsertLine && line != 0)
	{
		QLineF newLine(line->line().p1(),mouseEvent->scenePos());
		line->setLine(newLine);
	}
	else if (myMode == MoveItem)
	{
		QGraphicsScene::mouseMoveEvent(mouseEvent);
	}
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	if (line != 0 && myMode == InsertLine)
	{
		QList<QGraphicsItem*>   startItems = items(line->line().p1());
		if (startItems.count() && startItems.first() == line)
		{
			startItems.removeFirst();
		}

		QList<QGraphicsItem*>  endItems = items(line->line().p2());
		if (endItems.count() && endItems.first() == line)
			endItems.removeFirst();

		removeItem(line);
		delete line;
		if (startItems.count() > 0 && endItems.count() > 0 &&
			startItems.first()->type() == Dragramitem::Type &&
			endItems.first()->type() == Dragramitem::Type &&
			startItems.first() != endItems.first())
		{
			Dragramitem* startItem = qgraphicsitem_cast<Dragramitem *>(startItems.first());
			Dragramitem *endItem = qgraphicsitem_cast<Dragramitem *>(endItems.first());
			Arrow* arrow = new Arrow(startItem, endItem);
			arrow->setColor(myLineColor);
			startItem->addArrow(arrow);
			endItem->addArrow(arrow);
			arrow->setZValue(-1000);
			addItem(arrow);
			arrow->updatePosition();

		}
	}

	line = 0;
	QGraphicsScene::mouseReleaseEvent(mouseEvent);

}

bool DiagramScene::isItemChange(int type) const
{
	const QList<QGraphicsItem*>  items = selectedItems();
	const auto cb = [type](const QGraphicsItem* item) {return item->type() == type; };
	return std::find_if(items.begin(), items.end(), cb) != items.end();
}

void DiagramScene::setMode(Mode mode)
{
	myMode = mode;
}
