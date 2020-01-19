#pragma once

#include <QSpinBox>

class LspinBox : public QSpinBox
{
	Q_OBJECT

public:
	LspinBox(QWidget *parent);
	~LspinBox();

protected:
	void wheelEvent(QWheelEvent *event);
};
