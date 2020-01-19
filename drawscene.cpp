#include "drawscene.h"
#include<QGraphicsSceneMouseEvent>
#include<QGraphicsRectItem>
#include <QDebug>
#include <QFileDialog>
#include "drawtool.h"
#include "drawobj.h"
#include "QProgressIndicator.h"
#include "OutLineImage.h"
#include "GraphWidget.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "QtOpencvConvert.h"
#include "opencv2/imgproc/types_c.h"
#include "SGcodeGenerate.h"
#include <QTextCodec>
#include "mainwindow.h"


DrawScene::DrawScene(QObject *parent)
	:QGraphicsScene(parent)
{
	//QBrush brush(QColor(221,125,69));
	//setBackgroundBrush(brush);
}

void DrawScene::mouseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{ 
	switch (mouseEvent->type())
	{
	case QEvent::GraphicsSceneMousePress:
		QGraphicsScene::mousePressEvent(mouseEvent);
		break;
	case QEvent::GraphicsSceneMouseMove:
		QGraphicsScene::mouseMoveEvent(mouseEvent);
		break;
	case QEvent::GraphicsSceneMouseRelease:
		QGraphicsScene::mouseReleaseEvent(mouseEvent);
		break;

	}
}

void DrawScene::selectFile(int type)
{
	this->clearSelection();
	if (type != TEXT)
	{
		QFileDialog* fileDialog = new QFileDialog(MainWindow::GetMainWindow());
		fileDialog->setWindowTitle(tr(u8"打开图片"));
		fileDialog->setDirectory(".");
		fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
		fileDialog->setFileMode(QFileDialog::ExistingFile);
		fileDialog->setViewMode(QFileDialog::Detail);

		QStringList  filenames;
		if (fileDialog->exec())
		{
			filenames = fileDialog->selectedFiles();
		}

	/*	for (auto tmp : filenames)
		{
			qDebug() << tmp;
		}*/
		//加载一个图片并显示  
		if (!filenames.isEmpty())
		{
			QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题 UTF8  GB2312
			std::string  filename = code->fromUnicode(filenames[0]).data();
			QImage img;

			if (img.load(filenames[0]))
			{
				int b = 10;
			}
			QPixmap mp;
			mp.fill(Qt::transparent);
			mp = mp.fromImage(img);
			//原先资源的获取方式1 
			//cv::Mat matimage = QImage2cvMat(img);
			//新的资源获取方式2  
			cv::Mat matimage = cv::imread(filename);
			//imshow("matimage", matimage);
			qreal w_radius = this->width();
			qreal h_radius = this->height();
			QRect rects = QRect(0, 0, matimage.cols, matimage.rows);
			if (matimage.cols > 500 || matimage.rows > 500)
			{
				//rects = QRect(0, 0, 500, 500);
			}

			rects.setWidth((matimage.cols / PIXEL_RATE));
			rects.setHeight((matimage.rows / PIXEL_RATE));
			OutLineStruct  outdata1;
			outdata1.thred = 128;
			cv::Mat outimage= getGcodeWithIdOnlyImage(matimage, type,outdata1);
			//imshow("outimage", outimage);
			//vector<LINE> lines = getGcodeWithIdShape(matimage, type);
			//dynamic_cast<GraphicsPixmapItem*>(item)->setLines(lines);
			img = cvMat2QImage(outimage);
			/*if (img.save("testw.png", "png"))
			{
				int n = 10;
			}*/
			QPixmap imp = mp.fromImage(img);
			GraphicsItem* item = new GraphicsPixmapItem(rects, imp, mp, SelectType(type), NULL);
			dynamic_cast<GraphicsPixmapItem*>(item)->setMatSrc(matimage);
			item->setPos(0, 0);
			item->setSelected(true);
			addItem(item);

			//关于原生的pixmap的测试 
		/*	QGraphicsPixmapItem* pixmap = new QGraphicsPixmapItem(imp);
			addItem(pixmap);*/
		    SelectData  data;
			data.m_TransData.width = rects.width();
			data.m_TransData.height = rects.height();
			data.m_Type = type;
			data.m_Threlod = 168;
			if (type == VECTORS)
			{
				data.m_Threlod = 128;
			}
			emit itemSelectedFromFile(true,data);

		}
		else
		{
			emit itemSelectedFromFile(false);
		}
	}
	else
	{
		QString text = u8"LotMax";
		QFont  font;
		font.setFamily("Microsoft YaHei");
		font.setPointSize(40);
		QFontMetrics fm(font);
		QRect rec = fm.boundingRect(text);
		//QAbstractGraphicsShapeItem* item = new GraphicsTextItem(rec, text, font, NULL);
		GraphicsItem* item = new GraphicsTextItem(rec, text, font, NULL);
		item->setSelected(true);
		item->setPos(0, 0);
		addItem(item);
		setFocusItem(item);
		SelectData  data;
		data.m_TransData.width = rec.width();
		data.m_TransData.height = rec.height();
		data.m_Type = type;
		emit itemSelectedFromFile(true, data);
	}
}

