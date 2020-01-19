#pragma once

#include <QWidget>
#include "ui_Transformation.h"
#include "OutLineImage.h"

class QPaintEvent;

struct SelectData
{
	struct transPart
	{
		qreal   width;
		qreal   height;
		qreal   rotate;
		qreal   movex;
		qreal   movey;
		unsigned int   flipmodel;
	};
	struct workingPart
	{
		qreal  printOrder;
		qreal  inchingSpeed;
		qreal  workingSpeed;
		workingPart()
		{
			printOrder = 1;
			inchingSpeed = 1500;
			workingSpeed = 500;
		}
	};
	OutLineStruct m_OutLine;
	transPart   m_TransData;
	workingPart m_WorkingData;
	qreal       m_Type;//类型  黑与白  灰度  矢量 文本 
	qreal       m_Threlod;//阈值 
	qreal       m_GrayDiff;//灰度的算法 

	SelectData()
	{
		m_TransData.width = 0;
		m_TransData.height  = 0;
		m_TransData.rotate = 0;
		m_TransData.movex = 0;
		m_TransData.movey = 0;
		m_TransData.flipmodel = 0;
		m_Threlod = 168;
		m_GrayDiff = 0;
		m_OutLine.border = 0;
		m_OutLine.operar = 0;
	}
};

struct TextData
{
	QString  text;
	QString  font;
};

struct YuzhiData
{
	int  Threold;
	uint type;
};

Q_DECLARE_METATYPE(SelectData)
Q_DECLARE_METATYPE(TextData)
Q_DECLARE_METATYPE(YuzhiData);


class Transformation : public QWidget
{
	Q_OBJECT
public:

public:
	Transformation(QWidget *parent = nullptr);
	~Transformation();
	void CreateLayout();
	void setWidth(int width);
	void setHeight(int height);
	void rotateValue(int value);
	void moveXValue(int value);
	void moveYValue(int value);
	void setWidthEnable(bool en);
	void setHeightEnable(bool en);
	void paintEvent(QPaintEvent *event);

signals:
	void widthChanged(int width);
	void heightChanged(int height);
	void rotateChanged(int r);
	void xChanged(int x);
	void yChanged(int y);
	void flipChanged(int flip);
	void upChanged();
	void downChanged();
	void lineChanged();

public slots:
	void  widthEditingFinished();
	void  heightEditingFinished();
	void  rotateChangedFinished();
	void  movexChangedFinished();
	void  moveyChangedFinished();
	void  rotateSliderChanged(int value);
	void  moveXSliderChanged(int value);
	void  moveYSliderChanged(int value);
	void  flipModelChanged(int value);
	void  upMove();
	void  downMove();
	void  lineClicked();

private:
	Ui::Transformation ui;
};
