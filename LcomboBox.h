#pragma once

#include <QComboBox>

class LcomboBox : public QComboBox
{
	Q_OBJECT

public:
	LcomboBox(QWidget *parent);
	~LcomboBox();

protected:
	void wheelEvent(QWheelEvent *e);
};
