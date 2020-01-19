#include "drawobj.h"
#include <QPainter>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QCursor>
#include <QDebug>
#include <QGraphicsEffect>
#include <QTextOption>
#include "drawscene.h"
#include <QMenu>
//#include <QtMath>
#include <QPainter>
#include "QtOpencvConvert.h"
#include "SGcodeGenerate.h"


static QPainterPath  qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen)
{
	const qreal penWidthZero = qreal(0.00000001);
	if (path == QPainterPath() || pen == Qt::NoPen)
		return path;


	QPainterPathStroker ps;
	ps.setCapStyle(pen.capStyle());

	if (pen.widthF() <= 0.0)
		ps.setWidth(penWidthZero);
	else
		ps.setWidth(pen.widthF());

	ps.setJoinStyle(pen.joinStyle());
	ps.setMiterLimit(pen.miterLimit());

	QPainterPath  p = ps.createStroke(path);
	p.addPath(path);
	return p;

}

GraphicsItem::GraphicsItem(QGraphicsItem *parent)
:QAbstractGraphicsShapeItem(parent)
//:QGraphicsObject(parent)
{
	// QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
	// effect->setBlurRadius(8);
	// setGraphicsEffect(effect);
	Connect();
}

void GraphicsItem::Connect()
{
	/*connect(this, &GraphicsItem::rotationChanged,this, &GraphicsItem::rotationChanged);
	connect(this, &GraphicsItem::widthChanged, this, &GraphicsItem::sizeChange);
	connect(this, &GraphicsItem::xChanged, this, &GraphicsItem::positionChange);*/
}

void GraphicsItem::sizeChange()
{

}

void GraphicsItem::positionChange()
{

}

void GraphicsItem::rotationChange()
{

}

void GraphicsItem::updateGeometry()
{
	const QRectF &geom = this->boundingRect();

	const int w = SELECTION_HANDLE_SIZE;
	const int h = SELECTION_HANDLE_SIZE;

	const Handles::iterator hend = m_handles.end();
	for (Handles::iterator it = m_handles.begin(); it != hend; ++it) {
		SizeHandleRect *hndl = *it;;
		switch (hndl->dir()) {
		case SizeHandleRect::LeftTop:
			hndl->move(geom.x() - w / 2, geom.y() - h / 2);
			break;
		case SizeHandleRect::Top:
			hndl->move(geom.x() + geom.width() / 2 - w / 2, geom.y() - h / 2);
			break;
		case SizeHandleRect::RightTop:
			hndl->move(geom.x() + geom.width() - w / 2, geom.y() - h / 2);
			break;
		case SizeHandleRect::Right:
			hndl->move(geom.x() + geom.width() - w / 2, geom.y() + geom.height() / 2 - h / 2);
			break;
		case SizeHandleRect::RightBottom:
			hndl->move(geom.x() + geom.width() - w / 2, geom.y() + geom.height() - h / 2);
			break;
		case SizeHandleRect::Bottom:
			hndl->move(geom.x() + geom.width() / 2 - w / 2, geom.y() + geom.height() - h / 2);
			break;
		case SizeHandleRect::LeftBottom:
			hndl->move(geom.x() - w / 2, geom.y() + geom.height() - h / 2);
			break;
		case SizeHandleRect::Left:
			hndl->move(geom.x() - w / 2, geom.y() + geom.height() / 2 - h / 2);
			break;
		/*case SizeHandleRect::Center:
			hndl->move(geom.center().x() - w / 2, geom.center().y() - h / 2);
			break;*/
		case SizeHandleRect::Rotate:
			hndl->move(geom.x() + geom.width() / 2 - w / 2, geom.y() - h / 2 - 30);
			break;
		default:
			break;
		}
	}
}

void GraphicsItem::setState(SelectionHandleState st)
{
	const Handles::iterator hend = m_handles.end();
	for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
		(*it)->setState(st);
}

SizeHandleRect::Direction GraphicsItem::hitTest(const QPointF &point) const
{
	const Handles::const_iterator hend = m_handles.end();
	for (Handles::const_iterator it = m_handles.begin(); it != hend; ++it)
	{
		if ((*it)->hitTest(point))
		{
			return (*it)->dir();
		}
	}

	return SizeHandleRect::None;
}

