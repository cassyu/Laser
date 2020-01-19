#pragma once

#include <QObject>
#include <QPainter>
#include <QProxyStyle>


class CustomTabStyle : public QProxyStyle
{
	QSize sizeFromContents(ContentsType type,const QStyleOption* option,const QSize &size,const QWidget* widget)const;
	void drawControl(ControlElement element,const QStyleOption* option,QPainter* painter,const QWidget* widget)const;

public:
 };
