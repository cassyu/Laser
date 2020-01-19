#pragma once

#include <QWidget>
#include "ui_ExportWidget.h"

class QButtonGroup;
class ExportWidget : public QWidget
{
	Q_OBJECT
public:
	enum
	{
		BUTTON_GEN,
		BUTTON_EXPORT,
		BUTTON_TEXT
	};

public:
	ExportWidget(QWidget *parent = nullptr);
	~ExportWidget();
	void Createconnect();
	void paintEvent(QPaintEvent *event);

signals:
	void GcodeButtonClicked(int id);

public slots:
	void buttonClicked(int id);

private:
	Ui::ExportWidget ui;
	QButtonGroup *  buttonGroup;
};