Qt::CursorShape GraphicsItem::getCursor(SizeHandleRect::Direction dir)
{
	switch (dir)
	{
	case SizeHandleRect::Right:
		return Qt::SizeHorCursor;
	case SizeHandleRect::RightTop:
		return Qt::SizeBDiagCursor;
	case SizeHandleRect::RightBottom:
		return Qt::SizeFDiagCursor;
	case SizeHandleRect::LeftBottom:
		return Qt::SizeBDiagCursor;
	case SizeHandleRect::Bottom:
		return Qt::SizeVerCursor;
	case SizeHandleRect::LeftTop:
		return Qt::SizeFDiagCursor;
	case SizeHandleRect::Left:
		return Qt::SizeHorCursor;
	case SizeHandleRect::Top:
		return Qt::SizeVerCursor;
	case SizeHandleRect::Rotate:
		return  Qt::WaitCursor;//转动十字
	default:
		break;
	}
	return Qt::ArrowCursor;
}

void GraphicsItem::resizeTo(SizeHandleRect::Direction dir, const QPointF &point)
{

}

void GraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	scene()->clearSelection();
	setSelected(true);
	DrawScene* sc = static_cast<DrawScene*>(scene());
	if (sc)
	{
		sc->getContextMenu()->exec(event->screenPos());;
	}
}

QVariant GraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	if (change == QGraphicsItem::ItemSelectedHasChanged) 
	{
		//qDebug() << " Item Selected : " << value.toString() << "item ";;
		//sqDebug() << "item " << this;
		setState(value.toBool() ? SelectionHandleActive : SelectionHandleOff);
		DrawScene*  temp_scene = dynamic_cast<DrawScene*>(scene());
		if (temp_scene)
		{
			temp_scene->itemsChanged(change, value);
		}
	}//旋转改变 
	else if (change == QGraphicsItem::ItemRotationHasChanged) 
	{
		//qDebug() << "Item Rotation Changed:" << value.toDouble();
		DrawScene*  temp_scene = dynamic_cast<DrawScene*>(scene());
		if (temp_scene)
		{
			temp_scene->itemsChanged(change, value);
		}
	}
	else if (change == QGraphicsItem::ItemTransformOriginPointHasChanged)
	{
		//qDebug() << "ItemTransformOriginPointHasChanged:" << value.toPointF();
		DrawScene*  temp_scene = dynamic_cast<DrawScene*>(scene());
		if (temp_scene)
		{
			temp_scene->itemsChanged(change, value);
		}
	}
	else if (change == QGraphicsItem::ItemPositionChange)
	{
		qDebug() << "ItemTransformOriginPointHasChanged:" << value.toPointF();
		DrawScene*  temp_scene = dynamic_cast<DrawScene*>(scene());
		if (temp_scene)
		{
			temp_scene->itemsChanged(change, value);
		}
	}//位置改变 
	else if (change == QGraphicsItem::ItemPositionHasChanged)
	{
		//qDebug() << "ItemTransformOriginPointHasChanged:" << value.toPointF();
		DrawScene*  temp_scene = dynamic_cast<DrawScene*>(scene());
		if (temp_scene)
		{
			temp_scene->itemsChanged(change, value);
		}
	}
	else if (QGraphicsItem::ItemScaleHasChanged == change)
	{
		//qDebug() << "ItemTransformOriginPointHasChanged:" << value.toPointF();
		DrawScene*  temp_scene = dynamic_cast<DrawScene*>(scene());
		if (temp_scene)
		{
			temp_scene->itemsChanged(change, value);
		}
	}
	return value;
}

GraphicsRectItem::GraphicsRectItem(const QRect & rect, QGraphicsItem *parent)
	:GraphicsItem(parent)
	,m_width(rect.width())
	,m_height(rect.height())
{
	m_handles.reserve(SizeHandleRect::None);
	for (int i = SizeHandleRect::LeftTop; i <= SizeHandleRect::Rotate; ++i)
	{
		SizeHandleRect* shr = new SizeHandleRect(this, static_cast<SizeHandleRect::Direction>(i),this);
		m_handles.push_back(shr);
	}
	updateGeometry();
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

	m_inSpeed = 50;
	m_workSpeed = 50;

	setAcceptHoverEvents(true);

}

QRectF GraphicsRectItem::boundingRect() const
{
	QRectF bounding(-(m_width) / 2,
		-(m_height) / 2,
		m_width, m_height);
	return bounding;
}

//QPainterPath GraphicsRectItem::shape() const
//{
//	QPainterPath path;
//	path.addRect(boundingRect());
//	return qt_graphicsItem_shapeFromPath(path, pen());
//}

void GraphicsRectItem::setwidth(int width)
{
	m_width = width;
}

void GraphicsRectItem::setheight(int height)
{
	m_height = height;
}

