#ifndef SELECTTOOLBOX_H
#define SELECTTOOLBOX_H

#include "SSToolBox.h"
#include "Transformation.h"
#include <QGraphicsItem>
#include "OutLineImage.h"

class QStackedWidget;
class QButtonGroup;
class WorkingWid;
class ThresholdW;
class TextWidget;
class GrayDiffuse;
//class OutLineImage;

#define  TOOLBOX_WIDTH   320
#define  TOOLBOX_CHILD_WIDTH    320 - 6

class SelectToolBox : public SSToolBox
{
	Q_OBJECT
public:
	enum SELECT_TYPE
	{
		BLACK_LEVEL,
		GRAY_LEVEL,
		SVG_LEVEL,
		TEXT_LEVEL
	};

public:
	explicit  SelectToolBox(QWidget *parent = nullptr);
	~SelectToolBox();
	void ResetBox();
	//void paintEvent(QPaintEvent *event);

signals:
	void LevelButtonClicked(int id);
	void CreateGcode(int id);
	//所有信号发出去的
	void NotifySceneStateChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);

public slots :
	void buttonClicked();
	void buttonGroupClicked(int id);
	void gcodeButtonClicked();
    void adjectSelfSize(int id);
	void SelectFromFile(bool selected, const SelectData& data);
	void TextAndFontChanged(QString& text, QString &font);
	void handleOutlineChange(OutLineStruct& data);

	void handleSceneEvent(QGraphicsItem::GraphicsItemChange change, const QVariant &value);

	void handlePositionChange(QList<SSToolBoxItem *>& childlist, SSToolBoxItem * trans, const QVariant &value);

	void handleRotationChange(QList<SSToolBoxItem *>& childlist, SSToolBoxItem * trans, const QVariant &value);

	void handleScaleChanged(QList<SSToolBoxItem *>& childlist, SSToolBoxItem * trans, const QVariant &value);
	void handleSelectedChanged(QVariant& value);

	void setGrayPart(const QVariant& value = QVariant());
	void SetBlackWhitePart(const QVariant& value = QVariant());
	void SetVectorPart(const QVariant& value =  QVariant());
	void SetTextPart(const QVariant& value = QVariant());

public slots:
	void ItemwidthChanged(int width);
	void ItemheightChanged(int height);
	void ItemrotateChanged(int r);
	void ItemxChanged(int x);
	void ItemyChanged(int y);
	void ItemGrayDiffuseChanged(int n);
	void ItemFlipChanged(int flip);
	void ItemUpChanged();
	void ItemDownChanged();
	void SceneLineChanged();

	void  ItemprintOrderChanged(int n);
	void  IteminchingSpeedChanged(int n);
	void  ItemworkingSpeedChanged(int n);

	void ItemThresholdChanged(int n,uint type);

private:
	void createResolutionBar();
	void createExposureBar();
	void createWhiteBalanceBar();
	void createImageAdjustmentBar();
	void createHistogramBar();

private:
	QSize           m_BaseSize;
	Transformation* tem_Trans;
	WorkingWid*     m_WorkingWid;
	ThresholdW*     m_ThresholdW;
	TextWidget*     m_pTextWidget;
	QButtonGroup *  buttonGroup;
	QStackedWidget* m_StackWidget;
	GrayDiffuse*    m_graywidget;//误差分配算法界面 
	OutLineImage*   m_outline;//边缘检测算子界面 
	//预览设置 
	SSToolBoxItem *m_pResolutionBar;

	//曝光
	SSToolBoxItem *m_pExposureBar;

	//白平衡
	SSToolBoxItem *m_pWhiteBalanceBar;

	//图像调整
	SSToolBoxItem *m_pImageAdjustment;

	//直方图
	SSToolBoxItem *m_pHistogram;

};

#endif
