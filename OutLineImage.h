#pragma once

#include <QWidget>
#include "ui_OutLineImage.h"

class QButtonGroup;
class QPaintEvent;

struct OutLineStruct
{
	int  operar = 0;
	int  border = 0;
	int  thred  = 128;
};
Q_DECLARE_METATYPE(OutLineStruct)


class OutLineImage : public QWidget
{
	Q_OBJECT

public:
	OutLineImage(QWidget *parent = nullptr);
	~OutLineImage();

	void Init();
	void setOutLineState(OutLineStruct& data);
	void paintEvent(QPaintEvent *event);

signals:
	void NotifyArithmeticChange(OutLineStruct& data);

public slots:
	void buttonGroupClicked(int id);
	void arithmeticChanged();
	void buttonGroupClicked_A(int id);

private:
	Ui::OutLineImage ui;
	QButtonGroup *  buttonGroup;
	QButtonGroup *  buttonGroup_A;
};