void GraphicsRectItem::setOrder(int order)
{
	m_order = order;
}

void GraphicsRectItem::setInSpeed(int speed)
{
	m_inSpeed = speed;
}

void GraphicsRectItem::setWorkSpeed(int speed)
{
	m_workSpeed = speed;
}

//void GraphicsRectItem::setThreshold(int thresh)
//{
//	m_threshold = thresh;
//}

int GraphicsRectItem::getWidth() const
{
	return m_width;
}

int GraphicsRectItem::getHeight() const
{
	return m_height;
}

int GraphicsRectItem::getOrder() const
{
	return m_order;
}

int GraphicsRectItem::getInSpeed() const
{
	return m_inSpeed;
}

int GraphicsRectItem::getWorkSpeed() const
{
	return m_workSpeed;
}

//int GraphicsRectItem::getThresh() const
//{
//	return m_threshold;
//}

void GraphicsRectItem::resizeTo(SizeHandleRect::Direction dir, const QPointF &point)
{
	//old  one 
	QPointF  local = mapFromScene(point);
	QString  dirName;
	const QRectF & geom = this->boundingRect();
	QRect delta = this->rect().toRect();
	switch (dir) {
	case SizeHandleRect::Right:
		dirName = "Rigth";
		delta.setRight(local.x());
		break;
	case SizeHandleRect::RightTop:
		dirName = "RightTop";
		delta.setTopRight(local.toPoint());
		break;
	case SizeHandleRect::RightBottom:
		dirName = "RightBottom";
		delta.setBottomRight(local.toPoint());
		break;
	case SizeHandleRect::LeftBottom:
		dirName = "LeftBottom";
		delta.setBottomLeft(local.toPoint());
		break;
	case SizeHandleRect::Bottom:
		dirName = "Bottom";
		delta.setBottom(local.y());
		break;
	case SizeHandleRect::LeftTop:
		dirName = "LeftTop";
		delta.setTopLeft(local.toPoint());
		break;
	case SizeHandleRect::Left:
		dirName = "Left";
		delta.setLeft(local.x());
		break;
	case SizeHandleRect::Top:
		dirName = "Top";
		delta.setTop(local.y());
		break;
	default:
		break;
	}

	prepareGeometryChange();

	m_width = delta.width();
	m_height = delta.height();

	qDebug() << "resizeTo :" << dirName << "move:" << local << "newSize :" << QSize(m_width, m_height);
	updateGeometry();

	//new one  
	//QRect delta = this->rect().toRect();
	//QPointF pos = mapFromScene(point);
	//double wradius = boundingRect().width() / 2.0;
	//double hradius = boundingRect().height() / 2.0;
	//QPointF  topLeft = boundingRect().topLeft();
	////m_centerPointF = QPointF(topLeft.x() + pos.x() + wradius, topLeft.y() + pos.y() + hradius);
	//QPointF m_centerPointF = QPointF(topLeft.x() + wradius, topLeft.y() + hradius);
	//double  dist = sqrt(pow(m_centerPointF.x() - pos.x(), 2) + pow(m_centerPointF.y() - pos.y(), 2));
	////delta = QRect(m_centerPointF.x() - this->pos().x() - dist, m_centerPointF.y() - this->pos().y() - dist, dist * 2, dist * 2);
	///*delta = QRect(m_centerPointF.x(), m_centerPointF.y(), dist * 2, dist * 2);*/

	//double y = sqrt(pow(dist, 2) / (pow(wh_radius, 2) + 1));
	//double x = wh_radius * y;

	//delta = QRect(m_centerPointF.x() - pos.x() - x, m_centerPointF.y() - pos.y() - y, x, y);

	//prepareGeometryChange();

	//m_width = delta.width();
	//m_height = delta.height();

	////qDebug() << "resizeTo :" << dirName << "move:" << local << "newSize :" << QSize(m_width, m_height);
	//updateGeometry();

}

void GraphicsRectItem::setState(SelectionHandleState st)
{
	const Handles::iterator hend = m_handles.end();
	for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
	{
		(*it)->setState(st);
		SizeHandleRect *hndl = *it;
		switch (hndl->dir())
		{
		case SizeHandleRect::Left:
		case SizeHandleRect::Right:
		case SizeHandleRect::Bottom:
		case SizeHandleRect::Top:
			hndl->setVisible(false);
		}
	}
}

void GraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QColor c = QColor(Qt::red);
	c.setAlpha(160);
	QLinearGradient result(rect().topLeft(), rect().topRight());
	result.setColorAt(0, c.dark(150));
	result.setColorAt(0.5, c.light(200));
	result.setColorAt(1, c.dark(150));
	QPen   pen;
	//painter->setPen(pen());
	painter->setPen(pen);
	painter->setBrush(result);
	painter->drawRect(rect());
}

GraphicsEllipseItem::GraphicsEllipseItem(const QRect &rect, QGraphicsItem *parent)
	:GraphicsRectItem(rect, parent)
{

}

//QPainterPath GraphicsEllipseItem::shape() const
//{
//	QPainterPath  path;
//	path.addEllipse(boundingRect());
//	return  qt_graphicsItem_shapeFromPath(path, pen());
//}

void GraphicsEllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QColor c = QColor(Qt::red);
	c.setAlpha(160);
	QRectF rc = rect();
	qreal radius = qMax(rc.width(), rc.height());
	QRadialGradient result(rc.center(), radius);
	result.setColorAt(0, c.light(200));
	result.setColorAt(0.5, c.dark(150));
	result.setColorAt(1, c);
	QPen pen;
	//painter->setPen(pen());
	painter->setPen(pen);
	painter->setBrush(result);
	painter->drawEllipse(rc);
}

GraphicsLineItem::GraphicsLineItem(QGraphicsItem *parent)
	:GraphicsRectItem(QRect(0, 0, 0, 0), parent)
{
	// handles
	m_handles.reserve(SizeHandleRect::None);

	Handles::iterator  hend = m_handles.end();
	for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
		delete (*it);

	m_handles.clear();
	SizeHandleRect* shr = new SizeHandleRect(this,SizeHandleRect::LeftTop,this);
	m_handles.push_back(shr);

	shr = new  SizeHandleRect(this, SizeHandleRect::RightBottom, this);
	m_handles.push_back(shr);

	updateGeometry();
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	this->setAcceptHoverEvents(true);

	
}

//QPainterPath GraphicsLineItem::shape() const
//{
//	QPainterPath path;
//	path.moveTo(rect().topLeft());
//	path.lineTo(rect().bottomRight());
//	return qt_graphicsItem_shapeFromPath(path, pen());
//}

void GraphicsLineItem::resizeTo(SizeHandleRect::Direction dir, const QPointF &point)
{
	QPointF local = mapFromScene(point);
	prepareGeometryChange();

	const QRectF &geom = this->boundingRect();
	QRect delta = this->rect().toRect();

	switch (dir)
	{
	case SizeHandleRect::LeftTop:
		delta.setTopLeft(local.toPoint());
		break;
	case SizeHandleRect::RightBottom:
		delta.setBottomRight(local.toPoint());
		break;
	default:
		break;
	}

	m_width = delta.width();
	m_height = delta.height();

	updateGeometry();
}

void GraphicsLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QColor c = QColor(Qt::red);
	QPen pen;
	//painter->setPen(pen());
	painter->setPen(pen);
	painter->drawLine(rect().topLeft(), rect().bottomRight());
}

GraphicsItemGroup::GraphicsItemGroup(QGraphicsItemGroup *group, QGraphicsItem *parent)
	:GraphicsRectItem(QRect(0, 0, 0, 0), parent)
	, m_group(group)
{
	if (m_group)
	{
		m_group->setParentItem(this);
		QRectF bound = m_group->boundingRect();

		bound.normalized();
		prepareGeometryChange();
		m_width = bound.width();
		m_height = bound.height();

		QPointF  delta = pos() - bound.center();
		setTransform(transform().translate(delta.x(), delta.y()));
		setTransformOriginPoint(bound.center());

		m_group->setTransformOriginPoint(bound.center());

		updateGeometry();

	}
}

QRectF GraphicsItemGroup::boundingRect() const
{
	QRectF  bounding = m_group->mapRectFromParent(m_group->boundingRect());
	return bounding;
}

//QPainterPath GraphicsItemGroup::shape() const
//{
//	return m_group->shape();
//}

void GraphicsItemGroup::resizeTo(SizeHandleRect::Direction dir, const QPointF &point)
{

}

void GraphicsItemGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPointF origin = mapFromScene(pos());

	QPointF origin1 = m_group->transformOriginPoint();

	QColor c1 = QColor(Qt::blue);
	//  c1.setAlpha(180);
	painter->setBrush(c1);
	painter->drawEllipse(origin.x() - 3, origin.y() - 3, 6, 6);


	QColor c2 = QColor(Qt::green);
	//  c2.setAlpha(180);
	painter->setBrush(c2);
	painter->drawEllipse(origin1.x() - 3, origin1.y() - 3, 6, 6);

}

