#pragma once
#ifndef DRAWOBJ
#define DRAWOBJ

#include <qgraphicsitem.h>
#include <QGraphicsSceneMouseEvent>
#include "drawtool.h"
#include "sizehandle.h"
#include "drawscene.h"
#include "OutLineImage.h"

//class GraphicsItem : public QGraphicsObject//public QAbstractGraphicsShapeItem/
class GraphicsItem : public QObject, public QAbstractGraphicsShapeItem
{
	Q_OBJECT  
public:
	GraphicsItem(QGraphicsItem * parent);
	enum 
	{
		Type = UserType + 1
	};
	int type() const
	{
		return Type;
	}
	SizeHandleRect::Direction  hitTest(const QPointF& point) const;
	virtual void resizeTo(SizeHandleRect::Direction dir,const QPointF& point);
	virtual QPointF origin() const
	{
		return QPointF(0,0);
	}
	virtual Qt::CursorShape  getCursor(SizeHandleRect::Direction dir);
	virtual QRectF rect()const
	{
		return QRectF(0,0,0,0);
	}

	QVariant  itemChange(GraphicsItemChange change, const QVariant &value);

	void Connect();

public slots:
	void  sizeChange();
	void  positionChange();
	void  rotationChange();


protected:
	virtual void updateGeometry();
	virtual void setState(SelectionHandleState st);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	typedef  QVector<SizeHandleRect*>  Handles;
	Handles   m_handles;

protected:

		  
};

class GraphicsRectItem : public GraphicsItem
{
public:
	GraphicsRectItem(const QRect& rect, QGraphicsItem * parent);

	QRectF boundingRect() const;
	//QPainterPath shape() const;
	void setwidth(int width);
	void setheight(int height);
	void setOrder(int order);
	void setInSpeed(int speed);
	void setWorkSpeed(int speed);
	//void setThreshold(int thresh);
	int  getWidth()const;
	int  getHeight()const;
	int  getOrder() const;
	int  getInSpeed()const;
	int  getWorkSpeed() const;
	//int  getThresh()const;

	virtual void resizeTo(SizeHandleRect::Direction dir, const QPointF & point);
	virtual QRectF  rect()const
	{
		return QRectF(-m_width / 2, -m_height / 2, m_width, m_height);
	}

	virtual void setState(SelectionHandleState st);

protected:
	void paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
	qreal m_width;
	qreal m_height;
	qreal m_order;//打印顺序
	uint  m_inSpeed;//in的速度
	uint  m_workSpeed;//工作的速度 
	qreal m_type;//类型 
	//qreal m_threshold;//阈值额改变
	//double wh_radius;//纵横比 
};

class GraphicsItemGroup : public GraphicsRectItem
{
public:
	GraphicsItemGroup(QGraphicsItemGroup * group, QGraphicsItem * parent);
	QRectF  boundingRect() const;
	//QPainterPath shape() const;
	virtual void resizeTo(SizeHandleRect::Direction dir, const QPointF & point);
protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QGraphicsItemGroup *m_group;
};

class GraphicsEllipseItem : public GraphicsRectItem
{
public:
	GraphicsEllipseItem(const QRect & rect, QGraphicsItem * parent);
	//QPainterPath shape() const;
protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

class GraphicsLineItem : public GraphicsRectItem
{
public:
	GraphicsLineItem(QGraphicsItem * parent);
	//QPainterPath shape() const;
	virtual void resizeTo(SizeHandleRect::Direction dir, const QPointF & point);
protected:

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

class GraphicsPixmapItem : public GraphicsRectItem
{
public:
	//GraphicsPixmapItem(const QRect & rect,const QString& pixmap, QGraphicsItem * parent);
	GraphicsPixmapItem(const QRect & rect,QPixmap pixmap, QPixmap Oripixmap, SelectType type,QGraphicsItem * parent);
	//QPainterPath shape() const;

	//uint        getDianMoveSpeed() const;
	//uint        getWorkSpeed()const ;
	SelectType  SelectTypetype() const;
	qreal       getScale()const ;
	QImage      getFinalImage();
	qreal       getThresh() const;
	OutLineStruct getOutline() const;
	int         getGray() const;
	void        setMatSrc(cv::Mat  src);
	void        setWidthWithRate(int width);
	void        setHeightWithREate(int height);
	//void        setTransform(QTransform  trnas);
	void        setRotataX(qreal  a);
	void        setRotateY(qreal a);
	void        setThreshBlackWhite(qreal a);
	void        setThreshVector(qreal a);
	void        setGary(qreal n);
	void        setOutLine(OutLineStruct n);
	void        setLines(vector<LINE> lines);
	//void        setDianMoveSpeed(uint speed);
	//void        setWorkSpeed(uint speed);

	virtual void resizeTo(SizeHandleRect::Direction dir, const QPointF & point);

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
	//QString   m_pixmap;
	vector<LINE>     m_Lines;
	cv::Mat          m_MatSrc;
	QPointF          m_centerPointF;
	QTransform       m_TransForm;
	QPixmap          m_pixmap;
	QPixmap          m_Orimap;
	QPixmap          m_xpixmap;//新的用于绘制的位图
	OutLineStruct    m_OutLine;
	QRect            m_Rect;
	SelectType       m_Type;
	qreal            m_thresh;
	qreal            m_graydiff;
	//uint             m_dianMoveSpeed;
	//uint             m_workSpeed;
	double           wh_radius;//纵横比 
};

class GraphicsTextItem :public GraphicsRectItem
{
public :
	GraphicsTextItem(const QRect& rect,QString text,QFont font, QGraphicsItem * parent);
	//QPainterPath shape() const;
	void setHandleState(SelectionHandleState st);
	void setTextAndFont(const QString& text,QString& font);
	virtual void setState(SelectionHandleState st);

	QImage getImage() const;
	QPixmap  getPixmap() const;
	QFont   getFont() const;
	QString getText() const;
	SelectType  SelectTypetype() const;

	void    setRotataX(qreal  a);
	void    setRotateY(qreal a);
	//void setTransform(QTransform  trans);

	virtual void resizeTo(SizeHandleRect::Direction dir, const QPointF & point);

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
	QImage           m_Image;
	QTransform       m_TransForm;
	QPointF         m_centerPointF;
	QString         m_Text;
	QFont           m_Font;
	QRect           m_Rect;
	double          wh_radius;//纵横比 
	int             pointsize;
};


#endif 

