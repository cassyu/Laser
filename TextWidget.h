#pragma once

#include <QWidget>
#include "ui_TextWidget.h"

class TextWidget : public QWidget
{
	Q_OBJECT

public:
	TextWidget(QWidget *parent = nullptr);
	~TextWidget();
    
	void Init();
	void paintEvent(QPaintEvent *event);

signals:
	void  CurrentTextAndFont(QString& text,QString &font);

public slots:
	void TextChanged();

private:
	Ui::TextWidget ui;
};
