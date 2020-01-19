#pragma once
#ifndef SSCAMERATOOLBOX_H
#define SSCAMERATOOLBOX_H

#include "SSToolBox.h"

class Transformation;
class TextWidget;

class SSTestToolBox : public SSToolBox
{
	Q_OBJECT

public:
	explicit SSTestToolBox(QWidget *parent = nullptr);

signals:
	void TextAndFontTrans(QString& text, QString &font);

public slots:
	void buttonClicked();
	void TextAndFontChanged(QString& text, QString &font);

private:
	void createResolutionBar();
	void createExposureBar();
	void createWhiteBalanceBar();
	void createImageAdjustmentBar();
	void createHistogramBar();


private:
	//分辨率
	SSToolBoxItem *m_pResolutionBar;

	//曝光
	SSToolBoxItem *m_pExposureBar;

	//白平衡
	SSToolBoxItem *m_pWhiteBalanceBar;

	//图像调整
	SSToolBoxItem *m_pImageAdjustment;

	//直方图
	SSToolBoxItem *m_pHistogram;

	Transformation*  m_pTransformation;
	TextWidget*      m_pTextWidget;
};

#endif 