//GraphicsPixmapItem::GraphicsPixmapItem(const QRect & rect,const QString & pixmap, QGraphicsItem * parent)
//	:GraphicsRectItem(rect,parent)
//{
//	m_pixmap = pixmap;
//}

GraphicsPixmapItem::GraphicsPixmapItem(const QRect & rect,QPixmap pixmap, QPixmap Oripixmap, SelectType type, QGraphicsItem * parent)
	:GraphicsRectItem(rect, parent)
{
	m_pixmap = pixmap;
	m_Orimap  = Oripixmap;
	m_xpixmap = m_pixmap;
	m_xpixmap.fill(Qt::transparent);
	m_Type = type;

	m_Rect = rect;
	m_graydiff = 0;

	//宽除以高 
	wh_radius = double(rect.width()) / rect.height();
    
}

//QPainterPath GraphicsPixmapItem::shape() const
//{
//	QPainterPath path;
//	path.addRect(boundingRect());
//	return qt_graphicsItem_shapeFromPath(path, pen());
//}

//uint GraphicsPixmapItem::getDianMoveSpeed() const
//{
//	return m_dianMoveSpeed;
//}
//
//uint GraphicsPixmapItem::getWorkSpeed() const
//{
//	return m_workSpeed;
//}

SelectType GraphicsPixmapItem::SelectTypetype() const
{
	return m_Type;
}

qreal GraphicsPixmapItem::getScale() const
{
	return  (double)this->rect().toRect().width() / m_Rect.width();
}

QImage GraphicsPixmapItem::getFinalImage()
{
	QPixmap map = m_Orimap.scaled(this->rect().toRect().width(), this->rect().toRect().height());
	return map.toImage();
}

qreal GraphicsPixmapItem::getThresh() const
{
	return m_thresh;
}

OutLineStruct GraphicsPixmapItem::getOutline() const
{
	return m_OutLine;
}

int GraphicsPixmapItem::getGray() const
{
	return m_graydiff;
}

void GraphicsPixmapItem::setMatSrc(cv::Mat src)
{
	m_MatSrc = src;
}

void GraphicsPixmapItem::setWidthWithRate(int width)
{
	m_width = width;
	m_height = m_width / wh_radius;
	updateGeometry();
	update();
}

void GraphicsPixmapItem::setHeightWithREate(int height)
{
	m_height = height;
	m_width = m_height * wh_radius;
	updateGeometry();
	update();
}

void GraphicsPixmapItem::setRotataX(qreal a)
{
	QTransform  trans;
	trans.rotate(a, Qt::XAxis);
	m_pixmap = m_pixmap.transformed(trans);
	//
	m_TransForm *= trans;
	//updateGeometry();
	update();
}

void GraphicsPixmapItem::setRotateY(qreal a)
{
	QTransform  trans;
	trans.rotate(a, Qt::YAxis);
	m_pixmap = m_pixmap.transformed(trans);
	m_TransForm *= trans;
	//updateGeometry();
	update();
}

void GraphicsPixmapItem::setThreshBlackWhite(qreal a)
{
	m_thresh = a;
	//根据这个生成m_pixmap 
	QPixmap mp = m_Orimap;
	cv::Mat matimage;// QImage2cvMat(mp.toImage());
	matimage = getBinaryImageGcodeOnlyImage(m_MatSrc, a);
	QImage  image = cvMat2QImage(matimage);
	m_pixmap = QPixmap::fromImage(image);
	update();
}

void GraphicsPixmapItem::setThreshVector(qreal a)
{
	m_thresh = a;
	//根据这个生成m_pixmap 
	QPixmap mp = m_Orimap;
	cv::Mat matimage;// = QImage2cvMat(mp.toImage());
	OutLineStruct data1;
	data1.thred = a;
	matimage = getContoursGcodeOnlyImage(m_MatSrc,data1);
	QImage  image = cvMat2QImage(matimage);
	m_pixmap = QPixmap::fromImage(image);
	update();
}

void GraphicsPixmapItem::setGary(qreal n)
{
	m_graydiff = n;

	QPixmap mp = m_Orimap;
	cv::Mat matimage = QImage2cvMat(mp.toImage());
	matimage = getGrayImageGcodeOnlyImage(matimage,n);
	QImage  image = cvMat2QImage(matimage);
	m_pixmap = QPixmap::fromImage(image);
	update();
}