void DrawScene::setParamValue(int value, ParamType type)
{
	QGraphicsItem * focusItem = QGraphicsScene::focusItem();

}

//将场景的事件传递给绘图工具
void DrawScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	//尝试在这里决定要使用哪个工具
   //
	//this->clearSelection();
	QTransform  transform;
	QGraphicsItem* nowItem = itemAt(mouseEvent->scenePos(), transform);
	SizeHandleRect* sizehanlerect = dynamic_cast<SizeHandleRect*>(nowItem);
	//DrawTool::c_drawShape  =selection;
	if (nowItem && sizehanlerect == nullptr)
	{
		//nowItem->setSelected(true);
		DrawTool::c_Item = nowItem;
		nowItem->setSelected(true);
		//this->update();
		//nowItem->setFocus();
	}
	if (sizehanlerect)
	{
		if (sizehanlerect->dir() == SizeHandleRect::Direction::Rotate)
		{
			DrawTool::c_drawShape = rotation;
		}
	}
	m_bPressed = true;
	DrawTool* tool = DrawTool::findTool(DrawTool::c_drawShape);
	if (tool)
		tool->mousePressEvent(mouseEvent, this);
}

//将场景鼠标移动事件传递给绘图工具 
void DrawScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QTransform  transform;
	QGraphicsItem* nowItem = itemAt(mouseEvent->scenePos(), transform);
	SizeHandleRect* sizehanlerect = dynamic_cast<SizeHandleRect*>(nowItem);
	QList<QGraphicsView*> allViews = views();
	if (sizehanlerect)
	{
		if (!m_bPressed)
		{
			if (sizehanlerect->dir() == SizeHandleRect::Direction::Rotate)
			{
				DrawTool::c_drawShape = rotation;
			}
		}
	}
	else
	{
		if (!m_bPressed)
		{
			DrawTool::c_drawShape = selection;
		}
	}
	DrawTool * tool = DrawTool::findTool(DrawTool::c_drawShape);
	if (tool)
		tool->mouseMoveEvent(mouseEvent, this);
}

//将场景鼠标的释放事件  传递给绘图工具 
void DrawScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	m_bPressed = false;
	//DrawTool::c_Item = nullptr;
	//this->clearSelection();
	DrawTool * tool = DrawTool::findTool(DrawTool::c_drawShape);
	if (tool)
		tool->mouseReleaseEvent(mouseEvent, this);
}

Qt::CursorShape DrawScene::getCursor(SizeHandleRect::Direction dir)
{
	switch (dir) {
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
		//return Qt::WaitCursor;//
	case SizeHandleRect::Rotate:
		return Qt::WaitCursor;//转动十字
	default:
		break;
	}
	return Qt::ArrowCursor;
}

void DrawScene::setContextMenu(QMenu * menu)
{
	if (menu != nullptr)
	{
		m_ContextMenu = menu;
	}
}

QMenu * DrawScene::getContextMenu()
{
	return m_ContextMenu;
}

void DrawScene::TestEvent()
{

}

