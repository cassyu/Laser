#include "GraphWidget.h"
#include "GraphWidget.h"
#include <math.h>
//#include <math>
#include <QPainter>
#include <QKeyEvent>
//#include <QRandomGenerator>
#include <QPixmap>
//#include <QGLWidget>

GraphWidget::GraphWidget(QWidget *parent)
	: QGraphicsView(parent)
{
	setCacheMode(CacheBackground);
	setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	//setRenderHint(QPainter::Antialiasing);//抗锯齿 
	//setRenderHint(QPainter::HighQualityAntialiasing);//片段渲染 和 离屏渲染 
	//setRenderHints(QPainter::Antialiasing  | QPainter::SmoothPixmapTransform);//采用平滑pixmap转换算法  比如双线性插值
	//setRenderHint(QPainter::NonCosmeticDefaultPen);
	//setRenderHint(QPainter::Qt4CompatiblePainting);
	//setRenderHint(QPainter::LosslessImageRendering);
	//setRenderHints(QPainter::NonCosmeticDefaultPen | QPainter::SmoothPixmapTransform);
	setRenderHint(QPainter::SmoothPixmapTransform);
	setTransformationAnchor(AnchorUnderMouse);
	//setOptimizationFlag(QGraphicsView::IndirectPainting);
	setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing);
	//setViewport(new QGLWidget());
	//scale(qreal(0.7), qreal(0.7));
	//setViewport(new QWidget);
	//setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
	setCacheMode(QGraphicsView::CacheBackground);
	setMinimumSize(200, 200);
	m_bShowLine = true;
	//setMaximumSize(10000,10000);
}

void GraphWidget::showLine()
{
	m_bShowLine = !m_bShowLine;
	//update();
}

void GraphWidget::itemMoved()
{
}

void GraphWidget::zoomIn()
{
	scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
	scaleView(1 / qreal(1.2));
}

void GraphWidget::keyPressEvent(QKeyEvent * event)
{
	switch (event->key()) {
	case Qt::Key_Up:
		break;
	case Qt::Key_Down:
		
		break;
	case Qt::Key_Left:
		break;
	case Qt::Key_Right:
		break;
	case Qt::Key_Plus:
		zoomIn();
		break;
	case Qt::Key_Minus:
		zoomOut();
		break;
	case Qt::Key_Space:
	case Qt::Key_Enter:
		shuffle();
		break;
	default:
		QGraphicsView::keyPressEvent(event);
	}
}

void GraphWidget::timerEvent(QTimerEvent * event)
{
}

//#if QT_CONFIG(wheelevent)
void GraphWidget::wheelEvent(QWheelEvent * event)
{
	scaleView(pow((double)2, -event->delta() / 240.0));
}
//#endif

void GraphWidget::drawBackground(QPainter * painter, const QRectF & rect)
{
	Q_UNUSED(rect);
	if (m_bShowLine)
	{
		QRectF  sceneRect1 = this->sceneRect();
		QRectF  rightShadow(sceneRect1.right(), sceneRect1.top() + 5, 5, sceneRect1.height());
		QRectF  bottomShadow(sceneRect1.left() + 5, sceneRect1.bottom(), sceneRect1.width(), 5);
		//if (rightShadow.intersects(rect) || rightShadow.contains(rect))
		//	painter->fillRect(rightShadow, Qt::darkGray);
		//if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
		//	painter->fillRect(bottomShadow, Qt::darkGray);

		QLinearGradient  gradient(sceneRect1.topLeft(), sceneRect1.bottomRight());
		gradient.setColorAt(0, Qt::white);
		gradient.setColorAt(1, Qt::lightGray);
		painter->fillRect(rect.intersected(sceneRect1), gradient);
		painter->setBrush(Qt::NoBrush);
		//painter->drawRect(sceneRect1);
		QRect  rect1;
		//rect1.setLeft(sceneRect1.left());
		rect1.setRect(sceneRect1.left(), sceneRect1.top(), sceneRect1.width(), sceneRect1.height());
		if (QPixmap(":/Resourse/u5.png").isNull())
		{
			//int n = 10;
		}
		//if (m_bShowLine)
		painter->drawPixmap(rect1, QPixmap(":/Resourse/u5.png"));
	}

	//几个标的 
	//painter->drawPixmap(QPoint(0, 0), QPixmap(":/Resourse/rotation.png"));
	//painter->drawPixmap(QPoint(rect1.left(), rect1.top()), QPixmap(":/Resourse/rotation.png"));
	//painter->drawPixmap(QPoint(rect1.right(), rect1.bottom()), QPixmap(":/Resourse/rotation.png"));
	//painter->drawPixmap(sceneRect1, QPixmap(":/Images/logo"));

	/*QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
		sceneRect.width() - 4, sceneRect.height() - 4);
	QString message(tr("Click and drag the nodes around, and zoom with the mouse "
		"wheel or the '+' and '-' keys"));

	QFont  font = painter->font();
	font.setBold(true);
	font.setPointSize(14);
	painter->setFont(font);
	painter->setPen(Qt::lightGray);
	painter->drawText(textRect.translated(2, 2), message);
	painter->setPen(Qt::black);
	painter->drawText(textRect, message);*/


	//画中心圆
	/*painter->setPen(QPen(QColor(0, 160, 230), 2));
	painter->drawEllipse(QPointF(0,0),4,4);*/


}

void GraphWidget::scaleView(qreal scaleFactor)
{
	qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	/*if (factor < 0.07 || factor > 100)
		return;*/
	if (factor < 0.5 || factor > 30)
			return;

	scale(scaleFactor, scaleFactor);
}

void GraphWidget::shuffle()
{
}