void GraphicsPixmapItem::setOutLine(OutLineStruct n)
{
	m_OutLine = n;

	//QPixmap mp = m_Orimap;
	cv::Mat matimage;// = QImage2cvMat(mp.toImage());
	matimage = getContoursGcodeOnlyImage(m_MatSrc,n);
	QImage image = cvMat2QImage(matimage);
	m_pixmap = QPixmap::fromImage(image);
	update();
}

void GraphicsPixmapItem::setLines(vector<LINE> lines)
{
	m_Lines = lines;
    
	QPainter  painter(&m_xpixmap);
	painter.setPen(QPen(QColor(0,0,0),1));
	painter.setRenderHint(QPainter::Antialiasing,true);
	//painter.begin(&m_xpixmap);

	// 绘制直线
	for (int j = 0; j < m_Lines.size(); j++)//
	{
		QPoint start, end;
		start.setX(m_Lines[j].start.x);
		start.setY(m_Lines[j].start.y);
		end.setX(m_Lines[j].end.x);
		end.setY(m_Lines[j].end.y);
		painter.drawLine(start, end);
	}
	painter.end();
}

//void GraphicsPixmapItem::setDianMoveSpeed(uint speed)
//{
//	m_dianMoveSpeed = speed;
//}
//
//void GraphicsPixmapItem::setWorkSpeed(uint speed)
//{
//	m_workSpeed = speed;
//}

void GraphicsPixmapItem::resizeTo(SizeHandleRect::Direction dir, const QPointF & point)
{
	QRect delta = this->rect().toRect();
	QPointF pos = mapFromScene(point);
	double wradius = boundingRect().width() / 2.0;
	double hradius = boundingRect().height() / 2.0;
	QPointF  topLeft = boundingRect().topLeft();
	//m_centerPointF = QPointF(topLeft.x() + pos.x() + wradius, topLeft.y() + pos.y() + hradius);
	m_centerPointF = QPointF(topLeft.x() + wradius, topLeft.y() + hradius);
	double  dist = sqrt(pow(m_centerPointF.x() - pos.x(), 2) + pow(m_centerPointF.y() - pos.y(), 2));
	//delta = QRect(m_centerPointF.x() - this->pos().x() - dist, m_centerPointF.y() - this->pos().y() - dist, dist * 2, dist * 2);
	/*delta = QRect(m_centerPointF.x(), m_centerPointF.y(), dist * 2, dist * 2);*/

	double y = sqrt(pow(dist, 2) / (pow(wh_radius, 2) + 1));
	double x = wh_radius * y;

	delta = QRect(m_centerPointF.x() - pos.x() - x, m_centerPointF.y() - pos.y() - y, x, y);

	prepareGeometryChange();

	m_width = delta.width();
	m_height = delta.height();

	//qDebug() << "resizeTo :" << dirName << "move:" << local << "newSize :" << QSize(m_width, m_height);
	updateGeometry();
}

void GraphicsPixmapItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	//原先的画法 
	QRect  rect1;
	rect1.setRect(rect().left(), rect().top(), rect().width(), rect().height());
	//painter->setCompositionMode(QPainter::CompositionMode_Clear);
	//
    //m_pixmap.scaled(rect().width(), rect().height());
	//1 画法
	painter->drawPixmap(rect1, m_pixmap);
	//2 画法
	//painter->drawPixmap(rect1,m_xpixmap);

	//新的画法  
	//int x = this->pos().x();
	//int y = this->pos().y();
	//QTransform trans;
	//trans.scale(rect().width()/ m_Orimap.width(), rect().height()/m_Orimap.height());
	//trans.translate(x - (m_Orimap.width() / 2), y - (m_Orimap.height() / 2));
	//// 绘制直线
	//for (int j = 0; j < m_Lines.size(); j++)//
	//{
	//	QPoint start,end;
	//	start.setX(m_Lines[j].start.x);
	//	start.setY( m_Lines[j].start.y );
	//	end.setX(m_Lines[j].end.x);
	//	end.setY(m_Lines[j].end.y);
	//	start = trans.map(start);
	//	end = trans.map(end);
	//	painter->drawLine(start, end);
	//}

}

GraphicsTextItem::GraphicsTextItem(const QRect & rect, QString text, QFont font, QGraphicsItem * parent)
	:GraphicsRectItem(rect, parent)
{
	m_Text = text;
	m_Font = font;
	m_Rect = rect;

	wh_radius = double(m_Rect.width()) / m_Rect.height();
	pointsize  = m_Font.pointSize();
	
	//QTransform TRANS = m_TransForm;
	//int n = 10;
	//
	//setHandleState();
}