void DrawScene::itemsChanged(QGraphicsItem::GraphicsItemChange change, const QVariant & value)
{
	if (change == QGraphicsItem::ItemSelectedHasChanged)
	{
		//qDebug() << " Item Selected : " << value.toString() << "item ";;
		QList<QGraphicsItem *>  items = QGraphicsScene::selectedItems();
		if (!items.count())
		{
			QVariant   value1(NONE_SEL);
			emit  sceneEventNotify(change, value1);
		}
		else //如果当前有获取的  那么就需要获取数据进行抓换 
		{
			if (items.count() == 1)
			{
				GraphicsPixmapItem* pitem = dynamic_cast<GraphicsPixmapItem*>(items.first());
				GraphicsTextItem* ptextitem = dynamic_cast<GraphicsTextItem*>(items.first());
				if (pitem != nullptr)
				{
					QVariant  value2;
					SelectData  data1;
					data1.m_TransData.movex = pitem->pos().x();
					data1.m_TransData.movey = pitem->pos().y();
					data1.m_TransData.width = pitem->getWidth();
					data1.m_TransData.height = pitem->getHeight();
					data1.m_TransData.rotate = pitem->rotation();
					data1.m_WorkingData.printOrder = pitem->getOrder();
					data1.m_WorkingData.inchingSpeed = pitem->getInSpeed();
					data1.m_WorkingData.workingSpeed = pitem->getWorkSpeed();
					data1.m_Type = pitem->SelectTypetype();
					data1.m_GrayDiff = pitem->getGray();
					data1.m_OutLine = pitem->getOutline();
					data1.m_Threlod = pitem->getThresh();
					value2.setValue(data1);
					emit  sceneEventNotify(change, value2);
				}
				if (ptextitem != nullptr)
				{
					QVariant  value2;
					SelectData  data1;
					data1.m_TransData.movex = ptextitem->pos().x();
					data1.m_TransData.movey = ptextitem->pos().y();
					data1.m_TransData.width = ptextitem->getWidth();
					data1.m_TransData.height = ptextitem->getHeight();
					data1.m_TransData.rotate = ptextitem->rotation();
					data1.m_WorkingData.printOrder = ptextitem->getOrder();
					data1.m_WorkingData.inchingSpeed = ptextitem->getInSpeed();
					data1.m_WorkingData.workingSpeed = ptextitem->getWorkSpeed();
					data1.m_Type = ptextitem->SelectTypetype();
					value2.setValue(data1);
					emit  sceneEventNotify(change, value2);
				}
			}
		}
	}//旋转改变 
	else if (change == QGraphicsItem::ItemRotationHasChanged)
	{
		//qDebug() << "Item Rotation Changed:" << value.toDouble();
		emit  sceneEventNotify(change, value);
	}
	else if (change == QGraphicsItem::ItemTransformOriginPointHasChanged)
	{
		//qDebug() << "ItemTransformOriginPointHasChanged:" << value.toPointF();
	}
	else if (change == QGraphicsItem::ItemPositionChange)
	{
		//qDebug() << "ItemTransformOriginPointHasChanged:" << value.toPointF();
		emit  sceneEventNotify(change, value);
	}//位置改变 
	else if (change == QGraphicsItem::ItemPositionHasChanged)
	{
		QVariant::Type  type = value.type();
		//qDebug() << "ItemTransformOriginPointHasChanged:" << value.toPointF();
		emit  sceneEventNotify(change, value);
	}
	else if (QGraphicsItem::ItemScaleHasChanged == change)
	{
		//qDebug() << "ItemTransformOriginPointHasChanged:" << value.toPointF();
		emit  sceneEventNotify(change, value);
	}
}

void DrawScene::setTextAndFont(QString &text, QString & font)
{
	QList<QGraphicsItem *>  items = QGraphicsScene::selectedItems();
	GraphicsTextItem * item = 0;
	if (items.count() == 1)
	{
		item = qgraphicsitem_cast<GraphicsTextItem*>(items.first());
		if (item != nullptr)
		{
			//int n = 10;
			//text.isEmpty();
			if (!text.isEmpty())
			{
				item->setTextAndFont(text, font);
			}
		}
	}
}

