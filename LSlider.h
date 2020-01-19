#pragma once

#include <QSlider>

class LSlider : public QSlider
{
	Q_OBJECT

public:
	LSlider(QWidget *parent);
	~LSlider();

protected:
	void wheelEvent(QWheelEvent * e);
};