//QPainterPath GraphicsTextItem::shape() const
//{
//	QPainterPath path;
//	path.addRect(boundingRect());
//	return qt_graphicsItem_shapeFromPath(path, pen());
//}

void GraphicsTextItem::setHandleState(SelectionHandleState st)
{
	const Handles::iterator hend = m_handles.end();
	for (Handles::iterator it = m_handles.begin(); it != hend; ++it) 
	{
		SizeHandleRect *hndl = *it;;
		switch (hndl->dir()) 
		{
		case SizeHandleRect::LeftTop:
			
			break;
		case SizeHandleRect::Top:
			hndl->setVisible(false);
			break;
		case SizeHandleRect::RightTop:
		
			break;
		case SizeHandleRect::Right:
			hndl->setVisible(false);
			break;
		case SizeHandleRect::RightBottom:
			
			break;
		case SizeHandleRect::Bottom:
			hndl->setVisible(false);
			break;
		case SizeHandleRect::LeftBottom:
			
			break;
		case SizeHandleRect::Left:
			hndl->setVisible(false);
			break;    
		case SizeHandleRect::Rotate:
			break;
		default:
			break;
		}
	}
}

void GraphicsTextItem::setTextAndFont(const QString & text, QString & font)
{
	//int n = m_Font.fo
	m_Font = font;
	if (!text.isEmpty())
	{
		m_Text = text;
	}
	m_Font.setPointSize(pointsize);
	QFontMetrics fm(m_Font);
	QRect rec = fm.boundingRect(m_Text);
	m_Rect = rec;
	//setwidth(rec.width());
	//setheight(rec.height());
	wh_radius = m_Rect.width() / m_Rect.height();
	pointsize = m_Font.pointSize();
	setwidth(rec.width());
	setheight(rec.height());
	updateGeometry();
	//update();
}

void GraphicsTextItem::setState(SelectionHandleState st)
{
	const Handles::iterator hend = m_handles.end();
	for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
	{
		(*it)->setState(st);
		SizeHandleRect *hndl = *it;
		switch (hndl->dir())
		{
		case SizeHandleRect::Left:
		case SizeHandleRect::Right:
		case SizeHandleRect::Bottom:
		case SizeHandleRect::Top:
			hndl->setVisible(false);
		}
	}
}

QImage GraphicsTextItem::getImage() const
{
	QRect  rect(0,0, this->rect().width(), this->rect().height());
	QPixmap  pixmap(this->rect().width(),this->rect().height());
	pixmap.fill(qRgba(255, 255, 255, 0));
	
	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing);

	painter.setFont(m_Font);

	painter.setPen(QColor(0, 0, 0));
	painter.drawText(rect, m_Text);

	QPixmap  tem_pixmap = pixmap.transformed(m_TransForm);
	QImage im = tem_pixmap.toImage();

	//getTextGcodeOnlyImage();
	//im.transformed(m_TransForm);
	//cv::Mat  src = QImage2cvMat(im);
	//imshow("srcimaghe", src);
	return im;

}

QPixmap GraphicsTextItem::getPixmap() const
{
	QRect  rect(0, 0, this->rect().width(), this->rect().height());
	QPixmap  pixmap(this->rect().width(), this->rect().height());
	pixmap.fill(qRgba(255, 255, 255, 255));
	//pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing);

	painter.setFont(m_Font);

	painter.setPen(QColor(0, 0, 0));
	painter.drawText(rect, m_Text);

	QPixmap  tem_pixmap = pixmap.transformed(m_TransForm);

	cv::Mat matimage = QImage2cvMat(tem_pixmap.toImage());
	matimage = getTextGcodeOnlyImage(matimage);
	QImage image = cvMat2QImage(matimage);
	tem_pixmap = QPixmap::fromImage(image);
    
	return tem_pixmap;
}

QFont GraphicsTextItem::getFont() const
{
	return m_Font;
}

QString GraphicsTextItem::getText() const
{
	return m_Text;
}

SelectType GraphicsTextItem::SelectTypetype() const
{
	return TEXT;
}

void GraphicsTextItem::setRotataX(qreal a)
{
	//第一种设置方法 
	m_TransForm.rotate(a, Qt::XAxis);
	//update();
	getImage();
	update();
}

void GraphicsTextItem::setRotateY(qreal a)
{
	//第二种设置方法 
	QTransform  trans;
	trans.rotate(a, Qt::YAxis);
	m_TransForm *= trans;
	getImage();
	update();
}