void DrawScene::handleStateChangeEvent(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	switch (change)
	{
	case QGraphicsItem::ItemPositionChange://x的改变 
	{
		QList<QGraphicsItem *>  items = QGraphicsScene::selectedItems();
		GraphicsPixmapItem * item = 0;
		GraphicsTextItem* textitem = 0;
		if (items.count() == 1)
		{
			item = dynamic_cast<GraphicsPixmapItem*>(items.first());
			textitem = dynamic_cast<GraphicsTextItem*>(items.first());
			if (item)
			{
				item->setX(value.toInt());
			}
			if (textitem)
			{
				textitem->setX(value.toInt());
			}
		}
	}
		break;
	case QGraphicsItem::ItemMatrixChange://灰度图像算法的改变
	{
		QList<QGraphicsItem *>  items = QGraphicsScene::selectedItems();
		GraphicsPixmapItem * item = 0;
		if (items.count() == 1)
		{
			item = dynamic_cast<GraphicsPixmapItem*>(items.first());
			if (item)
			{
				if (item->SelectTypetype() == GRAY)
				{
					QProgressIndicator* pindicator = new QProgressIndicator();
					pindicator->setColor(Qt::red);
					pindicator->startAnimation();
					pindicator->show();
					item->setGary(value.toInt());
					pindicator->stopAnimation();
					pindicator->close();
				}
			}
		}
     }
		break;
	case QGraphicsItem::ItemVisibleChange:
		break;
	case QGraphicsItem::ItemEnabledChange://暂定为向上移
	{
		QList<QGraphicsItem *>  items = QGraphicsScene::selectedItems();
		if (items.count() == 1)
		{
			QGraphicsItem * selectedItem = items.first();
			if (selectedItem)
			{
				const QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

				qreal zValue = 0;
				if (value.toBool())//z轴向上移 
				{
					for (const QGraphicsItem* item : overlapItems)
					{
						if (item->zValue() >= zValue)
						{
							zValue = item->zValue() + 0.1;
						}
					}
				}
				else//z轴向下移 
				{
					for (const QGraphicsItem *item : overlapItems)
					{
						if (item->zValue() <= zValue)
						{
							zValue = item->zValue() - 0.1;
						}
					}
				}

				selectedItem->setZValue(zValue);
			}
		}
	}
		break;
	case QGraphicsItem::ItemSelectedChange:
		break;
	case QGraphicsItem::ItemParentChange://暂时用作flip的改变  
	{
		QList<QGraphicsItem *>  items = QGraphicsScene::selectedItems();
		GraphicsPixmapItem * item = 0;
		GraphicsTextItem* textitem = 0;
		if (items.count() == 1)
		{
			item = dynamic_cast<GraphicsPixmapItem*>(items.first());
			textitem = dynamic_cast<GraphicsTextItem*>(items.first());
			if (item)
			{
				if (value.toInt() == 0)//
				{
					item->setRotateY(180);
				}
				else
				{
					item->setRotataX(180);
				}
			}
			if (textitem)
			{
				if (value.toInt() == 0)
				{
					textitem->setRotateY(180);
				}
				else
				{
					textitem->setRotataX(180);
				}
			}
		}
	}
		break;
	case QGraphicsItem::ItemChildAddedChange:
		break;
	case QGraphicsItem::ItemChildRemovedChange:
		break;
	case QGraphicsItem::ItemTransformChange:
		break;
	case QGraphicsItem::ItemPositionHasChanged://暂定为y的改变 
	{
		QList<QGraphicsItem *>  items = QGraphicsScene::selectedItems();
		GraphicsPixmapItem * item = 0;
		GraphicsTextItem* textitem = 0;
		if (items.count() == 1)
		{
			item = dynamic_cast<GraphicsPixmapItem*>(items.first());
			textitem = dynamic_cast<GraphicsTextItem*>(items.first());
			if (item)
			{
				item->setY(value.toInt());
			}
			if (textitem)
			{
				textitem->setY(value.toInt());
			}
		}
	}
		break;
	case QGraphicsItem::ItemTransformHasChanged:
		break;
	case QGraphicsItem::ItemSceneChange://用作辅助线的改变 
	{
		QList<QGraphicsView*> list = QGraphicsScene::views();
		if (list.count() == 1)
		{
			GraphWidget* item = nullptr;
			item = dynamic_cast<GraphWidget*>(list.first());
			if (nullptr != item)
			{
				item->showLine();
				QBrush brush = item->backgroundBrush();
				item->setBackgroundBrush(brush);
			}
		}
	}
		break;
	case QGraphicsItem::ItemVisibleHasChanged:
		break;
	case QGraphicsItem::ItemEnabledHasChanged:
		break;
	case QGraphicsItem::ItemSelectedHasChanged:
		break;
	case QGraphicsItem::ItemParentHasChanged:
		break;
	case QGraphicsItem::ItemSceneHasChanged:
		break;
	case QGraphicsItem::ItemCursorChange:
		break;
	case QGraphicsItem::ItemCursorHasChanged:
		break;
	case QGraphicsItem::ItemToolTipChange://文字的改变
	{
		QList<QGraphicsItem *>  items = QGraphicsScene::selectedItems();
		GraphicsTextItem * item = 0;
		if (items.count() == 1)
		{
			item = dynamic_cast<GraphicsTextItem*>(items.first());
			if (item)
			{
				if (value.canConvert<TextData>())
				{
					//value.value<TextData>().font;
					//value.value<TextData>().text;
					item->setTextAndFont(value.value<TextData>().text, value.value<TextData>().font);
				}
			}
		}
	}
		break;
	case QGraphicsItem::ItemToolTipHasChanged://设置为 边缘检测算法的改变 
	{
		QList<QGraphicsItem *>  items = QGraphicsScene::selectedItems();
		GraphicsPixmapItem * item = 0;
		if (items.count() == 1)
		{
			item = dynamic_cast<GraphicsPixmapItem*>(items.first());
			if (item)
			{
				if (value.canConvert<OutLineStruct>())
				{
					//value.value<TextData>().font;
					//value.value<TextData>().text;
					OutLineStruct data;
					data.border = value.value<OutLineStruct>().border;
					data.operar = value.value<OutLineStruct>().operar;
					//弹出等待框
					QProgressIndicator* pindicator = new QProgressIndicator();
					pindicator->setColor(Qt::red);
					pindicator->startAnimation();
					pindicator->show();

					item->setOutLine(data);

					pindicator->stopAnimation();
					pindicator->close();
				}
			}
		}
	}
		break;
	case QGraphicsItem::ItemFlagsChange://阈值的改变 
	{
		QList<QGraphicsItem *>  items = QGraphicsScene::selectedItems();
		GraphicsPixmapItem * item = 0;
		if (items.count() == 1)
		{
			item = dynamic_cast<GraphicsPixmapItem*>(items.first());
			if (item)
			{
				if (value.canConvert<YuzhiData>())
				{
					YuzhiData  data1 = value.value<YuzhiData>();
					if (data1.type == 0)
					{
						item->setThreshBlackWhite(data1.Threold);
					}
					else
					{
						item->setThreshVector(data1.Threold);
					}
				}
			}
		}
	}
		break;
	case QGraphicsItem::ItemFlagsHaveChanged://标签的改变   工作速度的改变  
	{
		QList<QGraphicsItem *>  items = QGraphicsScene::selectedItems();
		GraphicsRectItem * item = 0;
		if (items.count() == 1)
		{
			item = dynamic_cast<GraphicsRectItem*>(items.first());
			if (item)
			{
				item->setWorkSpeed(value.toInt());
			}
		}
	}
		break;
	case QGraphicsItem::ItemZValueChange://
		break;
	case QGraphicsItem::ItemZValueHasChanged:
		break;
	case QGraphicsItem::ItemOpacityChange://透明度的改变  可以先做order的改变用 
	{
		QList<QGraphicsItem *>  items = QGraphicsScene::selectedItems();
		GraphicsRectItem * item = 0;
		if (items.count() == 1)
		{
			item = dynamic_cast<GraphicsRectItem*>(items.first());
			if (item)
			{
				item->setOrder(value.toInt());
			}
		}
	}
		break;
	case QGraphicsItem::ItemOpacityHasChanged://透明度的改变  可以用作inchspeed速度的改变
	{
		QList<QGraphicsItem *>  items = QGraphicsScene::selectedItems();
		GraphicsRectItem * item = 0;
		if (items.count() == 1)
		{
			item = dynamic_cast<GraphicsRectItem*>(items.first());
			if (item)
			{
				item->setInSpeed(value.toInt());
			}
		}
	}
		break;
	case QGraphicsItem::ItemScenePositionHasChanged:
		break;
	case QGraphicsItem::ItemRotationChange:
		break;
	case QGraphicsItem::ItemRotationHasChanged:
	{
		QList<QGraphicsItem *>  items = QGraphicsScene::selectedItems();
		GraphicsPixmapItem * item = 0;
		GraphicsTextItem* textitem = 0;
		if (items.count() == 1)
		{
			item = dynamic_cast<GraphicsPixmapItem*>(items.first());
			textitem = dynamic_cast<GraphicsTextItem*>(items.first());
			if (item)
			{
				item->setRotation(value.toInt());
			}
			if (textitem)
			{
				textitem->setRotation(value.toInt());
			}
		}
	}
		break;
	case QGraphicsItem::ItemScaleChange://暂时把这个设置为Y的改变 
	{
		QList<QGraphicsItem *>  items = QGraphicsScene::selectedItems();
		GraphicsPixmapItem * item = 0;
		if (items.count() == 1)
		{
			item = dynamic_cast<GraphicsPixmapItem*>(items.first());
			if (item)
			{
				item->setHeightWithREate(value.toInt());
				QVariant value(QPointF(item->getWidth(), item->getHeight()));
				emit  sceneEventNotify(QGraphicsItem::ItemScaleHasChanged, value);
			}
		}
	}
		break;
	case QGraphicsItem::ItemScaleHasChanged:
	{
		QList<QGraphicsItem *>  items = QGraphicsScene::selectedItems();
		GraphicsPixmapItem * item = 0;
		if (items.count() == 1)
		{
			item  = dynamic_cast<GraphicsPixmapItem*>(items.first());
			if (item)
			{
				item->setWidthWithRate(value.toInt());
				QVariant value(QPointF(item->getWidth(), item->getHeight()));
				emit  sceneEventNotify(QGraphicsItem::ItemScaleHasChanged, value);
			}
		}
	}
		break;
	case QGraphicsItem::ItemTransformOriginPointChange:
		break;
	case QGraphicsItem::ItemTransformOriginPointHasChanged:
		break;
	default:
		break;
	}
}