void GraphicsTextItem::resizeTo(SizeHandleRect::Direction dir, const QPointF & point)
{
	/*QPointF  local = mapFromScene(point);
	QString  dirName;
	const QRectF & geom = this->boundingRect();
	QRect delta = this->rect().toRect();
	switch (dir) {
	case SizeHandleRect::Right:
		dirName = "Rigth";
		delta.setRight(local.x());
		break;
	case SizeHandleRect::RightTop:
		dirName = "RightTop";
		delta.setTopRight(local.toPoint());
		break;
	case SizeHandleRect::RightBottom:
		dirName = "RightBottom";
		delta.setBottomRight(local.toPoint());
		break;
	case SizeHandleRect::LeftBottom:
		dirName = "LeftBottom";
		delta.setBottomLeft(local.toPoint());
		break;
	case SizeHandleRect::Bottom:
		dirName = "Bottom";
		delta.setBottom(local.y());
		break;
	case SizeHandleRect::LeftTop:
		dirName = "LeftTop";
		delta.setTopLeft(local.toPoint());
		break;
	case SizeHandleRect::Left:
		dirName = "Left";
		delta.setLeft(local.x());
		break;
	case SizeHandleRect::Top:
		dirName = "Top";
		delta.setTop(local.y());
		break;
	default:
		break;
	}*/

	QRect delta = this->rect().toRect();
	QPointF pos = mapFromScene(point);
	double wradius = boundingRect().width() / 2.0;
	double hradius = boundingRect().height() / 2.0;
	QPointF  topLeft = boundingRect().topLeft();
	//m_centerPointF = QPointF(topLeft.x() + pos.x() + wradius, topLeft.y() + pos.y() + hradius);
	m_centerPointF = QPointF(topLeft.x() + wradius, topLeft.y() + hradius);
	double  dist = sqrt(pow(m_centerPointF.x() - pos.x(), 2) + pow(m_centerPointF.y() - pos.y(), 2));
	//delta = QRect(m_centerPointF.x() - this->pos().x() - dist, m_centerPointF.y() - this->pos().y() - dist, dist * 2, dist * 2);
	/*delta = QRect(m_centerPointF.x(), m_centerPointF.y(), dist * 2, dist * 2);*/

	double y = sqrt(pow(dist, 2) / (pow(wh_radius, 2) + 1));
	double x = wh_radius * y;

	delta = QRect(m_centerPointF.x() - pos.x()- x , m_centerPointF.y() - pos.y() - y,x ,y);

	prepareGeometryChange();

	m_width = delta.width();
	m_height = delta.height();

	//qDebug() << "resizeTo :" << dirName << "move:" << local << "newSize :" << QSize(m_width, m_height);
	updateGeometry();
}

void GraphicsTextItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	QRect  rect1;
	rect1.setRect(rect().left(), rect().top(), rect().width(), rect().height());
	int n = m_Font.pixelSize();
	int p = m_Font.pointSize();

	/*QFontMetrics fm1(m_Font);
	qreal hRatio(rect().height() / m_Rect.height()), wRatio(1.0);

	if (m_Font.pixelSize() > 0) 
		m_Font.setPixelSize(m_Font.pixelSize() * hRatio);
	else if (m_Font.pointSizeF() > 0)
		m_Font.setPointSizeF(m_Font.pointSizeF() * hRatio);
	else if (m_Font.pointSize() > 0) 
		m_Font.setPointSize(m_Font.pointSize() * hRatio);

	QFontMetricsF fm2(m_Font);
	wRatio = fm1.width(m_Text) / fm2.width(m_Text);
	m_Font.setStretch(wRatio * m_Font.stretch());*/
	//设置换行 
    //QTextOption option1(Qt::AlignHCenter | Qt::AlignVCenter);
    //option1.setWrapMode(QTextOption::WordWrap);

	m_Font.setPointSize((pointsize*rect().width())/ m_Rect.width());


	//原先的绘制方式  绘制文字  这里由于是整个场景额绘制指针 所以执行旋转等操作较困难
	/*painter->setFont(m_Font);
	painter->setPen(QColor(0, 0, 0));
	painter->drawText(rect1, m_Text);
*/
	//新的绘制方式  1.绘制图片 
	//painter->drawImage(rect1, getImage());

	//新的绘制方式 2 绘制pixmap 
	//painter->setCompositionMode(Qt::OpaqueMode);
	//painter->setBackground(Qt::transparent);
	//painter->setBackgroundMode(Qt::TransparentMode);
	//painter->setOpacity(0.4);
	painter->drawPixmap(rect1, getPixmap());

}
